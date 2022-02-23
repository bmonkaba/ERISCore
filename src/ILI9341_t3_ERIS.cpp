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

bool FASTRUN Animation::getNextFrameChunk(){
  if (p_SD == NULL) return false;
  if (chunk==0){
    chunk++;
    sprintf(filename,"%03u.ile",frame);
    frame++;
    if(last_frame > 0 && frame > last_frame) frame = 1;
    p_SD->chdir(_path);
    if(p_SD->exists(filename)){
        //Serial.println("M Animation::getNextFrameChunk(): OK");    
        return true; 
    }else{
        last_frame = frame - 1;
        frame = 1;
        sprintf(filename,"%03u.ile",frame);
        if(p_SD->exists(filename)){
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

void FLASHMEM Animation::setSD(SdFs *ptr){p_SD = ptr;}

void FLASHMEM ILI9341_t3_ERIS::setSD(SdFs *ptr){p_SD = ptr;}

void FLASHMEM ILI9341_t3_ERIS::setPWMPin(uint8_t pin){
    backlight = pin;
    //pinMode(backlight, OUTPUT);
    analogWriteFrequency(backlight, 240);
    analogWrite(backlight, 240);
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


void FASTRUN ILI9341_t3_ERIS::bltSurface2Surface(Surface *dest, int16_t dest_x,int16_t dest_y, Surface *source, int16_t from_x, int16_t from_y, int16_t from_width, int16_t from_height,bltMode blt_mode){
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
        if (BLT_COPY == blt_mode){
          dstBuffer[write_index] = srcBuffer[read_index];
        }else if(BLT_HATCH_XOR){
          if (toggle) dstBuffer[write_index] = srcBuffer[read_index];
          toggle ^= true;
        }else if(BLT_BLK_COLOR_KEY){
          //TODO: unpack both source and dest, average the rgb values, repack and store in dest
        }
      }
    }
  }
}

void FASTRUN ILI9341_t3_ERIS::fillSurface(Surface *dest,uint16_t color){
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

void FASTRUN ILI9341_t3_ERIS::drawPixel(Surface *dest,int16_t x, int16_t y, uint16_t color){
  uint16_t *dstBuffer;
  dstBuffer = dest->getSurfaceBufferP();
  dstBuffer[(y * dest->getWidth()) + x] = color;
}

uint16_t FASTRUN ILI9341_t3_ERIS::readSurfacePixel(Surface *source,int16_t x, int16_t y){
  uint16_t *srcBuffer;
  srcBuffer = source->getSurfaceBufferP();
  return srcBuffer[(y * source->getWidth()) + x];
}

uint16_t FASTRUN ILI9341_t3_ERIS::readPixel(int16_t x, int16_t y){ 
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
  _width = SCREEN_WIDTH;
  _height = SCREEN_HEIGHT;
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
  _width = SCREEN_WIDTH;
  _height = SCREEN_HEIGHT;
}


/**
 * @brief blt directly from the SD card to the screen frame buffer at destination
 * 
 * @param path 
 * @param filename 
 * @param x dest coord 
 * @param y dest coord
 * @param blt_mode 
 */
void FASTRUN ILI9341_t3_ERIS::bltSD(const char *path, const char *filename,int16_t x,int16_t y,bltMode blt_mode){
  bltSDB(_pfbtft, _width, _height, path, filename,x,y,blt_mode); //blt from sd to buffer using the frame buffer as the destination
  return;
}




void FASTRUN ILI9341_t3_ERIS::bltSDAreaBufferDest(uint16_t *dest_buffer, int16_t dest_x,int16_t dest_y, uint16_t dest_buffer_width,\
 uint16_t dest_buffer_height, const char *file_name,int16_t src_x,int16_t src_y, uint16_t src_width, uint16_t src_height,bltMode blt_mode){
  int16_t iy; // x & y index
  int16_t w;int16_t h; //width & height
  int16_t mx;        //left clip x offset
  int16_t nx;        //right clip x offset
  unsigned long ifb; //frame buffer index
  //uint16_t dwbf[SCREEN_WIDTH];//file read pixel row input buffer
  uint16_t dw;       //pixel data
  char str[16];      //char buffer
  char *c;           //char pointer
  bool toggle = false;

  file.open(file_name, O_READ);//open image to read
  if (file.available() == 0){ //file not found
    Serial.print(F("M ILI9341_t3_ERIS::bltSD File Not Found: "));
    Serial.println(file_name);
    //pSD->ls();
    return;
  }
  file.fgets(str,sizeof(str)); //read the header data
  file.fgets(str,sizeof(str)); //to get the image dimensions
  strtok(str," ");             //convert dimension text to numbers
  w = atol(str);  //image width
  c = strtok(NULL, " ");
  h = atol(c);    //image height
  //seek to the source row
  if (src_y > 0) { 
    for (iy = 0; iy < src_y; iy += 1L){ //for each off screen row
      file.seekCur(w*2L);
      h -= 1; //reduce bitmap hight by 1 row
    }
    src_y = 0;
  }

  //clip in y dimension (top)
  if (dest_y < 0) { //throw away rows which are off screen
    for (iy = 0; iy < (-1L * dest_y); iy += 1L){ //for each off screen row
      file.seekCur(w*2L);
      h -= 1; //reduce bitmap hight by 1 row
    }
    dest_y = 0; //set y pos to 0 for the remaining portion of the bitmap
  }
  
  //for each remaining row
  for (iy = dest_y; iy < (dest_y + src_height); iy += 1){ 
    toggle ^= true;
    if (iy >= dest_buffer_height) break; //clip in y dimension (bottom) - simply don't draw anything
    mx = 0; nx = 0;
    //if (dest_x < 0L){file.seekCur(dest_x * -2L);mx += -1L * dest_x;} //clip in x dimension (left) - skip offscreen data
    ifb = (iy * dest_buffer_width) + dest_x + mx; //32bit index
    //if ((dest_x + src_width) > w){nx = (dest_x + src_width) - dest_buffer_width;}//clip in x dimension (right) - truncate copy to screen bounds
    //inital x offset
    file.seekCur(src_x * 2L); //seek to start_x
    //read the data for the row
    for (uint16_t z = 0; z < src_width; z += 1){
      file.read(&dw,2);
      
      //if alpha is enabled mask any colors close to black
      if (blt_mode == BLT_COPY){dest_buffer[ifb + z] = dw;}
      else if (blt_mode == BLT_BLK_COLOR_KEY && (dw & 0xE79C) != 0){dest_buffer[ifb + z] = dw;}
      else if (blt_mode == BLT_HATCH_BLK){
        toggle ^= true;
        if ((dw & 0xE79C) != 0) dest_buffer[ifb + z] = dw;
        else if (toggle) dest_buffer[ifb + z] = 0; //pFB[i] ^= pFB[i];
      }
      else if (blt_mode == BLT_HATCH_XOR){
        toggle ^= true;
        if ((dw & 0xE79C) != 0) dest_buffer[ifb + z] = dw;
        else if (toggle) dest_buffer[ifb + z] = dest_buffer[ifb + z]^dest_buffer[ifb + z];
      }else if (blt_mode == BLT_ADD){
        dest_buffer[ifb + z] += dw;
      }else if (blt_mode == BLT_SUB){
        dest_buffer[ifb + z] -= dw;
      }else if (blt_mode == BLT_MULT){
        dest_buffer[ifb + z] *= dw;
      }else if (blt_mode == BLT_DIV){
        if(dw > 0) dest_buffer[ifb + z] /= dw;
      }else if (blt_mode == BLT_AND){
        if(dw > 0) dest_buffer[ifb + z] = dw & dest_buffer[ifb + z];
      }else if (blt_mode == BLT_OR){
        if(dw > 0) dest_buffer[ifb + z] = dw | dest_buffer[ifb + z];
      }else if (blt_mode == BLT_XOR){
        if(dw > 0) dest_buffer[ifb + z] = dw ^ dest_buffer[ifb + z];
      }
      //if ( (dest_x + src_width) < w){file.seekCur( ((w - (dest_x + src_width)) * 2));} //clip in x dimension (right) - skip unused data
    }
    file.seekCur(2L* (w - (src_width + src_x))); //skip to the start of the next row
    toggle ^= true;
  }
  file.close();
}

bool FASTRUN ILI9341_t3_ERIS::getImageSize(const char* path,const char* filename,int32_t* width,int32_t* height){
  if (!p_SD->chdir(path)){ //change file path
    Serial.print(F("M ILI9341_t3_ERIS::bltSD Path not found: "));
    Serial.println(path);
    return false;             
  }
  file.open(filename, O_READ);        //open image to read
  if (file.available() == 0){ //file not found
    Serial.print(F("M ILI9341_t3_ERIS::bltSD File Not Found: "));
    Serial.println(filename);
    //pSD->ls();
    return false;
  }
  char *p;
  char str[16];      //char buffer

  file.fgets(str,sizeof(str)); //read the header data
  file.fgets(str,sizeof(str)); //to get the image dimensions
  strtok(str," ");             //convert dimension text to numbers
  *width = atol(str);
  p = strtok(NULL, " ");
  *height = atol(p);
  file.close();
  return true;
}

void FASTRUN ILI9341_t3_ERIS::printWithFont(const char* string_buffer,uint16_t x,uint16_t y,const char* font,uint16_t pt){
  int16_t j;
  const String end_of_index = "KEARNING\n";
  char terminator = '\n';
  char font_file_path[64];
  char font_kerning_file_path[64];
  char tmp[64];//tmp buffer to build the file name
  char *character;//[102];//storage for the loaded start stop x indexes from the kerning file
  int16_t startx[102];
  int16_t stopx[102];
  String line;
  
  //make sure the font exists
  strcpy(font_file_path,"/I/U/F/");
  strcat(font_file_path,font);
  strcat(font_file_path,"/");
  if (!p_SD->chdir(font_file_path)){
    setCursor(0,10);
    print("FONT PATH NOT FOUND! "); 
    return;
  }
  //build the kerning and font filenames
  sprintf(tmp,"%2hu",pt);//file name is just the pt number to two digits
  strcat(font_file_path,tmp);
  strcpy(font_kerning_file_path,font_file_path);//copy
  strcat(font_file_path,".ile");//add the file name sufixes
  strcat(font_kerning_file_path,".krn");
  setTextSize(pt);
  
  //open the kerning file
  FsFile kern = p_SD->open(font_kerning_file_path,O_RDONLY); 
  if (!kern) return;
  
  line = kern.readStringUntil(terminator,256);//throw away the header line
  //load the x start and stop indexes one time
  line = kern.readStringUntil(terminator,256); //get the first record
  j = 0;
  character = (char*)extmem_malloc(102);
  do{
    sscanf(line.c_str(),"%c %hu %hu\n",&character[j],&startx[j],&stopx[j]); //load the coords
    j+=1;
    line = kern.readStringUntil(terminator,256);
  }while(j<102);//end of index reached
  kern.close();

  //now walk the string buffer and 'print' 
  for(int16_t i = 0;i < strlen(string_buffer);i++){
    const char c = string_buffer[i]; //for each char
    j = -1;
    do{
      //search the kern x index
      j++;
      if (j>102) break;
    }while(c!=character[j]);

    //check stop_x it should be greater than zero.. if not the char was not found
    //else blt the font char
    if(j < 102){
      //bltSDAreaBufferDest(getFrameBuffer(),x,y,SCREEN_WIDTH,SCREEN_HEIGHT,font_file_path,start_x,0,stop_x-start_x,pt * 2,BLT_BLK_COLOR_KEY);
      bltSDAreaBufferDest(_pfbtft,x,y,SCREEN_WIDTH,SCREEN_HEIGHT,font_file_path,startx[j],0,stopx[j]-startx[j],pt * 2,BLT_BLK_COLOR_KEY);
      x += pt/1.75;//move index to the next char
    }else{
      //print("\n");
      //print(stop_x);
    }
  }
  extmem_free(character);
}

void FASTRUN ILI9341_t3_ERIS::bltRAMFileB(uint16_t *dest_buffer, uint16_t dest_buffer_width, uint16_t dest_buffer_height, File* file,int16_t x,int16_t y,bltMode blt_mode){
  int16_t iy; // x & y index
  int16_t mx;        //left clip x offset
  int16_t nx;        //right clip x offset
  unsigned long ifb; //frame buffer index
  uint16_t dw;       //pixel data
  int16_t w,h;
  char *c;
  char str[16];

  bool toggle = false;
  file->readBytesUntil('\n',&str[0],16);//read the file type (should be ILE565)
  file->readBytesUntil('\n',&str[0],16); //to get the image dimensions
 
  strtok(str," ");             //convert dimension text to numbers
  w = atol(str);
  c = strtok(NULL, " ");
  h = atol(c);

  //clip in y dimension (top)
  if (y<0) { //throw away rows which are off screen
    for (iy = 0; iy < -1L * y; iy += 1L){ //for each off screen row
      file->seek(w*2L, SeekCur);
      h -= 1; //reduce bitmap hight by 1 row
    }
    y = 0; //set y pos to 0 for the remaining portion of the bitmap
  }
  for (iy = y; iy < y + h; iy += 1){ //for each row
    toggle ^= true;
    if (iy < dest_buffer_height) //clip in y dimension (bottom) - simply don't draw anything
    {
      mx = 0; nx = 0;
      if (x < 0L){file->seek(file->position() + (x * -2L));mx = -1L * x;} //clip in x dimension (left) - skip offscreen data
      ifb = (iy * dest_buffer_width) + x + mx; //32bit index
      if (x + w > dest_buffer_width){nx = x + w - dest_buffer_width;}//clip in x dimension (right) - truncate copy to screen bounds
      for (uint16_t z = 0; z < (w - mx - nx); z += 1){
        file->readBytes((char*)&dw,2);
        toggle ^= true;
        //if alpha is enabled mask any colors close to black
        if (blt_mode == BLT_COPY){dest_buffer[ifb + z] = dw;}
        else if (blt_mode == BLT_BLK_COLOR_KEY && (dw & 0xE79C) != 0){dest_buffer[ifb + z] = dw;}
        else if (blt_mode == BLT_HATCH_BLK){
          if ((dw & 0xE79C) != 0) dest_buffer[ifb + z] = dw;
          else if (toggle) dest_buffer[ifb + z] = 0; //pFB[i] ^= pFB[i];
        }
        else if (blt_mode == BLT_HATCH_XOR){
          if ((dw & 0xE79C) != 0) dest_buffer[ifb + z] = dw;
          else if (toggle) dest_buffer[ifb + z] = dest_buffer[ifb + z]^dest_buffer[ifb + z];
        }else if (blt_mode == BLT_ADD){
          dest_buffer[ifb + z] += dw;
        }else if (blt_mode == BLT_SUB){
          dest_buffer[ifb + z] -= dw;
        }else if (blt_mode == BLT_MULT){
          dest_buffer[ifb + z] *= dw;
        }else if (blt_mode == BLT_DIV){
          if(dw > 0) dest_buffer[ifb + z] /= dw;
        }else if (blt_mode == BLT_AND){
          if(dw > 0) dest_buffer[ifb + z] = dw & dest_buffer[ifb + z];
        }else if (blt_mode == BLT_OR){
          if(dw > 0) dest_buffer[ifb + z] = dw | dest_buffer[ifb + z];
        }else if (blt_mode == BLT_XOR){
          if(dw > 0) dest_buffer[ifb + z] = dw ^ dest_buffer[ifb + z];
        }
      }
      if (x + w > dest_buffer_width){file->seek((x + w - dest_buffer_width) * 2, SeekCur);} //clip in x dimension (right) - skip unused data
    }
    else{
      //since y index is now off screen return
      return;
    }
  }
}


/**
 * @brief blt from the SD card to a memory buffer x,y destination
 * 
 * @param dest_buffer 
 * @param dest_buffer_width 
 * @param dest_buffer_height 
 * @param path 
 * @param filename 
 * @param x 
 * @param y 
 * @param blt_mode 
 */
void FASTRUN ILI9341_t3_ERIS::bltSDB(uint16_t *dest_buffer, uint16_t dest_buffer_width, uint16_t dest_buffer_height, const char *path, const char *filename,int16_t x,int16_t y,bltMode blt_mode){
  int16_t iy; // x & y index
  int16_t w;int16_t h; //width & height
  int16_t mx;        //left clip x offset
  int16_t nx;        //right clip x offset
  unsigned long ifb; //frame buffer index
  //uint16_t dwbf[SCREEN_WIDTH];//file read pixel row input buffer
  uint16_t dw;       //pixel data
  char str[16];      //char buffer
  char *c;           //char pointer
  bool toggle = false;
  //pSD->chdir();
  if (!p_SD->chdir(path)){ //change file path
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
        if (blt_mode == BLT_COPY){dest_buffer[ifb + z] = dw;}
        else if (blt_mode == BLT_BLK_COLOR_KEY && (dw & 0xE79C) != 0){dest_buffer[ifb + z] = dw;}
        else if (blt_mode == BLT_HATCH_BLK){
          if ((dw & 0xE79C) != 0) dest_buffer[ifb + z] = dw;
          else if (toggle) dest_buffer[ifb + z] = 0; //pFB[i] ^= pFB[i];
        }
        else if (blt_mode == BLT_HATCH_XOR){
          if ((dw & 0xE79C) != 0) dest_buffer[ifb + z] = dw;
          else if (toggle) dest_buffer[ifb + z] = dest_buffer[ifb + z]^dest_buffer[ifb + z];
        }else if (blt_mode == BLT_ADD){
          dest_buffer[ifb + z] += dw;
        }else if (blt_mode == BLT_SUB){
          dest_buffer[ifb + z] -= dw;
        }else if (blt_mode == BLT_MULT){
          dest_buffer[ifb + z] *= dw;
        }else if (blt_mode == BLT_DIV){
          if(dw > 0) dest_buffer[ifb + z] /= dw;
        }else if (blt_mode == BLT_AND){
          if(dw > 0) dest_buffer[ifb + z] = dw & dest_buffer[ifb + z];
        }else if (blt_mode == BLT_OR){
          if(dw > 0) dest_buffer[ifb + z] = dw | dest_buffer[ifb + z];
        }else if (blt_mode == BLT_XOR){
          if(dw > 0) dest_buffer[ifb + z] = dw ^ dest_buffer[ifb + z];
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
  p_SD->chdir(an->getPath());
  file.open(an->getFileName(), O_READ);
  if (file.available() == 0){ //file not found
    Serial.print(F("M ILI9341_t3_ERIS::bltSDFullScreen File Not Found:"));
    Serial.println(an->getFileName());
    return;
  }
  //for (unsigned long i = (SCREEN_WIDTH * 64) ; i < (SCREEN_WIDTH * SCREEN_HEIGHT); i += 32){
  uint32_t chunk_size = (_width * _height)/ANIMATION_CHUNKS_PER_FRAME;
  uint32_t i = an->chunk * chunk_size;
  file.seekSet(15 + (i*2)); //skip the header - header will always be 15 bytes for full screen wallpaper
  file.read(&_pfbtft[i],chunk_size*2);
  //Serial.println()
  file.close();
}

/**
 * @brief full screen block transfer with no clipping - used for full screen images matching the screen resolution
 * 
 * @param filename 
 */
void ILI9341_t3_ERIS::bltSDFullScreen(const char *filename){
  p_SD->chdir("/I/U/W");
  file.open(filename, O_READ);
  if (file.available() == 0){ //file not found
    Serial.print(F("M ILI9341_t3_ERIS::bltSDFullScreen File Not Found:"));
    Serial.println(filename);
    p_SD->ls();
    return;
  }
  file.seekSet(15); //skip the header - header will always be 15 bytes for full screen wallpaper
  //for (unsigned long i = (SCREEN_WIDTH * 64) ; i < (SCREEN_WIDTH * SCREEN_HEIGHT); i += 32){
  for (uint32_t i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT); i += 32){ //32,64
    file.read(&_pfbtft[i],64);
  }
  file.close();
}