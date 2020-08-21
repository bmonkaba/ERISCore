#ifndef _touch_
#define _touch_

#include <XPT2046_Touchscreen.h>

//inherit and extent the standard library
class Touch : public XPT2046_Touchscreen {
    public:
        int16_t xraw=0, yraw=0, zraw=0;
        Touch(uint8_t cspin, uint8_t tirq=255): XPT2046_Touchscreen(cspin,  tirq){csPin=cspin;tirqPin=tirq;};
        bool begin();
        bool touched();
        void setCalibrationInputs(uint16_t raw_minx,uint16_t raw_miny,uint16_t raw_maxx,uint16_t raw_maxy){
            _raw_minx = raw_minx;
            _raw_miny = raw_miny;
            _raw_maxx = raw_maxx;
            _raw_maxy = raw_maxy;
         }

        TS_Point getPoint();

        

    protected:
        void update();
        uint8_t csPin, tirqPin, rotation=1;
        uint32_t msraw=0x80000000;
        uint16_t _raw_minx;
        uint16_t _raw_miny;
        uint16_t _raw_maxx;
        uint16_t _raw_maxy;
};

#endif