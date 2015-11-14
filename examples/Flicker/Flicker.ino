/**********************************************************************************************
 * Arduino STIXXXWT Library - Version 1.0.0
 * by Frederik Gelder <frederik.gelder@freenet.de>
 *
 * Copyright 2015 Frederik Gelder
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **********************************************************************************************/
/*
 * This example lets the screen flicker in random colors. its just to demonstrate the refresh rate of the display
 */

#include <STIXXXWT.h>

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
    display.loop(); //check if the display sent us something
    delay(1000/60);
    display.setForeAndBackgroundColor(display.foregroundColor,color16(random(0,255),random(0,255),random(0,255)));
    display.fillRectangleBackground(display.screenRectangle);
}