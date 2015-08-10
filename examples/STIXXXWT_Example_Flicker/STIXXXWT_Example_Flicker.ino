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
    
    randomSeed(analogRead(0));
}

void loop()
{
    display.checkSerial(); //check if the display sent us something
    delay(1000/60);
    display.setForeAndBackgroundColor(display.foregroundColor,STIXXXWT::convert24bitColorTo16bitColor(random(0,255),random(0,255),random(0,255)));
    display.fillRectangleBackground(display.screenRectangle);
}