#include "ILI9341_t3_ERIS.h"
#include "Eris.h"
#include <arm_math.h>

static volatile bool dmabusy;

void renderCompleteCB(){
    dmabusy=false;
}

bool _busy(){
    return dmabusy;
}

bool FASTRUN ILI9341_t3_ERIS::updateScreenAsyncFrom(ILI9341_t3_ERIS* draw,bool update_cont){
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
    analogWriteFrequency(backlight, 16000);
    analogWrite(backlight, 180);
}

void FLASHMEM ILI9341_t3_ERIS::begin(){
    ILI9341_t3n::begin(tft_write_speed,tft_read_speed);
    fillScreen(ILI9341_BLACK);
    setClipRect();
    setOrigin();
    setTextColor(CL(244, 183, 255));
    setTextSize(1);
    setRotation(1);
    println("Display Initialized.");      
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
  uint16_t *dest_buffer;
  uint16_t first_pixel;
  
  srcBuffer = source->getSurfaceBufferP();
  dest_buffer = dest->getSurfaceBufferP();
  //check for null
  if (!srcBuffer) return;
  if (!dest_buffer) return;
  //check for off surface blt attempt
  if ((dest_x > dest->getWidth()) || (0 > (dest->getWidth() + dest_x))) return;
  if ((dest_y > dest->getHeight()) || (0 > (dest->getHeight() + dest_y))) return;
  
  first_pixel = srcBuffer[0];//for first pixel mask

  if(0 && blt_mode == BLT_COPY){ //TODO WIP - PXP currently disabled
    //PXP TEST
    uint16_t dw,dh,first_pixel;
    dw = dest_x + from_width;
    if(dw>SCREEN_WIDTH) dw=SCREEN_WIDTH;
    dh = dest_y + from_height;
    if(dh>SCREEN_HEIGHT) dh=SCREEN_HEIGHT;

    PXP_input_buffer(srcBuffer, 2, source->getWidth(), source->getHeight());
    PXP_input_format(PXP_RGB565,0,0,0);
    PXP_input_position(dest_x, dest_y, dw, dh);
    PXP_input_background_color(0,0,0);

    PXP_overlay_buffer(srcBuffer, 2, source->getWidth()-1, source->getHeight()-1);
    PXP_overlay_format(PXP_RGB565,0,true,0,PXP_MERGEAS,false);
    PXP_overlay_color_key_low(0);
    PXP_overlay_color_key_high(20,20,20);
    PXP_overlay_position(dest_x, dest_y, dw, dh);

    PXP_output_buffer(dest_buffer, 2, dest->getWidth(), dest->getHeight());
    PXP_output_format();
    PXP_output_clip(SCREEN_WIDTH, SCREEN_HEIGHT);
    //PXP_flip_horizontally(true); //testing - for visual confirmation
    PXP_process();
    return;
  }else{
    //for each pixel by x,y coords...
    for (source_y=0; source_y < source->getHeight();source_y +=1){
      for (source_x=0; source_x < source->getWidth();source_x +=1){
        //translate the source coords to the destination coords and calculate the indexes
        d_x = dest_x + source_x; 
        d_y = dest_y + source_y;
        write_index = (d_y * dest->getWidth()) + d_x;
        read_index = (source_y * source->getWidth()) + source_x;

        if(d_x < dest->getWidth() && d_x >= 0 && d_y < dest->getHeight() && d_y >= 0){ //bounds check
          uint16_t dw;
          int16_t r,g,b;
          
          toggle ^= true;//toggle for hatch operations

          dw = srcBuffer[read_index];

          //
          //pixel operation section
          //
          if (pixel_op_enable){
            int16_t src_r,src_g,src_b;
            bool is_not_color_keyed;
             //#define CL(_r, _g, _b) ((((_r)&0xF8) << 8) | (((_g)&0xFC) << 3) | ((_b) >> 3))
            //unpack the color channels from the 565 RGB format
            src_r = 0xFF & (srcBuffer[read_index] >> 8); 
            if (src_r > 255) src_r = 255; //clip the max value for each channel
            src_g = 0xFF & (srcBuffer[read_index] >> 3);
            if (src_g > 255) src_g = 255;
            src_b = 0xFF & (srcBuffer[read_index] << 3);
            
            is_not_color_keyed = (blt_mode == BLT_BLK_COLOR_KEY && ((dw & 0xE79C) != 0)) && \
              ( (blt_mode == BLT_1ST_PIXEL_COLOR_KEY) && (dw != first_pixel) );
            
            switch(pixel_op){
              case PXOP_1ST_PIXEL_COLOR_KEY:
                if (first_pixel != dw) dw = pixel_op_param;
                break;
              case PXOP_BLK_COLOR_KEY:
                if (!is_not_color_keyed) dw = pixel_op_param;
                break;
              case PXOP_HATCH_BLK:
                if (toggle && is_not_color_keyed) dw = 0;
                break;
              case PXOP_HATCH_XOR:
                  if (toggle && is_not_color_keyed) dw = pixel_op_param^dw;
                break;
              case PXOP_COPY:
                dw = pixel_op_param; //note: same result as a fill function - could be used for benchmarking 
                break;
              case PXOP_ADD:
                if (is_not_color_keyed) dw = CL(src_r + pixel_op_param_r,src_g + pixel_op_param_g, src_b + pixel_op_param_b);
                break;
              case PXOP_AND:
                if (is_not_color_keyed) dw = CL(src_r & pixel_op_param_r,src_g & pixel_op_param_g, src_b & pixel_op_param_b);
                break;
              case PXOP_DIV:
                if (is_not_color_keyed) dw = CL(src_r / pixel_op_param_r,src_g / pixel_op_param_g, src_b / pixel_op_param_b);
                break;
              case PXOP_MEAN:
                if (is_not_color_keyed) dw = CL((src_r + pixel_op_param_r)/2,(src_g + pixel_op_param_g)/2, (src_b + pixel_op_param_b)/2);
                break;
              case PXOP_MULT:
                if (is_not_color_keyed) dw = CL(src_r * pixel_op_param_r,src_g * pixel_op_param_g, src_b * pixel_op_param_b);
                break;
              case PXOP_OR:
                if (is_not_color_keyed) dw = CL(src_r | pixel_op_param_r,src_g | pixel_op_param_g, src_b | pixel_op_param_b);
                break;
              case PXOP_SUB:
                if (is_not_color_keyed) dw = CL(src_r - pixel_op_param_r,src_g - pixel_op_param_g, src_b - pixel_op_param_b);
                break;
              case PXOP_XOR:
                if (is_not_color_keyed) dw = CL(src_r ^ pixel_op_param_r,src_g ^ pixel_op_param_g, src_b ^ pixel_op_param_b);
                break;
            };
          }

          //bit transfer operation
          if (blt_mode == BLT_COPY){
            dest_buffer[write_index] = dw;
          }else if (blt_mode == BLT_BLK_COLOR_KEY && ((dw & 0xE79C) != 0)){dest_buffer[write_index] = dw;
          }else if (blt_mode == BLT_HATCH_BLK){
            if ((dw & 0xE79C) != 0){ dest_buffer[write_index] = dw;
            } else if (toggle) dest_buffer[write_index] = 0; //pFB[i] ^= pFB[i];
          }else if (blt_mode == BLT_HATCH_XOR){
            if ((dw & 0xE79C) != 0) dest_buffer[write_index] = dw^dest_buffer[write_index];
            else if (toggle) dest_buffer[write_index] = dest_buffer[write_index];
          }else if (blt_mode == BLT_ADD){
            r = (dest_buffer[write_index] >> 8) + (0xFF & (dw >> 8));
            g = (dest_buffer[write_index] >> 3) + (0xFF & (dw >> 3));
            b = (dest_buffer[write_index] << 3) + (0xFF & (dw << 3));
            if(r>0xFF) r = 0xFF; //clip the result to 8bit
            if(g>0xFF) g = 0xFF;
            if(b>0xFF) b = 0xFF;
            dest_buffer[write_index] = CL(r,g,b); //convert back to 565 format and write the result
          }else if (blt_mode == BLT_SUB){
            r = (dest_buffer[write_index] >> 8) - (0xFF & (dw >> 8));
            g = (dest_buffer[write_index] >> 3) - (0xFF & (dw >> 3));
            b = (dest_buffer[write_index] << 3) - (0xFF & (dw << 3));
            if (r < 0) r = 0; //clamp the result to zero
            if (g < 0) g = 0;
            if (b < 0) b = 0;
            dest_buffer[write_index] = CL(r,g,b);
          }else if (blt_mode == BLT_MULT){
            r = (dest_buffer[write_index] >> 8) * (0xFF & (dw >> 8));
            g = (dest_buffer[write_index] >> 3) * (0xFF & (dw >> 3));
            b = (dest_buffer[write_index] << 3) * (0xFF & (dw << 3));
            if (r > 255) r = 255; //clip the max value for each channel
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            dest_buffer[write_index] = CL(r,g,b);
          }else if (blt_mode == BLT_DIV){
            r = (dest_buffer[write_index] >> 8) / (0xFF & (dw >> 8));
            g = (dest_buffer[write_index] >> 3) / (0xFF & (dw >> 3));
            b = (dest_buffer[write_index] << 3) / (0xFF & (dw << 3));
            if (r > 255) r = 255; //clip the max value for each channel
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            dest_buffer[write_index] = CL(r,g,b);
          }else if (blt_mode == BLT_AND){
            r = (dest_buffer[write_index] >> 8) & (0xFF & (dw >> 8));
            g = (dest_buffer[write_index] >> 3) & (0xFF & (dw >> 3));
            b = (dest_buffer[write_index] << 3) & (0xFF & (dw << 3));
            dest_buffer[write_index] = CL(r,g,b);
          }else if (blt_mode == BLT_OR){
            r = (dest_buffer[write_index] >> 8) | (0xFF & (dw >> 8));
            g = (dest_buffer[write_index] >> 3) | (0xFF & (dw >> 3));
            b = (dest_buffer[write_index] << 3) | (0xFF & (dw << 3));
            dest_buffer[write_index] = CL(r,g,b);
          }else if (blt_mode == BLT_XOR){
            r = (dest_buffer[write_index] >> 8) ^ (0xFF & (dw >> 8));
            g = (dest_buffer[write_index] >> 3) ^ (0xFF & (dw >> 3));
            b = (dest_buffer[write_index] << 3) ^ (0xFF & (dw << 3));
            dest_buffer[write_index] = CL(r,g,b);
          }else if (blt_mode == BLT_MEAN){
            q7_t res[3];
            q7_t a[2];
            a[0] = (0x7F & (dest_buffer[write_index] >> 8));
            a[1] = (0x7F & (dw >> 8));
            arm_mean_q7(a,2,&res[0]);
            a[0] = (0x7F & (dest_buffer[write_index] >> 3));
            a[1] = (0x7F & (dw >> 3));
            arm_mean_q7(a,2,&res[1]);
            a[0] = (0x7F & (dest_buffer[write_index] << 3));
            a[1] = (0x7F & (dw << 3));
            arm_mean_q7(a,2,&res[2]);
            dest_buffer[write_index] = CL(res[0],res[1],res[2]);
          }else if (blt_mode == BLT_1ST_PIXEL_COLOR_KEY){
            if (dw != first_pixel) dest_buffer[write_index] = dw;
          }
        }
      }
    }
  }
}

