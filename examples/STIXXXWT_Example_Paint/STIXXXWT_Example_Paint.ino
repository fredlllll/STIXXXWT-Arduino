/********************************************************
 * STIXXXWT Example
 ********************************************************/

#include <STIXXXWT_v1.h>

STIXXXWT display;

void setup()
{
    display.connect(&Serial,19200);
    display.sendHandshake(); //get infos
    display.clearScreen();
    display.setCommandListener(STIXXXWT::touchPressHold,&handleTouchPos);
	display.setCommandListener(STIXXXWT::touchRelease,&handleTouchRelease);
}

void loop()
{
    display.checkSerial(); //check if the display sent us something
    delay(10);
}

bool isReleased = true;
point last;
void handleTouchPos(uint8_t *data, int16_t len){
    point p = *(point*)(data+2);
	if(isReleased){
		last = p;
	}
	isReleased = false;
	display.drawLineForeground(last,p);
    last = p;
}

void handleTouchRelease(uint8_t *data, int16_t len){
	isReleased = true;
}
