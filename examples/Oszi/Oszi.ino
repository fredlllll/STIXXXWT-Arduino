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
 * THIS EXAMPLE IS FOR THE ARDUINO DUE ONLY!
 * This realizes an oscilloscope with an arduino due
 * it has positive flank triggering and ~600KSa/s
 * Samples can be changed and the result will be scaled on the display accordingly
 */
#include <STIXXXWT_v1.h>

const int SAMPLES = 1000;

STIXXXWT display;

const float analogDivider = 4096.0;
float triggerPosition = 0.25;
float triggerThreshold = 0.25;
dynamicCurvePoint data;
uint16_t resYm1,resX,resY;

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

    data.color = color16(255,255,0);//set color beforehand because it wont change
    resY = display.getResY();
    resYm1 = display.getResY()-1;
    resX = display.getResX();
    
    setupADC();
}

uint16_t samples[SAMPLES];
char waitForTrigger[] = "Waiting for Trigger";
void loop()
{
    //display.checkSerial(); //optional check if the display sent us something. we dont expect anything from the display anymore so we just dont check to save time 
    display.displayStandardText(STIXXXWT::ASCII8_16,point(10,10), waitForTrigger, sizeof(waitForTrigger)-1);
    
    for(int i =0; i< SAMPLES;i++){ //fill array with current data
        samples[i] = customAnalogRead(); 
    }
    int s = 0;
    uint16_t tt = analogDivider * triggerThreshold;
    uint16_t last = tt;
    while(true){
        uint16_t tmp = customAnalogRead();
        samples[s] = tmp;
        s++;
        if(s >= SAMPLES){
            s = 0;
        }
        if(tmp >= tt && last < tt){
            for(int i = SAMPLES * (1 - triggerPosition); i >=0; i--){
                samples[(s++)%SAMPLES] = customAnalogRead();
            }
            s %= SAMPLES;
            break;
        }
        last = tmp;
    }

    int factor = SAMPLES / resX;
    for(int i =0; i < resX;i++){
      uint32_t tmp = 0;
      uint16_t offset = i * factor;
      for(int j = 0; j < factor; j++){
        tmp += samples[(s+offset+j)%SAMPLES];
      }
      tmp = tmp / factor;
      data.y = (1-(tmp / analogDivider)) * resY -1;
      display.drawDynamicCurvePoint(i,0,resYm1,0,data);
    }
}

void setupADC(){
    //make analog read faster.
    REG_ADC_MR = (REG_ADC_MR & ~ADC_MR_STARTUP_Msk) | ADC_MR_STARTUP_SUT0; // setting startup time to 0 ADC clock cycles
    REG_ADC_MR = (REG_ADC_MR & ~ADC_MR_FREERUN) | ADC_MR_FREERUN_ON; //enable FREERUN mode
    analogReadResolution(12);
    
    //ulChannel = g_APinDescription[A0].ulADCChannelNumber;
    //analogChannel = g_APinDescription[ulPin].ulAnalogChannel;
    adc_enable_channel( ADC, static_cast<adc_channel_num_t>(g_APinDescription[A0].ulADCChannelNumber));
}

uint32_t customAnalogRead(){
    //this is a very simplified version of analogRead from wiring_analog.c
    //i preenabled the adc channel in setupADC, so you can only use one channel. this only works for the DUE!

    // Start the ADC
    adc_start( ADC );

    // Wait for end of conversion
    while ((adc_get_status(ADC) & ADC_ISR_DRDY) != ADC_ISR_DRDY){}

    // Read the value
    return adc_get_latest_value(ADC);
}