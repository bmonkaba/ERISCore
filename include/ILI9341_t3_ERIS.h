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
            _SPI_CLOCK = 70000000;
            //pSD = NULL;
            //backlight = 0;
            //pFB = NULL;    
        };
        void setSD(SdFs *ptr); //pointer to the SD Class
        void setPWMPin(uint8_t pin);
        void begin();
        void bltSD(const char *path, const char *filename,int16_t x,int16_t y,UIBLTAlphaType alpha_type);  
        void bltSDFullScreen(const char *filename);
        bool busy(){return (_dma_state & ILI9341_DMA_ACTIVE);}

    protected:
        SdFs *pSD;
        FsFile file;
        uint8_t backlight;
        uint16_t *pFB;  //framebuffer pointer      
};

#endif