#include "ILI9341_t3_ERIS.h"
#include "ErisGlobals.h"


static volatile bool dmabusy;

void renderCompleteCB(){
    Serial.println(F("renderCompleteCB"));
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
    analogWriteFrequency(backlight, 420);
    analogWrite(backlight, 190);
}

void FLASHMEM ILI9341_t3_ERIS::begin(){
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
    println("Display Online...");      
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
void FLASHMEM ILI9341_t3_ERIS::bltMem(Surface *dest, Surface *source,int16_t dest_x,int16_t dest_y,bltAlphaType alpha_type){
  bool toggle = false;
  int16_t source_x,source_y, d_x,d_y;
  uint32_t read_index,write_index;
  uint16_t *srcBuffer;
  uint16_t *dstBuffer;
  srcBuffer = source->getSurfaceBufferP();
  dstBuffer = dest->getSurfaceBufferP();
  //check for null
  if (!srcBuffer) return;
  if (!dstBuffer) return;
  //check for off surface blt attempt
  if ((dest_x > dest->getWidth()) || (0 > (dest->getWidth() + dest_x))) return;
  if ((dest_y > dest->getHeight()) || (0 > (dest->getHeight() + dest_y))) return;
  //for each pixel in the source buffer, write to the dest buffer if the x,y coords are within bounds
  for (source_y=0; source_y < source->getHeight();source_y +=1){
    for (source_x=0; source_x < source->getWidth();source_x +=1){
      //translate the source coords to the destination coords
      d_x = dest_x + source_x; 
      d_y = dest_y + source_y;
      write_index = (d_y * dest->getWidth()) + d_x;
      read_index = (source_y * source->getWidth()) + source_x;
      if(d_x < dest->getWidth() && d_x >= 0 && d_y < dest->getHeight() && d_y >= 0){
        //if in bounds then ok to write
        if (AT_NONE == alpha_type){
          dstBuffer[write_index] = srcBuffer[read_index];
        }else if(AT_HATCHXOR){
          if (toggle) dstBuffer[write_index] = srcBuffer[read_index];
          toggle ^= true;
        }else if(AT_TRANS){
          //TODO: unpack both source and dest, average the rgb values, repack and store in dest
        }
      }
    }
  }
}

void ILI9341_t3_ERIS::fillSurface(Surface *dest,uint16_t color){
  int16_t dest_x,dest_y;
  uint16_t *dstBuffer;
  uint32_t write_index;
  dstBuffer = dest->getSurfaceBufferP();
  for (dest_y=0; dest_y < dest->getHeight();dest_y +=1){
    for (dest_x=0; dest_x < dest->getWidth();dest_x +=1){
      write_index = (dest_y * dest->getWidth()) + dest_x;
      dstBuffer[write_index] = color;
    }
  }
};

void ILI9341_t3_ERIS::drawPixel(Surface *dest,int16_t x, int16_t y, uint16_t color){
  uint16_t *dstBuffer;
  dstBuffer = dest->getSurfaceBufferP();
  dstBuffer[(y * dest->getWidth()) + x] = color;
}

uint16_t ILI9341_t3_ERIS::readSurfacePixel(Surface *dest,int16_t x, int16_t y){
  uint16_t *dstBuffer;
  dstBuffer = dest->getSurfaceBufferP();
  return dstBuffer[(y * dest->getWidth()) + x];
}

uint16_t ILI9341_t3_ERIS::readPixel(int16_t x, int16_t y){ 
  return _pfbtft[(y * _width) + x];
}


void ILI9341_t3_ERIS::drawSurfaceLine(Surface *dest,int16_t start_x, int16_t start_y,int16_t end_x, int16_t end_y, uint16_t color){
  uint16_t *dstBuffer;
  dstBuffer = dest->getSurfaceBufferP();
  uint16_t* save = _pfbtft;
  _pfbtft = dstBuffer;
  _width = dest->getWidth();
  _height = dest->getHeight();
  drawLine(start_x, start_y, end_x, end_y, color);
  _pfbtft = save;
  _width = 320;
  _height = 240;
}

void ILI9341_t3_ERIS::drawSurfaceFill(Surface *dest,uint16_t color){
  uint16_t *dstBuffer;
  dstBuffer = dest->getSurfaceBufferP();
  uint16_t* save = _pfbtft;
  _pfbtft = dstBuffer;
  _width = dest->getWidth();
  _height = dest->getHeight();
  fillScreen(color);
  _pfbtft = save;
  _width = 320;
  _height = 240;
}


void FLASHMEM ILI9341_t3_ERIS::bltSD(const char *path, const char *filename,int16_t x,int16_t y,bltAlphaType alpha_type){
  bltSDB(_pfbtft, _width, _height, path, filename,x,y,alpha_type);
  return;
}

void FLASHMEM ILI9341_t3_ERIS::bltSDB(uint16_t *dest_buffer, uint16_t dest_buffer_width, uint16_t dest_buffer_height, const char *path, const char *filename,int16_t x,int16_t y,bltAlphaType alpha_type){
  int16_t iy; // x & y index
  int16_t w;int16_t h; //width & height
  int16_t mx;        //left clip x offset
  int16_t nx;        //right clip x offset
  unsigned long ifb; //frame buffer index
  //uint16_t dwbf[320];//file read pixel row input buffer
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
    if (iy < dest_buffer_height) //clip in y dimension (bottom) - simply don't draw anything
    {
      mx = 0; nx = 0;
      if (x < 0L){file.seekCur(x * -2L);mx = -1L * x;} //clip in x dimension (left) - skip offscreen data
      ifb = (iy * dest_buffer_width) + x + mx; //32bit index
      if (x + w > dest_buffer_width){nx = x + w - dest_buffer_width;}//clip in x dimension (right) - truncate copy to screen bounds
      for (uint16_t z = 0; z < (w - mx - nx); z += 1){
        file.read(&dw,2);
        toggle ^= true;
        //if alpha is enabled mask any colors close to black
        if (alpha_type == AT_NONE){dest_buffer[ifb + z] = dw;}
        else if (alpha_type == AT_TRANS && (dw & 0xE79C) != 0){dest_buffer[ifb + z] = dw;}
        else if (alpha_type == AT_HATCHBLK){
          if ((dw & 0xE79C) != 0) dest_buffer[ifb + z] = dw;
          else if (toggle) dest_buffer[ifb + z] = 0; //pFB[i] ^= pFB[i];
        }
        else if (alpha_type == AT_HATCHXOR){
          if ((dw & 0xE79C) != 0) dest_buffer[ifb + z] = dw;
          else if (toggle) dest_buffer[ifb + z] = dest_buffer[ifb + z]^dest_buffer[ifb + z];
        }
      }
      if (x + w > dest_buffer_width){file.seekCur( (x + w - dest_buffer_width) * 2);} //clip in x dimension (right) - skip unused data
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
  uint32_t chunk_size = (_width * _height)/ANIMATION_CHUNKS_PER_FRAME;
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