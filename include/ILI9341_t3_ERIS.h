/**
 * @file ILI9341_t3_ERIS.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _ILI9341_t3ERIS_
#define _ILI9341_t3ERIS_
#include <LittleFS.h>
#include "ILI9341_t3n.h"
#include <SdCard/SdioCard.h>
#include <SdFatConfig.h>
#include <SdFat.h>
#include <LittleFS.h>

#include "T4_PXP.h"

// ERIS SD Graphics extention 
// Implements 2d image transfer from an SD card to a RAM framebuffer viewport 
void renderCompleteCB();
bool _busy();

//every surface owns and manages it's own memory
//therfore a 'Root' surface owns the entire surface memory (actual) from which it's space is 'BORROWED' by children
//a 'child' surface may 'SHARE' it's space with 'FRIENDS' .. etc.
//space is managed both directly and indirectly
//allocated space may be revoked and given to other sufaces as required

// min alloc size - lazy attemmpt to reduce potental memory fragmentation issues
#define SURFACE_MIN_ALLOC_SIZE 256



/**
 * @brief a Surface object manages a memory block with defined width and height dimensions
 * 
 */
class Surface {
  public:
    Surface(Surface *SubSurfaceFrom, SdFs* pSD, const char* path, const char* fileName){
        char str[16]; //char buffer
        char *c;     //char pointer
        FsFile file;
        guid = hash(path) * hash(fileName);
        w = 0;//init these in case of a bad path or file.
        c = 0;
        h = 0;
        alloc_size = 0;
        pSB = NULL;
        //open image, get size info, attempt to allocate a buffer then copy
        //and capture the dimensions
        if (!pSD->chdir(path)){ //change file path
            Serial.print(F("M ILI9341_t3_ERIS::bltSD Path not found: "));
            Serial.println(path);
            return;             
        }
        file.open(fileName, O_READ);//open image to read
        if (file.available() == 0){ //file not found
            Serial.print(F("M ILI9341_t3_ERIS::bltSD File Not Found: "));
            Serial.println(fileName);
            //pSD->ls();
            return;
        }
        file.fgets(str,sizeof(str)); //read the header data
        file.fgets(str,sizeof(str)); //to get the image dimensions
        file.close();
        strtok(str," ");             //convert dimension text to numbers
        w = atol(str);
        c = strtok(NULL, " ");
        h = atol(c);
        alloc_size = w*h;
        if (alloc_size < SURFACE_MIN_ALLOC_SIZE) alloc_size = SURFACE_MIN_ALLOC_SIZE;
        pSB = SubSurfaceFrom->requestSubSurfaceBufferP(alloc_size);
    }
    
    Surface(Surface *SubSurfaceFrom, int16_t width, int16_t height){
        guid =  ((uint32_t)SubSurfaceFrom<<4) * ((uint32_t)width + ((uint32_t)height<<8));
        w = width;
        h = height;
        alloc_size = w*h;
        if (alloc_size < SURFACE_MIN_ALLOC_SIZE) alloc_size = SURFACE_MIN_ALLOC_SIZE;
        pSB = SubSurfaceFrom->requestSubSurfaceBufferP(alloc_size);
        //attempt to allocate a sub surface buffer then clear it
        /*
        Serial.print(F("M SubSurface alloted: "));
        Serial.print(alloc_size);
        Serial.print(F(" pixels GUID: "));
        Serial.println(guid);
        */
    }
    
    Surface(uint16_t *buffer,uint32_t length){
        //take an existing buffer as the surface buffer
        guid =  (uint32_t)buffer<<2 * (uint32_t)length;
        pSB = buffer;
        w = 0;
        h = 0;
        alloc_size = length;
        head = 0; //used for sub surface allocation
        /*
        Serial.print(F("M Surface assigned: "));
        Serial.print(alloc_size);
        Serial.print(F(" pixels GUID: "));
        Serial.println(guid);
        */
    }

    Surface(uint16_t *buffer,int16_t width, int16_t height){
        //take an existing buffer as the surface buffer
        guid =  (uint32_t)buffer * ((uint32_t)width<<11) / (1 + (uint32_t)height);
        pSB = buffer;
        w = width;
        h = height;
        alloc_size = w*h;
        head = 0; //used for sub surface allocation
        /*
        Serial.print(F("M Surface assigned: "));
        Serial.print(alloc_size);
        Serial.print(F(" pixels GUID: "));
        Serial.println(guid);
        */
    }