void FASTRUN ILI9341_t3_ERIS::fillSurface(Surface *dest,uint16_t color,bltMode blt_mode){
  int16_t dest_x,dest_y;
  uint16_t *dest_buffer;
  bool toggle, is_not_color_keyed;
  uint16_t dw, first_pixel,w;
  
  toggle = false;
  dest_buffer = dest->getSurfaceBufferP();
  first_pixel = dest_buffer[0];

  int16_t r,g,b;
  //#define CL(_r, _g, _b) ((((_r)&0xF8) << 8) | (((_g)&0xFC) << 3) | ((_b) >> 3))
  //unpack the color channels from the 565 RGB format
  r = 0xFF & (color >> 8); 
  if (r > 255) r = 255; //clip the max value for each channel
  g = 0xFF & (color >> 3);
  if (g > 255) g = 255;
  b = 0xFF & (color << 3);
  dw = color;

  if ( (blt_mode == BLT_BLK_COLOR_KEY) && ((dw & 0xE79C) == 0) ) is_not_color_keyed = false;
      else is_not_color_keyed = true;
  w =  dest->getWidth();
  for (dest_y=0; dest_y < dest->getHeight();dest_y +=1){
    for (dest_x=0; dest_x < dest->getWidth();dest_x +=1){
      uint32_t write_index;

      toggle ^= toggle;
      write_index = (dest_y * w) + dest_x;
  
      if (blt_mode == BLT_COPY){
        dest_buffer[write_index] = dw;
      }else if (blt_mode == BLT_BLK_COLOR_KEY && ((dw & 0xE79C) != 0)){dest_buffer[write_index] = dw;
      }else if (blt_mode == BLT_HATCH_BLK){
        if ((dw & 0xE79C) != 0){ dest_buffer[write_index] = dw;
        } else if (toggle) dest_buffer[write_index] = 0; //pFB[i] ^= pFB[i];
      }else if (blt_mode == BLT_HATCH_XOR){
        if ((dw & 0xE79C) != 0) dest_buffer[write_index] = dw^dest_buffer[write_index];
        else if (toggle) dest_buffer[write_index] = dest_buffer[write_index];
      }else if (blt_mode == BLT_ADD){
        r = (dest_buffer[write_index] >> 8) + (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) + (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) + (0xFF & (dw << 3));
        if(r>0xFF) r = 0xFF; //clip the result to 8bit
        if(g>0xFF) g = 0xFF;
        if(b>0xFF) b = 0xFF;
        dest_buffer[write_index] = CL(r,g,b); //convert back to 565 format and write the result
      }else if (blt_mode == BLT_SUB){
        r = (dest_buffer[write_index] >> 8) - (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) - (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) - (0xFF & (dw << 3));
        if (r < 0) r = 0; //clamp the result to zero
        if (g < 0) g = 0;
        if (b < 0) b = 0;
        dest_buffer[write_index] = CL(r,g,b);
      }else if (blt_mode == BLT_MULT){
        r = (dest_buffer[write_index] >> 8) * (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) * (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) * (0xFF & (dw << 3));
        if (r > 255) r = 255; //clip the max value for each channel
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        dest_buffer[write_index] = CL(r,g,b);
      }else if (blt_mode == BLT_DIV){
        r = (dest_buffer[write_index] >> 8) / (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) / (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) / (0xFF & (dw << 3));
        if (r > 255) r = 255; //clip the max value for each channel
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        dest_buffer[write_index] = CL(r,g,b);
      }else if (blt_mode == BLT_AND){
        r = (dest_buffer[write_index] >> 8) & (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) & (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) & (0xFF & (dw << 3));
        dest_buffer[write_index] = CL(r,g,b);
      }else if (blt_mode == BLT_OR){
        r = (dest_buffer[write_index] >> 8) | (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) | (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) | (0xFF & (dw << 3));
        dest_buffer[write_index] = CL(r,g,b);
      }else if (blt_mode == BLT_XOR){
        r = (dest_buffer[write_index] >> 8) ^ (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) ^ (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) ^ (0xFF & (dw << 3));
        dest_buffer[write_index] = CL(r,g,b);
      }else if (blt_mode == BLT_MEAN){
        q7_t res[3];
        q7_t a[2];
        a[0] = (0x7F & (dest_buffer[write_index] >> 8));
        a[1] = (0x7F & (dw >> 8));
        arm_mean_q7(a,2,&res[0]);
        a[0] = (0x7F & (dest_buffer[write_index] >> 3));
        a[1] = (0x7F & (dw >> 3));
        arm_mean_q7(a,2,&res[1]);
        a[0] = (0x7F & (dest_buffer[write_index] << 3));
        a[1] = (0x7F & (dw << 3));
        arm_mean_q7(a,2,&res[2]);
        dest_buffer[write_index] = CL(res[0],res[1],res[2]);
      }else if (blt_mode == BLT_1ST_PIXEL_COLOR_KEY){
        if (dw != first_pixel) dest_buffer[write_index] = dw;
      }else{
        dest_buffer[write_index] = dw;
      }      
    }
  }
};

