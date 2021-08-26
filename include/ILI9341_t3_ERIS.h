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
#include <SdFat-beta.h>


// ERIS SD Graphics extention 
// Implements 2d image transfer from an SD card to a RAM framebuffer viewport 


//animatiobn helper class
class Animation{
    friend class ILI9341_t3_ERIS;
    public:
        Animation(){frame = 1;chunk=0;}
        void setPath(const char *path){
            if (strlen(path) < 120) strcpy(_path,path);
            return;
        };
        bool isFrameComplete(){
            if(chunk==0)return true; 
            return false;
        }
        bool getNextFrameChunk(SdFs *ptr);
        char* getFileName(){return filename;};
        char* getPath(){return _path;};
    protected:
        uint16_t frame;
        uint16_t chunk;
        char filename[128];
        char _path[128];
};


enum UIBLTAlphaType{AT_NONE, AT_TRANS, AT_HATCHBLK,AT_HATCHXOR};

class ILI9341_t3_ERIS : public ILI9341_t3n {
    public:
        //simply pass the constructor input parameters to the base class
        ILI9341_t3_ERIS(uint8_t cs, uint8_t dc, uint8_t rst = 255, uint8_t mosi=11, uint8_t sclk=13, uint8_t miso=12): ILI9341_t3n(cs,dc,rst,mosi,sclk,miso){
            //_SPI_CLOCK = 1000000;
            tft_write_speed = 40000000;
            tft_read_speed = 20000000;

            //pSD = NULL;
            //backlight = 0;
            //pFB = NULL;    
        };
        void setSD(SdFs *ptr); //pointer to the SD Class
        void setPWMPin(uint8_t pin);
        void begin();
        void flipBuffer();
        void flipWritePointer();
        uint32_t getFrameAddress(){return (uint32_t)(void *)pFB;}
        uint32_t getFrame2Address(){return (uint32_t)(void *)pFB2;}
        void bltSD(const char *path, const char *filename,int16_t x,int16_t y,UIBLTAlphaType alpha_type);  
        void bltSDFullScreen(const char *filename);
        void bltSDAnimationFullScreen(Animation *an);
        bool busy(){return (_dma_state & ILI9341_DMA_ACTIVE);}
        bool updateScreenAsync(bool update_cont){
            bool rval;
            _dma_state &= ~ILI9341_DMA_INIT;
            rval= ILI9341_t3n::updateScreenAsync(update_cont);
            flipBuffer();
            flipWritePointer();
            return rval;
        }
    protected:
        uint32_t tft_write_speed;
        uint32_t tft_read_speed;
        SdFs *pSD;
        FsFile file;
        uint8_t backlight;
        uint16_t *pFB;  //framebuffer pointer
        uint16_t *pFB2;  //framebuffer pointer     
};

#endif