    Surface(int16_t width, int16_t height){
        //allocate and attach a buffer as the surface buffer
        pSB = NULL;
        w = width;
        h = height;
        head = 0;
        uint16_t *buffer;
        buffer = (uint16_t*)malloc(width*height*sizeof(uint16_t));
        Surface(buffer,width,height);
        Serial.print(F("M Surface buffer allocated: "));
        Serial.print(alloc_size);
        Serial.print(F(" pixels Surface GUID: "));
        Serial.println(guid);
    }

    uint16_t* requestSubSurfaceBufferP(uint32_t size){
        /* provides an interface to enable childern subsurfaces
           to request a portion of the parents surface buffer

           function input is size of the surface array of N elements
           each element of the array being two bytes (uint16_t).
           so the allocation head will be moved size * 2
        */
        uint16_t* pSSB;
        if ((head + size) > alloc_size){
            //not enough space
            Serial.println(F("M ERROR: Surface out of MEM"));
            return NULL;
        }
        if (!pSB){
            Serial.println(F("M ERROR: Surface has no buffer"));
            return NULL;
        }
        pSSB = pSB + (head); 
        head += size;
        Serial.println(F("M SubSurface allocated"));
        Serial.print(F("M Surface buffer remaining:"));
        Serial.println(alloc_size - head);
        return pSSB;
    }

    uint16_t* getSurfaceBufferP(){return pSB;}
    uint16_t getWidth(){return w;}
    uint16_t getHeight(){return h;}
        
  protected:        
    uint16_t *pSB;
    uint16_t w;
    uint16_t h;
    uint32_t alloc_size;
    uint32_t guid;
    uint32_t head;
    
    void allocate();
    void revoke();
    uint32_t hash(const char* s){
        //uint32 djb2 string hash
        uint32_t h = 5381;
        int c;
        while ((c = *s++)){
            h = ((h << 5) + h) + c;
        }
        return h;
    }
};



/**
 * @brief helper class used to render the animated wallpaper in chunks
 * 
 */
class Animation{
    friend class ILI9341_t3_ERIS;
    public:
        Animation(){
            frame = 1;chunk=0;last_frame=-1;p_SD=NULL;
            memset(filename,0,sizeof(filename));
            memset(_path,0,sizeof(_path));
        
        }
        void setPath(const char *path){
            if (strlen(path) < 120) strcpy(_path,path);
            last_frame = -1; //reset end of animation frame marker
            return;
        }
        bool isFrameComplete(){
            if(chunk==0)return true; 
            return false;
        }
        bool getNextFrameChunk();
        void setSD(SdFs *ptr);
        char* getFileName(){return filename;};
        char* getPath(){return _path;};
    protected:
        SdFs* p_SD;
        uint16_t frame;
        int16_t last_frame;
        uint16_t chunk;
        char filename[64];
        char _path[128];
};

/**
 * @brief block transfer operation
 * 
 */
enum bltMode{BLT_COPY,BLT_BLK_COLOR_KEY,BLT_HATCH_BLK,BLT_HATCH_XOR,BLT_ADD,BLT_SUB,BLT_MULT,BLT_DIV,BLT_AND,BLT_OR,BLT_XOR,BLT_MEAN,BLT_1ST_PIXEL_COLOR_KEY};
enum pixelOPMode{PXOP_COPY,PXOP_BLK_COLOR_KEY,PXOP_HATCH_BLK,PXOP_HATCH_XOR,PXOP_ADD,PXOP_SUB,PXOP_MULT,PXOP_DIV,PXOP_AND,PXOP_OR,PXOP_XOR,PXOP_MEAN,PXOP_1ST_PIXEL_COLOR_KEY, PXOP_NOT_1ST_PIXEL_COLOR_KEY, PXOP_NOT_BLK_COLOR_KEY};


/**
 * @brief Eris class extentions to the ILI9341_t3n base class 
 * This class is resposible for managing the tft display and providing functional extentions and interfaces for working with Surface class parameters
 */
class ILI9341_t3_ERIS : public ILI9341_t3n {
    public:
        //simply pass the constructor input parameters to the base class
        ILI9341_t3_ERIS(uint8_t cs, uint8_t dc, uint8_t rst = 255, uint8_t mosi=11, uint8_t sclk=13, uint8_t miso=12): ILI9341_t3n(cs,dc,rst,mosi,sclk,miso){
            //_SPI_CLOCK = 1000000;
            tft_write_speed = 76000000;
            tft_read_speed = 30000000;
            p_SD = NULL;
            backlight = 0;
            pFB = NULL;
            pixel_op = PXOP_MEAN;
            pixel_op_param = 0;
            pixel_op_param_r = 0;
            pixel_op_param_g = 0;
            pixel_op_param_b = 0;
            pixel_op_enable = false;  
            PXP_init(); 
        };
        void setSD(SdFs *ptr); //pointer to the SD Class
        void setPWMPin(uint8_t pin);
        void begin();
        //void flipBuffer();
        //void flipWritePointer();
        
