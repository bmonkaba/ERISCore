#include "ILI9341_t3_ERIS.h"

#define ANIMATION_CHUNKS_PER_FRAME 2

static volatile bool dmabusy;

void FASTRUN renderCompleteCB(){
    Serial.println("renderCompleteCB");
    dmabusy=false;
}

bool _busy(){
    return dmabusy;
}

bool ILI9341_t3_ERIS::updateScreenAsyncFrom(ILI9341_t3_ERIS* draw,bool update_cont){
            bool rval;
            //Serial.printf("%u\n",(uint32_t)(void*)draw->_pfbtft);
            //Serial.flush();
            dmabusy=true;
            setFrameCompleteCB(&renderCompleteCB);
            rval= updateScreenAsync(false);
            return rval;
        }

bool Animation::getNextFrameChunk(){
    if (pSD == NULL) return false;
    if (chunk==0){
      chunk++;
      sprintf(filename,"%03u.ile",frame);
      frame++;
      if(last_frame > 0 && frame > last_frame) frame = 1;
      pSD->chdir(_path);
      if(pSD->exists(filename)){
          //Serial.println("M Animation::getNextFrameChunk(): OK");    
          return true; 
      }else{
          last_frame = frame - 1;
          frame = 1;
          sprintf(filename,"%03u.ile",frame);
          if(pSD->exists(filename)){
              //Serial.println("M Animation::getNextFrameChunk(): CYCLE OK");
              return true;
          }
      }
      Serial.printf(F("M SDCard Error  File: %s Not found at: %s\n"),filename,_path);
      return false;
    }
    chunk++;
    if(chunk>=ANIMATION_CHUNKS_PER_FRAME) chunk=0;
    return true;
}

void Animation::setSD(SdFs *ptr){pSD = ptr;}

void ILI9341_t3_ERIS::setSD(SdFs *ptr){pSD = ptr;}

void ILI9341_t3_ERIS::setPWMPin(uint8_t pin){
    backlight = pin;
    //pinMode(backlight, OUTPUT);
    analogWriteFrequency(backlight, 30000);
    analogWrite(backlight, 220);
}

void ILI9341_t3_ERIS::begin(){
    ILI9341_t3n::begin(tft_write_speed,tft_read_speed);
    //setFrameBuffer(FB1);
    //useFrameBuffer(true);
    fillScreen(ILI9341_BLACK);
    //pFB = 0;//FB1;
    //try and force a second buffer
    //setFrameBuffer(FB2);
    //useFrameBuffer(true);
    //fillScreen(ILI9341_PURPLE);
    //pFB2 = 0;//FB2;
    //setClock(30000000); //110000000
    setClipRect();
    setOrigin();
    //fillScreen(ILI9341_RED);
    setTextColor(CL(174, 143, 255));
    setTextSize(1);
    setRotation(1);
    println("Online...");      
    updateScreen();
    dmabusy=false;
}

/*
void ILI9341_t3_ERIS::flipBuffer(){
  if(pFB == _pfbtft){
    setFrameBuffer(pFB2);
    useFrameBuffer(true);
  } else{
    setFrameBuffer(pFB);
    useFrameBuffer(true);
  }
};
*/

