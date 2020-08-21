#include "ILI9341_t3_ERIS.h"

void ILI9341_t3_ERIS::setSD(SdFs *ptr){pSD = ptr;}
void ILI9341_t3_ERIS::setPWMPin(uint8_t pin){
    backlight = pin;
    pinMode(backlight, OUTPUT);
    //analogWriteFrequency(TFT_LED_PWM, 42000);
    //analogWrite(backlight, 5);
}
void ILI9341_t3_ERIS::begin(){
    ILI9341_t3n::begin();
    ILI9341_t3n::useFrameBuffer(1);
    //setClock(30000000); //110000000
    setClipRect();
    setOrigin();
    fillScreen(ILI9341_BLACK);
    setTextColor(ILI9341_YELLOW);
    setTextSize(2);
    setRotation(3);
    println("Online...");
    pFB = _pfbtft;          
    pSD->chdir("I/U/W");
    if (pSD->exists("bluehex.ile")){println("File found...");}
    file.open("bluehex.ile", O_READ);
    char buf[32];
    file.fgets(buf,sizeof(buf)); // thow away these valuse - wallpaper is fixed @ 320x240
    file.fgets(buf,sizeof(buf)); //
    for (uint32_t i = 0; i < (320 * 240); i += 1){file.read(&pFB[i],2);}
    file.close();
    updateScreen();
}

void ILI9341_t3_ERIS::bltSD(const char *path, const char *filename,int16_t x,int16_t y,UIBLTAlphaType alpha_type){
  SdFile file;
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
  pSD->chdir(path);                //change file path
  file.open(filename, O_READ);        //open image to read
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
        //debug - testing performance increase of reading an entire row of img data at once
        if ((w - mx - nx)<= 320){
          if (z == 0) file.read(&dwbf,(w - mx - nx)*2);
          dw = dwbf[z];
        } else file.read(&dw,2); //should never execute...but just in case

        toggle ^= true;
        //if alpha is enabled mask any colors close to black
        if (alpha_type == AT_NONE){pFB[ifb + z] = dw;}
        else if (alpha_type == AT_TRANS && (dw & 0xE79C) != 0){pFB[ifb + z] = dw;}
        else if (alpha_type == AT_HATCHBLK){
          if ((dw & 0xE79C) != 0) pFB[ifb + z] = dw;
          else if (toggle) pFB[ifb + z] = 0; //pFB[i] ^= pFB[i];
        }
        else if (alpha_type == AT_HATCHXOR){
          if ((dw & 0xE79C) != 0) pFB[ifb + z] = dw;
          else if (toggle) pFB[ifb + z] = pFB[ifb + z]^pFB[ifb + z];
        }
      }
      if (x + w > 320L){file.seekCur( (x + w - 320L) * 2);} //clip in x dimension (right) - skip unused data away
    }
    else{
      //since y index is now off screen close the file and return
      file.close();
      return;
    }
  }
  file.close();
}

void ILI9341_t3_ERIS::bltSDFullScreen(char *filename){
  //full screen block transfer with no clipping - used for full screen images matching the screen resolution
  pSD->chdir("./I/U/W");
  file.open(filename, O_READ);
  if (file.available() == 0){ //file not found
    Serial.print("ILI9341_t3_ERIS::bltSDFullScreen File Not Found:");
    Serial.println(filename);
    return;
  }
  file.seekSet(15); //skip the header - header will always be 15 bytes for full screen wallpaper
  //for (unsigned long i = (320 * 64) ; i < (320 * 240); i += 32){
  for (unsigned long i = 0; i < (320 * 240); i += 1){ //32,64
    file.read(&pFB[i],2);
  }
  file.close();
}