        /**
         * @brief enable literal pixel operations on the source buffer within the rendering pipeline
         * 
         * @param param is the literal value to be applied to the source buffer during BitBlts
         * @param operation is an enumerated pixel operation mode (for math operations like: and, or, add, sub, mult, etc.)
         */
        void enablePixelOP(uint16_t param,pixelOPMode operation){
            pixel_op_param = param;
            pixel_op = operation;
            pixel_op_enable = true;
            //unpack the color channels from the 565 RGB format
            pixel_op_param_r = 0xF8 & (param >> 8); 
            pixel_op_param_g = 0xFC & (param >> 3);
            pixel_op_param_b = 0xF8 & (param << 3); 
        }; 

        void disablePixelOP(){
            pixel_op_enable = false; 
        }; 

        /**
         * @brief Get the Frame Address object
         * 
         * @return uint32_t 
         */
        uint32_t getFrameAddress(){return (uint32_t)(void *)_pfbtft;}

        
        /**
         * @brief loads the width and height parameters from the given image
         * returns true if the values have been loaded
         * returns false if the file could not be found 
         * 
         * @param path 
         * @param filename 
         * @param width 
         * @param height 
         * @param file_system optional pointer to a LittleFS file system, otherwise the SD card is used 
         * @return true 
         * @return false 
         */
        bool getImageSize(const char* path,const char* filename,int32_t* width,int32_t* height, LittleFS_RAM* file_system = NULL);


        /**
         * @brief prints the string buffer using the provided sprite library name and pt size
         * 
         * @param string_buffer 
         * @param x 
         * @param y 
         * @param font 
         * @param pt 
         */
        void printWithRAMFileSystemFont(const char* string_buffer,uint16_t x,uint16_t y,const char* font,uint16_t pt,LittleFS_RAM* file_system);

        /**
         * @brief prints the string buffer using the provided sprite library name and pt size
         * 
         * @param string_buffer 
         * @param x 
         * @param y 
         * @param font 
         * @param pt 
         * @param useKernFile apply kerning using the font .krn file; default is disable (false)
         * @param file_system optional pointer to a LittleFS file system, otherwise the SD card is used 
         */
        void printWithFont(const char* string_buffer,uint16_t x,uint16_t y,const char* font,uint16_t pt,LittleFS_RAM* file_system = NULL);

        /**
         * @brief blt from an open TinyFS open file pointer to a destination buffer
         * 
         * @param dest_buffer 
         * @param dest_buffer_width 
         * @param dest_buffer_height 
         * @param file 
         * @param blt_mode 
         */
        void bltRAMFileB(uint16_t *dest_buffer, uint16_t dest_buffer_width, uint16_t dest_buffer_height, File* file,int16_t x,int16_t y,bltMode blt_mode);

        /**
         * @brief image block transfer from the area given by src_x, src_y, src_width, src_height to the destination given similar parameters
         * used for composing image areas from one buffer to another. no scaling is applied.
         * 
         * @param dest_buffer 
         * @param dest_x 
         * @param dest_y 
         * @param dest_buffer_width 
         * @param dest_buffer_height 
         * @param file_name 
         * @param src_x 
         * @param src_y 
         * @param src_width 
         * @param src_height 
         * @param blt_mode 
         */
        void bltArea2Buffer(uint16_t *dest_buffer, int16_t dest_x,int16_t dest_y, uint16_t dest_buffer_width, \
            uint16_t dest_buffer_height, const char *file_name,int16_t src_x,int16_t src_y, uint16_t src_width, \
             uint16_t src_height,bltMode blt_mode,bool strip_color_key_rows, LittleFS_RAM* file_system = NULL);
        
        /**
         * @brief image block transfer from the SD card to a buffer at the given x,y coords
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
        void bltSDB(uint16_t *dest_buffer, uint16_t dest_buffer_width, uint16_t dest_buffer_height, const char *path, const char *filename,int16_t x,int16_t y,bltMode blt_mode);
        
        void bltSD(const char *path, const char *filename,int16_t x,int16_t y,bltMode blt_mode);  
        
        /**
         * @brief used for block transfer from the SD to the frame buffer
         * images should be the same dimensions as the display
         * @param filename 
         */
        void bltSDFullScreen(const char *filename);
        
