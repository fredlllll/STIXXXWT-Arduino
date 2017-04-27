/**********************************************************************************************
   Arduino STIXXXWT Library - Version 1.0.0
   by Frederik Gelder <frederik.gelder@freenet.de>

   Copyright 2015 Frederik Gelder

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 **********************************************************************************************/
/*
  this is a little paint program to demonstrate the touch capabilities of the display
*/

#include <STIXXXWT.h>

STIXXXWT display;

void setup()
{
  Serial.begine(19200);
  display.connect(&Serial, 19200);
  display.sendHandshake(); //get infos
  display.clearScreen();
  display.setCommandListener(STIXXXWT::touchPressHold, &handleTouchPos, NULL);
  display.setCommandListener(STIXXXWT::touchRelease, &handleTouchRelease, NULL);
}

void loop()
{
  display.loop(); //check if the display sent us something
  delay(10);
}

bool isReleased = true;
point last;
void handleTouchPos( void* arg, uint8_t *data, int16_t len) {
  point p = *(point*)(data + 2);
  if (isReleased) {
    last = p;
  }
  isReleased = false;
  display.drawLineForeground(last, p);
  last = p;
}

void handleTouchRelease( void* arg, uint8_t *data, int16_t len) {
  isReleased = true;
}