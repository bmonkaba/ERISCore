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

#include "ILI9341_t3n.h"
#include <SdCard/SdioCard.h>
#include <SdFatConfig.h>
#include <SdFat.h>

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
    int16_t getWidth(){return w;}
    int16_t getHeight(){return h;}
        
  protected:        
    uint16_t *pSB;
    int16_t w;
    int16_t h;
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


//animation helper class

class Animation{
    friend class ILI9341_t3_ERIS;
    public:
        Animation(){
            frame = 1;chunk=0;last_frame=-1;pSD=NULL;
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
        SdFs* pSD;
        uint16_t frame;
        int16_t last_frame;
        uint16_t chunk;
        char filename[64];
        char _path[128];
};

enum bltAlphaType{AT_NONE, AT_TRANS, AT_HATCHBLK,AT_HATCHXOR};

class ILI9341_t3_ERIS : public ILI9341_t3n {
    public:
        //simply pass the constructor input parameters to the base class
        ILI9341_t3_ERIS(uint8_t cs, uint8_t dc, uint8_t rst = 255, uint8_t mosi=11, uint8_t sclk=13, uint8_t miso=12): ILI9341_t3n(cs,dc,rst,mosi,sclk,miso){
            //_SPI_CLOCK = 1000000;
            tft_write_speed = 74000000;
            tft_read_speed = 20000000;
            pSD = NULL;
            backlight = 0;
            pFB = NULL;    
        };
        void setSD(SdFs *ptr); //pointer to the SD Class
        void setPWMPin(uint8_t pin);
        void begin();
        //void flipBuffer();
        //void flipWritePointer();
        uint32_t getFrameAddress(){return (uint32_t)(void *)_pfbtft;}
        
        void bltSD(uint16_t *dest_buffer, uint16_t dest_buffer_width,const char *path, const char *filename,int16_t x,int16_t y,bltAlphaType alpha_type);
        void bltSD(const char *path, const char *filename,int16_t x,int16_t y,bltAlphaType alpha_type);  
        void bltSDFullScreen(const char *filename);
        void bltSDAnimationFullScreen(Animation *an);
        bool busy(){return _busy();} //(_dma_state & ILI9341_DMA_ACTIVE);}
        bool updateScreenAsyncFrom(ILI9341_t3_ERIS* draw,bool update_cont);
        //wrappers for drawing to a designated surface other than the main frame buffer(s)
        //write methods
        void bltMem(Surface *dest, Surface *source,int16_t x,int16_t y,bltAlphaType alpha_type);
        void fillSurface(Surface *dest,uint16_t color);
        void drawPixel(Surface *dest,int16_t x, int16_t y, uint16_t color);
        void drawSurfaceLine(Surface *dest,int16_t start_x, int16_t start_y,int16_t end_x, int16_t end_y, uint16_t color);
        void drawSurfaceFill(Surface *dest,uint16_t color);
        void drawFastVLine(Surface *dest,int16_t x, int16_t y, int16_t h, uint16_t color);
        void drawFastHLine(Surface *dest,int16_t x, int16_t y, int16_t w, uint16_t color);
        void fillRect(Surface *dest,int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        void fillRectHGradient(Surface *dest,int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1, uint16_t color2);
        void fillRectVGradient(Surface *dest,int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1, uint16_t color2);
        void fillScreenVGradient(Surface *dest,uint16_t color1, uint16_t color2);
        void fillScreenHGradient(Surface *dest,uint16_t color1, uint16_t color2);
        //read methods
        uint16_t readSurfacePixel(Surface *dest,int16_t x, int16_t y);
        uint16_t readPixel(int16_t x, int16_t y);

    protected:
        uint32_t tft_write_speed;
        uint32_t tft_read_speed;
        SdFs *pSD;
        FsFile file;
        uint16_t *pFB;  //framebuffer pointer     
        uint8_t backlight;
};



#endif