        /**
         * @brief renders an image chunk using the information provided by Animation parameter
         * This function is used to render the animated backgrounds
         * 
         * @param an 
         */
        void bltSDAnimationFullScreen(Animation *an);
        
        /**
         * @brief tft busy check
         * 
         * @return true 
         * @return false 
         */
        bool busy(){return _dma_state & ILI9341_DMA_ACTIVE;}//{return _busy();} //
        
        bool updateScreenAsyncFrom(ILI9341_t3_ERIS* draw,bool update_cont);
        
        //wrappers for drawing to a designated surface other than the main frame buffer(s)
        //write methods
        
        /**
         * @brief block transfer from source to dest with the given parameters
         * 
         * @param dest 
         * @param dest_x 
         * @param dest_y 
         * @param source 
         * @param from_x 
         * @param from_y 
         * @param from_width 
         * @param from_height 
         * @param blt_mode 
         */
        void bltSurface2Surface(Surface *dest, int16_t dest_x,int16_t dest_y, Surface *source, int16_t from_x, int16_t from_y, int16_t from_width, int16_t from_height,bltMode blt_mode);
        
        /**
         * @brief
         * 
         * @param dest 
         * @param color 
         */
        void fillSurface(Surface *dest,uint16_t color,bltMode blt_mode = BLT_COPY);

        /**
         * @brief 
         * 
         * @param dest 
         * @param x 
         * @param y 
         * @param color 
         */
        void drawPixel(Surface *dest,int16_t x, int16_t y, uint16_t color);

        /**
         * @brief 
         * 
         * @param dest 
         * @param start_x 
         * @param start_y 
         * @param end_x 
         * @param end_y 
         * @param color 
         */
        void drawSurfaceLine(Surface *dest,int16_t start_x, int16_t start_y,int16_t end_x, int16_t end_y, uint16_t color);

        /**
         * @brief 
         * 
         * @param dest 
         * @param color 
         */
        void drawSurfaceFill(Surface *dest,uint16_t color);

        /**
         * @brief 
         * 
         * @param dest 
         * @param x 
         * @param y 
         * @param h 
         * @param color 
         */
        void drawFastVLine(Surface *dest,int16_t x, int16_t y, int16_t h, uint16_t color);

        /**
         * @brief 
         * 
         * @param dest 
         * @param x 
         * @param y 
         * @param w 
         * @param color 
         */
        void drawFastHLine(Surface *dest,int16_t x, int16_t y, int16_t w, uint16_t color);

        /**
         * @brief 
         * 
         * @param dest 
         * @param x 
         * @param y 
         * @param w 
         * @param h 
         * @param color 
         */
        void fillRect(Surface *dest,int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

        /**
         * @brief 
         * 
         * @param dest 
         * @param x 
         * @param y 
         * @param w 
         * @param h 
         * @param color1 
         * @param color2 
         */
        void fillRectHGradient(Surface *dest,int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1, uint16_t color2);

        /**
         * @brief 
         * 
         * @param dest 
         * @param x 
         * @param y 
         * @param w 
         * @param h 
         * @param color1 
         * @param color2 
         */
        void fillRectVGradient(Surface *dest,int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1, uint16_t color2);

        /**
         * @brief 
         * 
         * @param dest 
         * @param color1 
         * @param color2 
         */
        void fillScreenVGradient(Surface *dest,uint16_t color1, uint16_t color2);

        /**
         * @brief 
         * 
         * @param dest 
         * @param color1 
         * @param color2 
         */
        void fillScreenHGradient(Surface *dest,uint16_t color1, uint16_t color2);

        /**
         * @brief reads the pixel data from the surface object
         * note the data is packed in the 565 RGB format
         * @param source 
         * @param x 
         * @param y 
         * @return uint16_t 
         */
        uint16_t readSurfacePixel(Surface *source,int16_t x, int16_t y);
        
        /**
         * @brief reads the pixel data from the frame buffer
         * note the data is packed in the 565 RGB format
         * @param x 
         * @param y 
         * @return uint16_t 
         */
        uint16_t readPixel(int16_t x, int16_t y);

    protected:
        uint32_t tft_write_speed;
        uint32_t tft_read_speed;
        SdFs *p_SD;
        FsFile file;
        uint16_t *pFB;  //framebuffer pointer     
        uint8_t backlight;
        uint16_t pixel_op_param;
        uint16_t pixel_op_param_r;
        uint16_t pixel_op_param_g;
        uint16_t pixel_op_param_b;
        pixelOPMode pixel_op;
        bool pixel_op_enable;
};



#endif