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

enum UIBLTAlphaType{AT_NONE, AT_TRANS, AT_HATCHBLK,AT_HATCHXOR};

class ILI9341_t3_ERIS : public ILI9341_t3n {
    public:
        //simply pass the constructor input parameters to the base class
        ILI9341_t3_ERIS(uint8_t cs, uint8_t dc, uint8_t rst = 255, uint8_t mosi=11, uint8_t sclk=13, uint8_t miso=12): ILI9341_t3n(cs,dc,rst,mosi,sclk,miso){
            _SPI_CLOCK = 80000000;
            //pSD = NULL;
            //backlight = 0;
            //pFB = NULL;    
        };
        void setSD(SdFs *ptr); //pointer to the SD Class
        void setPWMPin(uint8_t pin);
        void begin();
        void flipBuffer();
        void flipWritePointer();
        void bltSD(const char *path, const char *filename,int16_t x,int16_t y,UIBLTAlphaType alpha_type);  
        void bltSDFullScreen(const char *filename);
        bool busy(){return (_dma_state & ILI9341_DMA_ACTIVE);}
        bool updateScreenAsync(bool update_cont){
            bool rval;
            _dma_state &= ~ILI9341_DMA_INIT;
            rval= ILI9341_t3n::updateScreenAsync(update_cont);
            flipWritePointer();
            return rval;
        }
    protected:
        SdFs *pSD;
        FsFile file;
        uint8_t backlight;
        uint16_t *pFB;  //framebuffer pointer
        uint16_t *pFB2;  //framebuffer pointer     
};

#endif