void FASTRUN ILI9341_t3_ERIS::drawPixel(Surface *dest,int16_t x, int16_t y, uint16_t color){
  //uint16_t *dstBuffer;
  //dstBuffer = dest->getSurfaceBufferP();
  //dstBuffer[(y * dest->getWidth()) + x] = color;
  ((uint16_t *)(dest->getSurfaceBufferP()))[(y * dest->getWidth()) + x] = color;
}

uint16_t FASTRUN ILI9341_t3_ERIS::readSurfacePixel(Surface *source,int16_t x, int16_t y){
  //uint16_t *srcBuffer;
  //srcBuffer = source->getSurfaceBufferP();
  //return srcBuffer[(y * source->getWidth()) + x];
  return ((uint16_t*)source->getSurfaceBufferP())[(y * source->getWidth()) + x];
}

uint16_t FASTRUN ILI9341_t3_ERIS::readPixel(int16_t x, int16_t y){ 
  return _pfbtft[(y * _width) + x];
}


void FASTRUN ILI9341_t3_ERIS::drawSurfaceLine(Surface *dest,int16_t start_x, int16_t start_y,int16_t end_x, int16_t end_y, uint16_t color){
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

void FASTRUN ILI9341_t3_ERIS::drawSurfaceFill(Surface *dest,uint16_t color){
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
 * @param x 
 * @param y 
 * @param blt_mode 
 */
void FASTRUN ILI9341_t3_ERIS::bltSD(const char *path, const char *filename,int16_t x,int16_t y,bltMode blt_mode){
  bltSDB(_pfbtft, _width, _height, path, filename,x,y,blt_mode); //blt from sd to buffer using the frame buffer as the destination
  return;
}

/**
 * @brief area block transfer from the SD card (default) or from a TinyFS file system to a memory buffer
 * 
 * @param dest_buffer buffer pointer
 * @param dest_x dest start transfer coord
 * @param dest_y dest start transfer coord 
 * @param dest_buffer_width this should be the width of the destination buffer
 * @param dest_buffer_height this should be the height of the destination buffer
 * @param file_name 
 * @param src_x src start transfer coord
 * @param src_y src start transfer coord
 * @param src_width this is the width to be transfered
 * @param src_height this is the height to be transfered
 * @param blt_mode  transfer operation
 * @param file_system this is the optional pointer to a TinyFS object
 */
void FLASHMEM ILI9341_t3_ERIS::bltArea2Buffer(uint16_t *dest_buffer, int16_t dest_x,int16_t dest_y, uint16_t dest_buffer_width,uint16_t dest_buffer_height, const char *file_name,int16_t src_x,int16_t src_y, uint16_t src_width, uint16_t src_height,bltMode blt_mode,bool strip_color_key_rows,LittleFS_RAM* file_system){
  int16_t iy; // x & y index
  int16_t w;int16_t h; //width & height
  int16_t mx;        //left clip x offset
  int16_t nx;        //right clip x offset
  unsigned long ifb; //frame buffer index
  //uint16_t dwbf[SCREEN_WIDTH];//file read pixel row input buffer
  uint16_t dw;       //pixel data
  uint16_t first_pixel;
  char str[16];      //char buffer
  char *c;           //char pointer
  bool toggle = false;

  File tinyFile;
  if(file_system == NULL){
    file.open(file_name, O_READ);//open image to read
  } else tinyFile = file_system->open(file_name, O_READ);
  
  if ((file_system == NULL && file.available() == 0) ||(file_system != NULL &&  tinyFile.available() == 0)){ //file not found
    print(F("M ILI9341_t3_ERIS::bltArea2Buffer File Not Found: "));
    println(file_name);
    return;
  }

  if(file_system == NULL){
    file.fgets(str,sizeof(str)); //read the header data
    file.fgets(str,sizeof(str)); //to get the image dimensions
  } else{
    tinyFile.readBytesUntil('\n',&str[0],16);//read the file type (should be ILE565)
    tinyFile.readBytesUntil('\n',&str[0],16); //to get the image dimensions
  }
  strtok(str," ");             //convert dimension text to numbers
  w = atol(str);  //image width
  c = strtok(NULL, " ");
  h = atol(c);    //image height

  //get first pixel value in case of color key by pixel
  if(file_system == NULL){
    file.readBytes((char*)&first_pixel,2);
    file.seek(file.position()-2L); //rewind the read pointer
  }else{
    tinyFile.readBytes((char*)&first_pixel,2);
    tinyFile.seek(tinyFile.position()-2L); //rewind the read pointer
  }

  //seek to the source row
  if (src_y > 0) { 
    for (iy = 0; iy < src_y; iy += 1L){ //for each off screen row
      
      if(file_system == NULL){
        file.seekCur(w*2L);
      } else tinyFile.seek(w*2L,SeekCur);

      h -= 1; //reduce bitmap hight by 1 row
    }
    src_y = 0;
  }

  //strip empty rows (black unless first pixel)
  if(strip_color_key_rows){
    for (iy = dest_y; iy < (dest_y + src_height); iy += 1){
      ifb = (iy * dest_buffer_width) + dest_x; //32bit index
      if(file_system == NULL){
        file.seekCur(src_x * 2L); //seek to start_x
      } else {
        tinyFile.seek(src_x * 2L,SeekCur);
      }
      //read the data for the row
      bool found_nonkeyed_pixel;
      found_nonkeyed_pixel = false;
      for (uint16_t z = 0; z < src_width; z += 1){
        if (file_system == NULL){
          file.read(&dw,2);
        }else tinyFile.read(&dw,2);
        if ((dw & 0xE79C) != 0) found_nonkeyed_pixel = true;
      }
      if (found_nonkeyed_pixel){
        //rewind to the begining of the row and continue to render sections
        if(file_system == NULL){
          file.seekCur((src_width + src_x) * -2L); 
        } else {
          tinyFile.seek(tinyFile.position() - ((src_width + src_x) * 2L),SeekSet);
        } 
        break;
      }else{
        h -= 1; //reduce bitmap hight by 1 row
        dest_y += 1;
        //seek to the end of the row and continue with the next
        if(file_system == NULL){
          file.seekCur(2L* (w - (src_width + src_x))); //skip to the start of the next row
        }else{
          tinyFile.seek(2L* (w - (src_width + src_x)),SeekCur); //skip to the start of the next row
        }
      }
    }
  }
  
  //clip in y dimension (top)
  if (dest_y < 0) { //throw away rows which are off screen
    for (iy = 0; iy < (-1L * dest_y); iy += 1L){ //for each off screen row

      if(file_system == NULL){
        file.seekCur(w*2L);
      } else tinyFile.seek(w*2L,SeekCur);

      h -= 1; //reduce bitmap hight by 1 row
    }
    dest_y = 0; //set y pos to 0 for the remaining portion of the bitmap
  }

  //for each remaining row
  bool pixel_detector;
  int16_t src_height_remaining;

  pixel_detector = false;
  src_height_remaining = h;
  for (iy = dest_y; iy < (dest_y + src_height); iy += 1){ 
    if(pixel_detector==false && ((h - src_height_remaining) > (h*0.4))){
      break;
    }else{
      pixel_detector = false;//reset the detector
    }

    toggle ^= true;
    if (iy >= dest_buffer_height || src_height_remaining == 0) break; //clip in y dimension (bottom) - simply don't do anything
    mx = 0; nx = 0;
    //if (dest_x < 0L){file.seekCur(dest_x * -2L);mx += -1L * dest_x;} //clip in x dimension (left) - skip offscreen data
    ifb = (iy * dest_buffer_width) + dest_x + mx; //32bit index
    //if ((dest_x + src_width) > w){nx = (dest_x + src_width) - dest_buffer_width;}//clip in x dimension (right) - truncate copy to screen bounds
    //inital x offset
    if(file_system == NULL){
      file.seekCur(src_x * 2L); //seek to start_x
    } else tinyFile.seek(src_x * 2L,SeekCur);

    //read the data for the row
    for (uint16_t z = 0; z < src_width; z += 1){
      if (file_system == NULL){
        file.read(&dw,2);
      }else{
        tinyFile.read(&dw,2);
      }
      //if alpha is enabled mask any colors close to black
      int16_t r,g,b;
      bool is_not_color_keyed;  
      toggle ^= true;//toggle for hatch operations

      if ((dw & 0xE79C) != 0) pixel_detector=true;

      //
      //pixel operation section
      //
      if (pixel_op_enable){
        int16_t src_r,src_g,src_b;
        //#define CL(_r, _g, _b) ((((_r)&0xF8) << 8) | (((_g)&0xFC) << 3) | ((_b) >> 3))
        //unpack the color channels from the 565 RGB format
        src_r = 0xFF & (dw >> 8); 
        if (src_r > 255) src_r = 255; //clip the max value for each channel
        src_g = 0xFF & (dw >> 3);
        if (src_g > 255) src_g = 255;
        src_b = 0xFF & (dw << 3);
        
        if ( (blt_mode == BLT_BLK_COLOR_KEY) && ((dw & 0xE79C) == 0) ) is_not_color_keyed = false;
        else if ( (blt_mode == BLT_1ST_PIXEL_COLOR_KEY) && (dw == first_pixel) ) is_not_color_keyed = false;
        else is_not_color_keyed = true;
        
        switch(pixel_op){
          case PXOP_1ST_PIXEL_COLOR_KEY:
            if (is_not_color_keyed) dw = pixel_op_param;
            break;
          case PXOP_BLK_COLOR_KEY:
            if (is_not_color_keyed) dw = pixel_op_param;
            break;
          case PXOP_HATCH_BLK:
            if (toggle && is_not_color_keyed) dw = 0;
            break;
          case PXOP_HATCH_XOR:
              if (toggle && is_not_color_keyed) dw = pixel_op_param^dw;
            break;
          case PXOP_COPY:
            if (is_not_color_keyed) dw = pixel_op_param; //note: same result as a fill function - could be used for benchmarking 
            break;
          case PXOP_ADD:
            if (is_not_color_keyed) dw = CL(src_r + pixel_op_param_r,src_g + pixel_op_param_g, src_b + pixel_op_param_b);
            break;
          case PXOP_AND:
            if (is_not_color_keyed) dw = CL(src_r & pixel_op_param_r,src_g & pixel_op_param_g, src_b & pixel_op_param_b);
            break;
          case PXOP_DIV:
            if (is_not_color_keyed) dw = CL(src_r / pixel_op_param_r,src_g / pixel_op_param_g, src_b / pixel_op_param_b);
            break;
          case PXOP_MEAN:
            if (is_not_color_keyed) dw = CL((src_r + pixel_op_param_r)/2,(src_g + pixel_op_param_g)/2, (src_b + pixel_op_param_b)/2);
            break;
          case PXOP_MULT:
            if (is_not_color_keyed) dw = CL(src_r * pixel_op_param_r,src_g * pixel_op_param_g, src_b * pixel_op_param_b);
            break;
          case PXOP_OR:
            if (is_not_color_keyed) dw = CL(src_r | pixel_op_param_r,src_g | pixel_op_param_g, src_b | pixel_op_param_b);
            break;
          case PXOP_SUB:
            if (is_not_color_keyed) dw = CL(src_r - pixel_op_param_r,src_g - pixel_op_param_g, src_b - pixel_op_param_b);
            break;
          case PXOP_XOR:
            if (is_not_color_keyed) dw = CL(src_r ^ pixel_op_param_r,src_g ^ pixel_op_param_g, src_b ^ pixel_op_param_b);
            break;
          case PXOP_NOT_1ST_PIXEL_COLOR_KEY:
            if (!is_not_color_keyed) dw = pixel_op_param;
            break;
          case PXOP_NOT_BLK_COLOR_KEY:
            if (!is_not_color_keyed) dw = pixel_op_param;
            break;
          //default:
            //dw = dw;//unknown op - just read the pixel
        };
      }//else dw = dw; //pixel op disabled so simply read the source pixel
      
      //
      //bit transfer operation
      //
      uint32_t write_index;
      write_index = ifb + z;
      if (blt_mode == BLT_COPY){
        dest_buffer[write_index] = dw;
      }else if (blt_mode == BLT_BLK_COLOR_KEY && ((dw & 0xE79C) != 0)){dest_buffer[write_index] = dw;
      }else if (blt_mode == BLT_HATCH_BLK){
        if ((dw & 0xE79C) != 0){ dest_buffer[write_index] = dw;
        } else if (toggle) dest_buffer[write_index] = 0; //pFB[i] ^= pFB[i];
      }else if (blt_mode == BLT_HATCH_XOR){
        if ((dw & 0xE79C) != 0) dest_buffer[write_index] = dw^dest_buffer[write_index];
        else if (toggle) dest_buffer[write_index] = dest_buffer[write_index];
      }else if (blt_mode == BLT_ADD){
        r = (dest_buffer[write_index] >> 8) + (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) + (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) + (0xFF & (dw << 3));
        if(r>0xFF) r = 0xFF; //clip the result to 8bit
        if(g>0xFF) g = 0xFF;
        if(b>0xFF) b = 0xFF;
        dest_buffer[write_index] = CL(r,g,b); //convert back to 565 format and write the result
      }else if (blt_mode == BLT_SUB){
        r = (dest_buffer[write_index] >> 8) - (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) - (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) - (0xFF & (dw << 3));
        if (r < 0) r = 0; //clamp the result to zero
        if (g < 0) g = 0;
        if (b < 0) b = 0;
        dest_buffer[write_index] = CL(r,g,b);
      }else if (blt_mode == BLT_MULT){
        r = (dest_buffer[write_index] >> 8) * (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) * (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) * (0xFF & (dw << 3));
        if (r > 255) r = 255; //clip the max value for each channel
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        dest_buffer[write_index] = CL(r,g,b);
      }else if (blt_mode == BLT_DIV){
        r = (dest_buffer[write_index] >> 8) / (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) / (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) / (0xFF & (dw << 3));
        if (r > 255) r = 255; //clip the max value for each channel
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        dest_buffer[write_index] = CL(r,g,b);
      }else if (blt_mode == BLT_AND){
        r = (dest_buffer[write_index] >> 8) & (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) & (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) & (0xFF & (dw << 3));
        dest_buffer[write_index] = CL(r,g,b);
      }else if (blt_mode == BLT_OR){
        r = (dest_buffer[write_index] >> 8) | (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) | (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) | (0xFF & (dw << 3));
        dest_buffer[write_index] = CL(r,g,b);
      }else if (blt_mode == BLT_XOR){
        r = (dest_buffer[write_index] >> 8) ^ (0xFF & (dw >> 8));
        g = (dest_buffer[write_index] >> 3) ^ (0xFF & (dw >> 3));
        b = (dest_buffer[write_index] << 3) ^ (0xFF & (dw << 3));
        dest_buffer[write_index] = CL(r,g,b);
      }else if (blt_mode == BLT_MEAN){
        q7_t res[3];
        q7_t a[2];
        a[0] = (0x7F & (dest_buffer[write_index] >> 8));
        a[1] = (0x7F & (dw >> 8));
        arm_mean_q7(a,2,&res[0]);
        a[0] = (0x7F & (dest_buffer[write_index] >> 3));
        a[1] = (0x7F & (dw >> 3));
        arm_mean_q7(a,2,&res[1]);
        a[0] = (0x7F & (dest_buffer[write_index] << 3));
        a[1] = (0x7F & (dw << 3));
        arm_mean_q7(a,2,&res[2]);
        dest_buffer[write_index] = CL(res[0],res[1],res[2]);
      }else if (blt_mode == BLT_1ST_PIXEL_COLOR_KEY){
        if (dw != first_pixel) dest_buffer[write_index] = dw;
      }else{
        dest_buffer[write_index] = dw;
      }
    //if ( (dest_x + src_width) < w){file.seekCur( ((w - (dest_x + src_width)) * 2));} //clip in x dimension (right) - skip unused data
    }
  
    if(file_system == NULL){
      file.seekCur(2L* (w - (src_width + src_x))); //skip to the start of the next row
    }else{
      tinyFile.seek(2L* (w - (src_width + src_x)),SeekCur); //skip to the start of the next row
    }
    toggle ^= true;
    src_height_remaining -= 1; //reduce bitmap hight by 1 row
  }
  
  
  if(file_system == NULL){
    file.close();
  }else{
    tinyFile.close();
  }
}

bool FLASHMEM ILI9341_t3_ERIS::getImageSize(const char* path,const char* filename,int32_t* width,int32_t* height, LittleFS_RAM* file_system){
  char str[16];      //char buffer
  char *c;           //char pointer
  char file_name[64];
  File tinyFile;

  //build the path + filename string
  strcpy(file_name,path);
  strcat(file_name,filename);

  if(file_system == NULL){
    file.open(file_name, O_READ);//open image to read
  } else tinyFile = file_system->open(file_name, O_READ);
  
  if ((file_system == NULL && file.available() == 0) ||(file_system != NULL &&  tinyFile.available() == 0)){ //file not found
    Serial.print(F("M ILI9341_t3_ERIS::getImageSize File Not Found: "));
    Serial.println(file_name);
    return false;
  }

  if(file_system == NULL){
    file.fgets(str,sizeof(str)); //read the header data
    file.fgets(str,sizeof(str)); //to get the image dimensions
    file.close();
  } else{
    tinyFile.readBytesUntil('\n',&str[0],16);//read the file type (should be ILE565)
    tinyFile.readBytesUntil('\n',&str[0],16); //to get the image dimensions
    tinyFile.close();
  }
  strtok(str," ");             //convert dimension text to numbers
  *width = atol(str);  //image width
  c = strtok(NULL, " ");
  *height = atol(c);    //image height
  return true;
}


void FLASHMEM ILI9341_t3_ERIS::printWithFont(const char* string_buffer,uint16_t x,uint16_t y,const char* font,uint16_t pt,LittleFS_RAM* file_system){
  uint64_t position;
  const String end_of_index = "KEARNING\n"; //yeah, its spelled wrong in the file format
  float fx;
  int32_t width,height;
  int16_t j;
  int16_t count;
  int16_t startx[128];
  int16_t stopx[128];
  int16_t kern_value;
  char wildcard[2];
  char terminator = '\n';
  char font_file_path[64];
  char font_file_path_only[64];
  char font_file_name[64];
  char font_kerning_file_path[64];
  char tmp[8];//tmp buffer to build the file name
  char character[128];//storage for the loaded start stop x indexes from the kerning file
  String line;
  FsFile kern;
  File lfs_kern;
  
  //make sure the font exists
  if(file_system == NULL){
    strcpy(font_file_path,"/I/U/F/");
    strcat(font_file_path,font);
    strcat(font_file_path,"/");
    if (!p_SD->exists(font_file_path)){
      setCursor(0,10);
      print("FONT PATH NOT FOUND! "); 
      return;
    }
  }else{
    strcpy(font_file_path,"/");
    strcat(font_file_path,font);
    strcat(font_file_path,"/");
    if (!file_system->exists(font_file_path)){
      setCursor(0,10);
      print("FONT PATH NOT FOUND!\n");
      print(font_file_path);
      return;
    } 
  }
  strcpy(font_file_path_only,font_file_path);//capture the font file path only; used later to get the image size
  sprintf(tmp,"%2hu",pt);//build the kerning and font filenames; file name is just the pt number to two digits
  strcpy(font_file_name,tmp);//build the filename used to get the image size later
  strcat(font_file_name,".ile");
  strcat(font_file_path,tmp);//build the full paths with the filename included for both the kerning and image file
  strcpy(font_kerning_file_path,font_file_path);//copy
  strcat(font_file_path,".ile");//add the file name sufixes
  strcat(font_kerning_file_path,".krn");
  setTextSize(pt);
  //open the kerning file
  if(file_system == NULL){
    kern = p_SD->open(font_kerning_file_path,O_RDONLY); 
    if (!kern) return;
  }else{
    lfs_kern = file_system->open(font_kerning_file_path,O_RDONLY);
    if (!lfs_kern){
      setCursor(0,10); 
      println("kerning file not found.");
      print(font_kerning_file_path);
      return;
    }
  }
  //load the x start and stop indexes one time
  if(file_system == NULL){
    line = kern.readStringUntil(terminator,64);//throw away the header line
    line = kern.readStringUntil(terminator,64); //get the first record
  }else{
    line = lfs_kern.readStringUntil(terminator,64);//throw away the header line
    line = lfs_kern.readStringUntil(terminator,64); //get the first record
  }
  j = 0;
  
  do{
    count = sscanf(line.c_str(),"%c %hu %hu\n",&character[j],&startx[j],&stopx[j]); //load the coords
    j+=1;
    if(file_system == NULL){
      line = kern.readStringUntil(terminator,256);
    }else{
      line = lfs_kern.readStringUntil(terminator,256);
    }
  }while(count == 3);//j < 102 end of index reached
  //throw away the next line and save the file positions
  if(file_system == NULL){
    line = kern.readStringUntil(terminator,256);
    position = kern.position(); //save the position
  }else{
    line = lfs_kern.readStringUntil(terminator,256);
    position = lfs_kern.position(); //save the position
  }
  if(file_system == NULL){
    kern.close();
  }else{
    lfs_kern.close();
  }
  
  //get the font image size
  if(file_system == NULL){
    getImageSize(font_file_path_only,font_file_name, &width, &height);
  }else{
    getImageSize(font_file_path_only,font_file_name, &width, &height,file_system);
  }
  //now walk the string buffer and 'print' 
  strcpy(wildcard,"*");
  fx = x;
  char last_c;
  last_c = ' ';
  for(int16_t i = 0; i < (int16_t)strlen(string_buffer);i++){
    char string_c;
    string_c = string_buffer[i]; //for each char
    j = -1;
    do{
      //search the character to find the index for loading the start/stop x values
      j++;
      if (j>102) break;
    }while(string_c!=character[j]);
    //check stop_x it should be greater than zero.. if not the char was not found
    //else blt the font char
    if(j < 102 && string_c != ' '){ 
      //fx -= pt/6.0;
      if(file_system == NULL){ 
        bltArea2Buffer(_pfbtft,fx,y,SCREEN_WIDTH,SCREEN_HEIGHT,font_file_path,startx[j],0,stopx[j]-startx[j],height * 0.40,BLT_1ST_PIXEL_COLOR_KEY,true,NULL);
      }else{
        bltArea2Buffer(_pfbtft,fx,y,SCREEN_WIDTH,SCREEN_HEIGHT,font_file_path,startx[j],0,stopx[j]-startx[j],height * 0.40,BLT_1ST_PIXEL_COLOR_KEY,true,file_system);
      }
      fx += (stopx[j]-startx[j])+1;
    } else{
      fx += pt/2.0;
    }
    last_c = string_buffer[i];
  }
}

void FLASHMEM ILI9341_t3_ERIS::bltRAMFileB(uint16_t *dest_buffer, uint16_t dest_buffer_width, uint16_t dest_buffer_height, File* file,int16_t x,int16_t y,bltMode blt_mode){
  int16_t iy; // x & y index
  int16_t mx;        //left clip x offset
  int16_t nx;        //right clip x offset
  unsigned long ifb; //frame buffer index
  uint16_t dw;       //pixel data
  uint16_t first_pixel;
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

  //get first pixel value in case of color key by pixel
  file->readBytes((char*)&first_pixel,2);
  file->seek(file->position()-2L, SeekSet); //rewind the read pointer

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
      int16_t r,g,b;
      mx = 0; nx = 0;
      if (x < 0L){file->seek(file->position() + (x * -2L));mx = -1L * x;} //clip in x dimension (left) - skip offscreen data
      ifb = (iy * dest_buffer_width) + x + mx; //32bit index
      if (x + w > dest_buffer_width){nx = x + w - dest_buffer_width;}//clip in x dimension (right) - truncate copy to screen bounds
      for (uint16_t z = 0; z < (w - mx - nx); z += 1){
        file->readBytes((char*)&dw,2);
        toggle ^= true;
        //if alpha is enabled mask any colors close to black 
        if (blt_mode == BLT_COPY){dest_buffer[ifb + z] = dw;}
        else if (blt_mode == BLT_BLK_COLOR_KEY && ((dw & 0xE79C) != 0)){dest_buffer[ifb + z] = dw;}
        else if (blt_mode == BLT_HATCH_BLK){
          if ((dw & 0xE79C) != 0) dest_buffer[ifb + z] = dw;
          else if (toggle) dest_buffer[ifb + z] = 0; //pFB[i] ^= pFB[i];
        }
        else if (blt_mode == BLT_HATCH_XOR){
          if ((dw & 0xE79C) != 0) dest_buffer[ifb + z] = dw^dest_buffer[ifb + z];
          else if (toggle) dest_buffer[ifb + z] = dest_buffer[ifb + z];
        }else if (blt_mode == BLT_ADD){
          //#define CL(_r, _g, _b) ((((_r)&0xF8) << 8) | (((_g)&0xFC) << 3) | ((_b) >> 3))
          //unpack the color channels from the 565 RGB format
          r = (0xFF & (dest_buffer[ifb + z] >> 8)) + (0xFF & (dw >> 8));
          if (r > 255) r = 255; //clip the max value for each channel
          g = (0xFF & (dest_buffer[ifb + z] >> 3)) + (0xFF & (dw >> 3));
          if (g > 255) g = 255;
          b = (0xFF & (dest_buffer[ifb + z] << 3)) + (0xFF & (dw << 3));
          if (b > 255) b = 255;
          dest_buffer[ifb + z] = CL(r,g,b);
          //dest_buffer[ifb + z] += dw;
        }else if (blt_mode == BLT_SUB){
          r = (0xFF & (dest_buffer[ifb + z] >> 8)) - (0xFF & (dw >> 8));
          if (r < 0) r = 0; //clip the max value for each channel
          g = (0xFF & (dest_buffer[ifb + z] >> 3)) - (0xFF & (dw >> 3));
          if (g < 0) g = 0;
          b = (0xFF & (dest_buffer[ifb + z] << 3)) - (0xFF & (dw << 3));
          if (b < 0) b = 0;
          dest_buffer[ifb + z] = CL(r,g,b);
          //dest_buffer[ifb + z] -= dw;
        }else if (blt_mode == BLT_MULT){
          r = (0xFF & (dest_buffer[ifb + z] >> 8)) * (0xFF & (dw >> 8));
          if (r > 255) r = 255; //clip the max value for each channel
          g = (0xFF & (dest_buffer[ifb + z] >> 3)) * (0xFF & (dw >> 3));
          if (g > 255) g = 255;
          b = (0xFF & (dest_buffer[ifb + z] << 3)) * (0xFF & (dw << 3));
          if (b > 255) b = 255;
          dest_buffer[ifb + z] = CL(r,g,b);
          //dest_buffer[ifb + z] *= dw;
        }else if (blt_mode == BLT_DIV){
          r = (0xFF & (dest_buffer[ifb + z] >> 8)) / (0xFF & (dw >> 8));
          if (r > 255) r = 255; //clip the max value for each channel
          g = (0xFF & (dest_buffer[ifb + z] >> 3)) / (0xFF & (dw >> 3));
          if (g > 255) g = 255;
          b = (0xFF & (dest_buffer[ifb + z] << 3)) / (0xFF & (dw << 3));
          if (b > 255) b = 255;
          dest_buffer[ifb + z] = CL(r,g,b);
          //if(dw > 0) dest_buffer[ifb + z] /= dw;
        }else if (blt_mode == BLT_AND){
          if(dw > 0) dest_buffer[ifb + z] = dw & dest_buffer[ifb + z];
        }else if (blt_mode == BLT_OR){
          if(dw > 0) dest_buffer[ifb + z] = dw | dest_buffer[ifb + z];
        }else if (blt_mode == BLT_XOR){
          if(dw > 0) dest_buffer[ifb + z] = dw ^ dest_buffer[ifb + z];
        }else if (blt_mode == BLT_MEAN){
          q7_t res[3];
          q7_t a[2];
          a[0] = (0x7F & (dest_buffer[ifb + z] >> 8));
          a[1] = (0x7F & (dw >> 8));
          arm_mean_q7(a,2,&res[0]);
          a[0] = (0x7F & (dest_buffer[ifb + z] >> 3));
          a[1] = (0x7F & (dw >> 3));
          arm_mean_q7(a,2,&res[1]);
          a[0] = (0x7F & (dest_buffer[ifb + z] << 3));
          a[1] = (0x7F & (dw << 3));
          arm_mean_q7(a,2,&res[2]);
          dest_buffer[ifb + z] = CL(res[0],res[1],res[2]);
        }else if (blt_mode == BLT_1ST_PIXEL_COLOR_KEY){
            if (dw != first_pixel) dest_buffer[ifb + z] = dw;
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
void FLASHMEM ILI9341_t3_ERIS::bltSDB(uint16_t *dest_buffer, uint16_t dest_buffer_width, uint16_t dest_buffer_height, const char *path, const char *filename,int16_t x,int16_t y,bltMode blt_mode){
  int16_t iy; // x & y index
  int16_t w;int16_t h; //width & height
  int16_t mx;        //left clip x offset
  int16_t nx;        //right clip x offset
  unsigned long ifb; //frame buffer index
  //uint16_t dwbf[SCREEN_WIDTH];//file read pixel row input buffer
  uint16_t dw;       //pixel data
  uint16_t first_pixel;
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

  //get first pixel value in case of color key by pixel
  file.readBytes((char*)&first_pixel,2);
  file.seek(file.position()-2L); //rewind the read pointer

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
      int16_t r,g,b;
      mx = 0; nx = 0;
      if (x < 0L){file.seekCur(x * -2L);mx = -1L * x;} //clip in x dimension (left) - skip offscreen data
      ifb = (iy * dest_buffer_width) + x + mx; //32bit index
      if (x + w > dest_buffer_width){nx = x + w - dest_buffer_width;}//clip in x dimension (right) - truncate copy to screen bounds
      for (uint16_t z = 0; z < (w - mx - nx); z += 1){
        file.read(&dw,2);
        toggle ^= true;
      
      //
      //pixel operation section
      //
      if (pixel_op_enable){
        int16_t src_r,src_g,src_b;
        bool is_not_color_keyed;
        //#define CL(_r, _g, _b) ((((_r)&0xF8) << 8) | (((_g)&0xFC) << 3) | ((_b) >> 3))
        //unpack the color channels from the 565 RGB format
        src_r = 0xFF & (dw >> 8); 
        if (src_r > 255) src_r = 255; //clip the max value for each channel
        src_g = 0xFF & (dw >> 3);
        if (src_g > 255) src_g = 255;
        src_b = 0xFF & (dw << 3);
        
        if ( (blt_mode == BLT_BLK_COLOR_KEY) && ((dw & 0xE79C) == 0) ) is_not_color_keyed = false;
        else if ( (blt_mode == BLT_1ST_PIXEL_COLOR_KEY) && (dw == first_pixel) ) is_not_color_keyed = false;
        else is_not_color_keyed = true;
        
        switch(pixel_op){
          case PXOP_1ST_PIXEL_COLOR_KEY:
            if (is_not_color_keyed) dw = pixel_op_param;
            break;
          case PXOP_BLK_COLOR_KEY:
            if (is_not_color_keyed) dw = pixel_op_param;
            break;
          case PXOP_HATCH_BLK:
            if (toggle && is_not_color_keyed) dw = 0;
            break;
          case PXOP_HATCH_XOR:
              if (toggle && is_not_color_keyed) dw = pixel_op_param^dw;
            break;
          case PXOP_COPY:
            if (is_not_color_keyed) dw = pixel_op_param; //note: same result as a fill function - could be used for benchmarking 
            break;
          case PXOP_ADD:
            if (is_not_color_keyed) dw = CL(src_r + pixel_op_param_r,src_g + pixel_op_param_g, src_b + pixel_op_param_b);
            break;
          case PXOP_AND:
            if (is_not_color_keyed) dw = CL(src_r & pixel_op_param_r,src_g & pixel_op_param_g, src_b & pixel_op_param_b);
            break;
          case PXOP_DIV:
            if (is_not_color_keyed) dw = CL(src_r / pixel_op_param_r,src_g / pixel_op_param_g, src_b / pixel_op_param_b);
            break;
          case PXOP_MEAN:
            if (is_not_color_keyed) dw = CL((src_r + pixel_op_param_r)/2,(src_g + pixel_op_param_g)/2, (src_b + pixel_op_param_b)/2);
            break;
          case PXOP_MULT:
            if (is_not_color_keyed) dw = CL(src_r * pixel_op_param_r,src_g * pixel_op_param_g, src_b * pixel_op_param_b);
            break;
          case PXOP_OR:
            if (is_not_color_keyed) dw = CL(src_r | pixel_op_param_r,src_g | pixel_op_param_g, src_b | pixel_op_param_b);
            break;
          case PXOP_SUB:
            if (is_not_color_keyed) dw = CL(src_r - pixel_op_param_r,src_g - pixel_op_param_g, src_b - pixel_op_param_b);
            break;
          case PXOP_XOR:
            if (is_not_color_keyed) dw = CL(src_r ^ pixel_op_param_r,src_g ^ pixel_op_param_g, src_b ^ pixel_op_param_b);
            break;
          case PXOP_NOT_1ST_PIXEL_COLOR_KEY:
            if (!is_not_color_keyed) dw = pixel_op_param;
            break;
          case PXOP_NOT_BLK_COLOR_KEY:
            if (!is_not_color_keyed) dw = pixel_op_param;
            break;
          //default:
            //dw = dw;//unknown op - just read the pixel
        };
      }//else dw = dw; //pixel op disabled so simply read the source pixel

        //bit transfer operation
        uint32_t write_index;
        write_index = ifb + z;
        if (blt_mode == BLT_COPY){
          dest_buffer[write_index] = dw;
        }else if (blt_mode == BLT_BLK_COLOR_KEY && ((dw & 0xE79C) != 0)){dest_buffer[write_index] = dw;
        }else if (blt_mode == BLT_HATCH_BLK){
          if ((dw & 0xE79C) != 0){ dest_buffer[write_index] = dw;
          } else if (toggle) dest_buffer[write_index] = 0; //pFB[i] ^= pFB[i];
        }else if (blt_mode == BLT_HATCH_XOR){
          if ((dw & 0xE79C) != 0) dest_buffer[write_index] = dw^dest_buffer[write_index];
          else if (toggle) dest_buffer[write_index] = dest_buffer[write_index];
        }else if (blt_mode == BLT_ADD){
          r = (dest_buffer[write_index] >> 8) + (0xFF & (dw >> 8));
          g = (dest_buffer[write_index] >> 3) + (0xFF & (dw >> 3));
          b = (dest_buffer[write_index] << 3) + (0xFF & (dw << 3));
          if(r>0xFF) r = 0xFF; //clip the result to 8bit
          if(g>0xFF) g = 0xFF;
          if(b>0xFF) b = 0xFF;
          dest_buffer[write_index] = CL(r,g,b); //convert back to 565 format and write the result
        }else if (blt_mode == BLT_SUB){
          r = (dest_buffer[write_index] >> 8) - (0xFF & (dw >> 8));
          g = (dest_buffer[write_index] >> 3) - (0xFF & (dw >> 3));
          b = (dest_buffer[write_index] << 3) - (0xFF & (dw << 3));
          if (r < 0) r = 0; //clamp the result to zero
          if (g < 0) g = 0;
          if (b < 0) b = 0;
          dest_buffer[write_index] = CL(r,g,b);
        }else if (blt_mode == BLT_MULT){
          r = (dest_buffer[write_index] >> 8) * (0xFF & (dw >> 8));
          g = (dest_buffer[write_index] >> 3) * (0xFF & (dw >> 3));
          b = (dest_buffer[write_index] << 3) * (0xFF & (dw << 3));
          if (r > 255) r = 255; //clip the max value for each channel
          if (g > 255) g = 255;
          if (b > 255) b = 255;
          dest_buffer[write_index] = CL(r,g,b);
        }else if (blt_mode == BLT_DIV){
          r = (dest_buffer[write_index] >> 8) / (0xFF & (dw >> 8));
          g = (dest_buffer[write_index] >> 3) / (0xFF & (dw >> 3));
          b = (dest_buffer[write_index] << 3) / (0xFF & (dw << 3));
          if (r > 255) r = 255; //clip the max value for each channel
          if (g > 255) g = 255;
          if (b > 255) b = 255;
          dest_buffer[write_index] = CL(r,g,b);
        }else if (blt_mode == BLT_AND){
          r = (dest_buffer[write_index] >> 8) & (0xFF & (dw >> 8));
          g = (dest_buffer[write_index] >> 3) & (0xFF & (dw >> 3));
          b = (dest_buffer[write_index] << 3) & (0xFF & (dw << 3));
          dest_buffer[write_index] = CL(r,g,b);
        }else if (blt_mode == BLT_OR){
          r = (dest_buffer[write_index] >> 8) | (0xFF & (dw >> 8));
          g = (dest_buffer[write_index] >> 3) | (0xFF & (dw >> 3));
          b = (dest_buffer[write_index] << 3) | (0xFF & (dw << 3));
          dest_buffer[write_index] = CL(r,g,b);
        }else if (blt_mode == BLT_XOR){
          r = (dest_buffer[write_index] >> 8) ^ (0xFF & (dw >> 8));
          g = (dest_buffer[write_index] >> 3) ^ (0xFF & (dw >> 3));
          b = (dest_buffer[write_index] << 3) ^ (0xFF & (dw << 3));
          dest_buffer[write_index] = CL(r,g,b);
        }else if (blt_mode == BLT_MEAN){
          q7_t res[3];
          q7_t a[2];
          a[0] = (0x7F & (dest_buffer[write_index] >> 8));
          a[1] = (0x7F & (dw >> 8));
          arm_mean_q7(a,2,&res[0]);
          a[0] = (0x7F & (dest_buffer[write_index] >> 3));
          a[1] = (0x7F & (dw >> 3));
          arm_mean_q7(a,2,&res[1]);
          a[0] = (0x7F & (dest_buffer[write_index] << 3));
          a[1] = (0x7F & (dw << 3));
          arm_mean_q7(a,2,&res[2]);
          dest_buffer[write_index] = CL(res[0],res[1],res[2]);
        }else if (blt_mode == BLT_1ST_PIXEL_COLOR_KEY){
          if (dw != first_pixel) dest_buffer[write_index] = dw;
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

void FASTRUN ILI9341_t3_ERIS::bltSDAnimationFullScreen(Animation *an){
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
void FASTRUN ILI9341_t3_ERIS::bltSDFullScreen(const char *filename){
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