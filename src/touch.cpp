#include "touch.h"
#include "HSI.h"

#define Z_THRESHOLD     400
#define Z_THRESHOLD_INT	75
#define MSEC_THRESHOLD  3
#define SPI_SETTING     SPISettings(3000000, MSBFIRST, SPI_MODE0)

bool Touch::begin(){
	Serial.println("Touch:begin");
    //SPI1.setCS(CS_TOUCH);
    SPI1.setMISO(SDI);
    SPI1.setMOSI(SDO);
    SPI1.setSCK(SCLK);

    SPI1.begin();
	pinMode(csPin, OUTPUT);
	digitalWrite(csPin, HIGH);

	tirqPin = 255;
    /*
	if (255 != tirqPin) {
		pinMode( tirqPin, INPUT );
		attachInterrupt(digitalPinToInterrupt(tirqPin), isrPin, FALLING);
		isrPinptr = this;
	}
    */
	return true;
}

static int16_t besttwoavg( int16_t x , int16_t y , int16_t z ) {
  int16_t da, db, dc;
  int16_t reta = 0;
  if ( x > y ) da = x - y; else da = y - x;
  if ( x > z ) db = x - z; else db = z - x;
  if ( z > y ) dc = z - y; else dc = y - z;
  if ( da <= db && da <= dc ) reta = (x + y) >> 1;
  else if ( db <= da && db <= dc ) reta = (x + z) >> 1;
  else reta = (y + z) >> 1;   //    else if ( dc <= da && dc <= db ) reta = (x + y) >> 1;
  return (reta);
}

void Touch::update()
{
	//Serial.println("Touch:update");
	int16_t data[6];

	if (!isrWake) return;
	uint32_t now = millis();
	if (now - msraw < MSEC_THRESHOLD) return;

	SPI1.beginTransaction(SPI_SETTING);
	digitalWrite(csPin, LOW);
	SPI1.transfer(0xB1 /* Z1 */);
	int16_t z1 = SPI1.transfer16(0xC1 /* Z2 */) >> 3;
	int z = z1 + 4095;
	int16_t z2 = SPI1.transfer16(0x91 /* X */) >> 3;
	z -= z2;
	if (z >= Z_THRESHOLD) {
		SPI1.transfer16(0x91 /* X */);  // dummy X measure, 1st is always noisy
		data[0] = SPI1.transfer16(0xD1 /* Y */) >> 3;
		data[1] = SPI1.transfer16(0x91 /* X */) >> 3; // make 3 x-y measurements
		data[2] = SPI1.transfer16(0xD1 /* Y */) >> 3;
		data[3] = SPI1.transfer16(0x91 /* X */) >> 3;
	}
	else data[0] = data[1] = data[2] = data[3] = 0;	// Compiler warns these values may be used unset on early exit.
	data[4] = SPI1.transfer16(0xD0 /* Y */) >> 3;	// Last Y touch power down
	data[5] = SPI1.transfer16(0) >> 3;
	digitalWrite(csPin, HIGH);
	SPI1.endTransaction();
	//Serial.printf("z=%d  ::  z1=%d,  z2=%d  ", z, z1, z2);
	if (z < 0) z = 0;
	if (z < Z_THRESHOLD) { //	if ( !touched ) {
		// Serial.println();
		zraw = 0;
		if (z < Z_THRESHOLD_INT) { //	if ( !touched ) {
			if (255 != tirqPin) isrWake = false;
		}
		return;
	}
	zraw = z;

	// Average pair with least distance between each measured x then y
	//Serial.printf("    z1=%d,z2=%d  ", z1, z2);
	//Serial.printf("p=%d,  %d,%d  %d,%d  %d,%d", zraw,
		//data[0], data[1], data[2], data[3], data[4], data[5]);
	int16_t x = besttwoavg( data[0], data[2], data[4] );
	int16_t y = besttwoavg( data[1], data[3], data[5] );

	//Serial.printf("    %d,%d", x, y);
	//Serial.println();
	if (z >= Z_THRESHOLD) {
		msraw = now;	// good read completed, set wait
		switch (rotation) {
		  case 0:
			xraw = 4095 - y;
			yraw = x;
			break;
		  case 1:
			xraw = x;
			yraw = y;
			break;
		  case 2:
			xraw = y;
			yraw = 4095 - x;
			break;
		  default: // 3
			xraw = 4095 - x;
			yraw = 4095 - y;
		}
	}
}

bool Touch::touched()
{
	//Serial.println("Touch:touched");
	update();
	return (zraw >= Z_THRESHOLD);
}

TS_Point Touch::getPoint()
{
	float fx,fy;
	int16_t x,y;
	//Serial.println("Touch:getPoint");
	update();
	
	//additional dynamic calibration of the screen       
	if (xraw < _raw_minx) _raw_minx = xraw;
	if (xraw > _raw_maxx) _raw_maxx = xraw;
	if (yraw < _raw_miny) _raw_miny = yraw;
	if (yraw > _raw_maxy) _raw_maxy = yraw;

	//translate the raw values to screen space
	//tranlate raw touch readings to percentage of full scale (0 to 1)
	fx = (xraw - _raw_minx)/(1.0 * (_raw_maxx - _raw_minx));
	fy = (yraw - _raw_miny)/(1.0 * (_raw_maxy - _raw_miny));
	x = (int16_t)(fx * 320); 
	y = (int16_t)(fy * 240);
	(x < 0)?x=0:x=x;
	(y < 0)?y=0:y=y;
	
	return TS_Point(x, y, zraw);
}