void ILI9341_t3_ERIS::bltSD(const char *path, const char *filename,int16_t x,int16_t y,UIBLTAlphaType alpha_type){
  int16_t iy; // x & y index
  int16_t w;int16_t h; //width & height
  int16_t mx;        //left clip x offset
  int16_t nx;        //right clip x offset
  unsigned long ifb; //frame buffer index
  uint16_t dwbf[320];//file read pixel row input buffer
  uint16_t dw;       //pixel data
  char str[16];      //char buffer
  char *c;           //char pointer
  bool toggle = false;
  //pSD->chdir();
  if (!pSD->chdir(path)){ //change file path
    Serial.print(F("M ILI9341_t3_ERIS::bltSD Path not found: "));
    Serial.println(path);
    return;             
  }
  file.open(filename, O_READ);        //open image to read
  if (file.available() == 0){ //file not found
    Serial.print(F("M ILI9341_t3_ERIS::bltSD File Not Found: "));
    Serial.println(filename);
    //pSD->ls();
    return;
  }
  
  file.fgets(str,sizeof(str)); //read the header data
  file.fgets(str,sizeof(str)); //to get the image dimensions
  strtok(str," ");             //convert dimension text to numbers
  w = atol(str);
  c = strtok(NULL, " ");
  h = atol(c);

  //clip in y dimension (top)
  if (y<0) { //throw away rows which are off screen
    for (iy = 0; iy < -1L * y; iy += 1L){ //for each off screen row
      file.seekCur(w*2L);
      h -= 1; //reduce bitmap hight by 1 row
    }
    y = 0; //set y pos to 0 for the remaining portion of the bitmap
  }
  for (iy = y; iy < y + h; iy += 1){ //for each row
    toggle ^= true;
    if (iy < 240L) //clip in y dimension (bottom) - simply don't draw anything
    {
      mx = 0; nx = 0;
      if (x < 0L){file.seekCur(x * -2L);mx = -1L * x;} //clip in x dimension (left) - skip offscreen data
      ifb = (iy * 320L) + x + mx; //32bit index
      if (x + w > 320L){nx = x + w - 320L;}//clip in x dimension (right) - truncate copy to screen bounds
      for (uint16_t z = 0; z < (w - mx - nx); z += 1){
        file.read(&dw,2);
        toggle ^= true;
        //if alpha is enabled mask any colors close to black
        if (alpha_type == AT_NONE){_pfbtft[ifb + z] = dw;}
        else if (alpha_type == AT_TRANS && (dw & 0xE79C) != 0){_pfbtft[ifb + z] = dw;}
        else if (alpha_type == AT_HATCHBLK){
          if ((dw & 0xE79C) != 0) _pfbtft[ifb + z] = dw;
          else if (toggle) _pfbtft[ifb + z] = 0; //pFB[i] ^= pFB[i];
        }
        else if (alpha_type == AT_HATCHXOR){
          if ((dw & 0xE79C) != 0) _pfbtft[ifb + z] = dw;
          else if (toggle) _pfbtft[ifb + z] = _pfbtft[ifb + z]^_pfbtft[ifb + z];
        }
      }
      if (x + w > 320L){file.seekCur( (x + w - 320L) * 2);} //clip in x dimension (right) - skip unused data
    }
    else{
      //since y index is now off screen close the file and return
      file.close();
      return;
    }
  }
  file.close();
}

void ILI9341_t3_ERIS::bltSDAnimationFullScreen(Animation *an){
  //full screen block transfer with no clipping - used for full screen images matching the screen resolution
  pSD->chdir(an->getPath());
  file.open(an->getFileName(), O_READ);
  if (file.available() == 0){ //file not found
    Serial.print(F("M ILI9341_t3_ERIS::bltSDFullScreen File Not Found:"));
    Serial.println(an->getFileName());
    return;
  }
  //for (unsigned long i = (320 * 64) ; i < (320 * 240); i += 32){
  uint32_t chunk_size = (320 * 240)/ANIMATION_CHUNKS_PER_FRAME;
  uint32_t i = an->chunk * chunk_size;
  file.seekSet(15 + (i*2)); //skip the header - header will always be 15 bytes for full screen wallpaper
  file.read(&_pfbtft[i],chunk_size*2);
  //Serial.println()
  file.close();
}

void ILI9341_t3_ERIS::bltSDFullScreen(const char *filename){
  //full screen block transfer with no clipping - used for full screen images matching the screen resolution
  pSD->chdir("/I/U/W");
  file.open(filename, O_READ);
  if (file.available() == 0){ //file not found
    Serial.print(F("M ILI9341_t3_ERIS::bltSDFullScreen File Not Found:"));
    Serial.println(filename);
    pSD->ls();
    return;
  }
  file.seekSet(15); //skip the header - header will always be 15 bytes for full screen wallpaper
  //for (unsigned long i = (320 * 64) ; i < (320 * 240); i += 32){
  for (uint32_t i = 0; i < (320 * 240); i += 32){ //32,64
    file.read(&_pfbtft[i],64);
  }
  file.close();
}