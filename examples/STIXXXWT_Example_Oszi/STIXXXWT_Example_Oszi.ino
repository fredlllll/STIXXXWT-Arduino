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

float analogDivider = 1023.0;
uint16_t currentX = 0;
dynamicCurvePoint data;
uint16_t resYm1,resX,resY;
//color16 lineColor;

void setup()
{
    display.connect(&Serial1,115200);
    for(int i = 0; i < 10; i++){
        display.sendHandshake(); //get infos
        delay(200);
        display.checkSerial();
        if(display.getResX() > 0){ //sometimes i had trouble getting the resolution from the display, thats why i do this loop
            break;
        }
    } //if we didnt get res after 2 seonds we give up
    display.clearScreen();

    //lineColor = color16(255,255,0);
    data.color = color16(255,255,0);//set color beforehand because it wont change
    resY = display.getResY();
    resYm1 = display.getResY()-1;
    resX = display.getResX();
    
    #ifdef __SAM3X8E__ //due only
    setupDue();
    #endif
}

#define SAMPLES (320)
uint16_t samples[SAMPLES];
void loop()
{
    //display.checkSerial(); //optional check if the display sent us something. we dont expect anything from the display anymore so we just dont check
    //delayMicroseconds(25); //4000 samples a second. have to test if the display can keep up
    for(int i =0; i< SAMPLES;i++){
        #ifdef __SAM3X8E__
        samples[i] = customAnalogRead(0);
        #else
        samples[i] = analogRead(0);
        #endif
    }

    int factor = SAMPLES / resX;
    for(int i =0; i< resX;i++){
      uint16_t tmp = 0;
      for(int j = 0; j< factor;j++){
        tmp += samples[i*factor+j];
      }
      tmp/=factor;
      data.y = tmp/analogDivider * resY;
      display.drawDynamicCurvePoint(i,0,resYm1,0,data);
    }
    //data.y = a*display.getResY();
    
    
    //display.drawDynamicCurvePoint(currentX,0,resYm1,0,data);
    
    //currentX++;
    //if(currentX >= resX){
    //    currentX = 0;
    //}
}

#ifdef __SAM3X8E__
void setupDue(){
    //make analog read faster.
    REG_ADC_MR = (REG_ADC_MR & ~ADC_MR_STARTUP_Msk) | ADC_MR_STARTUP_SUT0; // setting startup time to 0 ADC clock cycles
    REG_ADC_MR = (REG_ADC_MR & ~ADC_MR_FREERUN) | ADC_MR_FREERUN_ON; //enable FREERUN mode
    analogReadResolution(12);
    analogDivider = 4096.0;
}

uint32_t customAnalogRead(uint8_t ulPin){
    //this is basically a copy from wiring_analog.c but with minor differences (no clue if it really brings any speed, but lets give it a try)
    uint32_t ulValue = 0;
    uint32_t ulChannel;

    if (ulPin < A0)
    ulPin += A0;

    ulChannel = g_APinDescription[ulPin].ulADCChannelNumber ;

    static uint32_t latestSelectedChannel = -1;
    switch ( g_APinDescription[ulPin].ulAnalogChannel )
    {
        // Handling ADC 12 bits channels
        case ADC0 :
        case ADC1 :
        case ADC2 :
        case ADC3 :
        case ADC4 :
        case ADC5 :
        case ADC6 :
        case ADC7 :
        case ADC8 :
        case ADC9 :
        case ADC10 :
        case ADC11 :
            // Enable the corresponding channel
            if (ulChannel != latestSelectedChannel) {
                adc_enable_channel( ADC, static_cast<adc_channel_num_t>(ulChannel));
                if ( latestSelectedChannel != (uint32_t)-1 )
                    adc_disable_channel( ADC, static_cast<adc_channel_num_t>(latestSelectedChannel));
                latestSelectedChannel = ulChannel;
            }

            // Start the ADC
            adc_start( ADC );

            // Wait for end of conversion
            while ((adc_get_status(ADC) & ADC_ISR_DRDY) != ADC_ISR_DRDY)
                ;

            // Read the value
            ulValue = adc_get_latest_value(ADC);
            //ulValue = mapResolution(ulValue, ADC_RESOLUTION, _readResolution); //we dont need map resolution. we want the full 12 bit. yes i know that this will only shed a few instructions. 

            break;

        // Compiler could yell because we don't handle DAC pins
        default :
            ulValue=0;
            break;
    }
    return ulValue;
}
#endif