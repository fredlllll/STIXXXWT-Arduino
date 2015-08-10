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

#include <STIXXXWT_v1.h>

STIXXXWT display;

void setup()
{
    display.connect(&Serial,19200);
    display.sendHandshake(); //get infos
    display.clearScreen();
}

uint16_t currentX = 0;
void loop()
{
    display.checkSerial(); //check if the display sent us something
    delay(10);
    float a = analogRead(0)/1023.0;
    float b = analogRead(1)/1023.0;
    float c = analogRead(2)/1023.0;
    float d = analogRead(3)/1023.0;
    dynamicCurvePoint data[4];
    data[0].y = a*display.getResY();
    data[0].color = color16(255,0,0);
    data[1].y = b*display.getResY();
    data[1].color = color16(0,255,0);
    data[2].y = c*display.getResY();
    data[2].color = color16(0,0,255);
    data[3].y = d*display.getResY();
    data[3].color = color16(255,255,0);
    
    display.drawDynamicCurvePoints(currentX,0,display.getResY()-1,0,data,4);
    
    currentX++;
    if(currentX >= display.getResX()){
        currentX = 0;
    }
}