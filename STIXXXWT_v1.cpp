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

const uint8_t STIXXXWT::messageHeader[] = {0xAA};
const uint8_t STIXXXWT::messageEnd[] = {0xCC,0x33,0xC3,0x3C};

const standardTextType STIXXXWT::ASCII8_16 = 0x54,STIXXXWT::ASCII16_32 = 0x55,STIXXXWT::ASCII6_12 = 0x6E,STIXXXWT::ASCII12_24 = 0x6F;
const textEncoding STIXXXWT::ASCII = 0x00,STIXXXWT::GB2312 = 0x01,STIXXXWT::GBK = 0x02,STIXXXWT::BIG5 = 0x03,STIXXXWT::SJIS = 0x04,STIXXXWT::UTF16Unicode = 0x05;
const fontType STIXXXWT::smallCustomFont0 = 0x00,STIXXXWT::smallCustomFont1 = 0x01,STIXXXWT::smallCustomFont2 = 0x02,STIXXXWT::smallCustomFont3 = 0x03,STIXXXWT::smallCustomFont4 = 0x04,STIXXXWT::smallCustomFont5 = 0x05,STIXXXWT::smallCustomFont6 = 0x06,STIXXXWT::smallCustomFont7 = 0x07;
const fontType STIXXXWT::bigCustomFont0 = 0x08,STIXXXWT::bigCustomFont1 = 0x09,STIXXXWT::bigCustomFont2 = 0x0A,STIXXXWT::bigCustomFont3 = 0x0B,STIXXXWT::bigCustomFont4 = 0x0C,STIXXXWT::bigCustomFont5 = 0x0D,STIXXXWT::bigCustomFont6 = 0x0E,STIXXXWT::bigCustomFont7 = 0x0F;
const fontType STIXXXWT::font6_12 = 0x20,STIXXXWT::font8_16 = 0x21,STIXXXWT::font12_24 = 0x22,STIXXXWT::font16_32 = 0x23,STIXXXWT::font24_48 = 0x24,STIXXXWT::font32_64 = 0x25,STIXXXWT::font48_96 = 0x26,STIXXXWT::font64_128 = 0x27;
const characterSize STIXXXWT::size6_12 = 0x01,STIXXXWT::size8_16 = 0x02,STIXXXWT::size12_24 = 0x03,STIXXXWT::size16_32 = 0x04,STIXXXWT::size20_40 = 0x05,STIXXXWT::size24_48 = 0x06,STIXXXWT::size28_58 = 0x07,STIXXXWT::size32_64 = 0x08,STIXXXWT::size12_12 = 0x09,STIXXXWT::size16_16 = 0x0A,STIXXXWT::size24_24 = 0x0B,STIXXXWT::size32_32 = 0x0C,STIXXXWT::size40_40 = 0x0D,STIXXXWT::size48_48 = 0x0E,STIXXXWT::size56_56 = 0x0F,STIXXXWT::size64_64 = 0x10,STIXXXWT::size40_80 = 0x11,STIXXXWT::size48_96 = 0x12,STIXXXWT::size56_112 = 0x13,STIXXXWT::size64_128 = 0x14,STIXXXWT::size80_80 = 0x15,STIXXXWT::size96_96 = 0x16,STIXXXWT::size112_112 = 0x17,STIXXXWT::size128_128 = 0x18,STIXXXWT::size6_8 = 0x19,STIXXXWT::size8_10 = 0x1A,STIXXXWT::size8_12 = 0x1B,STIXXXWT::size100_200 = 0x1C,STIXXXWT::size200_200 = 0x1D,STIXXXWT::size64_48 = 0x1E;
const baudrateConfig STIXXXWT::br1200 = 0x00,STIXXXWT::br2400 = 0x01,STIXXXWT::br4800 = 0x02,STIXXXWT::br9600 = 0x03,STIXXXWT::br19200 = 0x04,STIXXXWT::br38400 = 0x05,STIXXXWT::br57600 = 0x06,STIXXXWT::br115200 = 0x07;
const hardwareKeyboardType STIXXXWT::matrix8_8 = 0x00,STIXXXWT::matrix4_4IO8 = 0x01,STIXXXWT::IO16 = 0x02,STIXXXWT::matrix4_8IO4 = 0x03;

const incomingCommand STIXXXWT::touchPressHold = 0x73, STIXXXWT::touchRelease = 0x72;

const circleType circle::drawInvertColor = 0x00, circle::drawForegroundColor = 0x01, circle::fillInvertColor = 0x02, circle::fillForegroundColor = 0x03; //0x04 is circle segment, but not compatible with the normal circles

/*color16 STIXXXWT::convert24bitColorTo16bitColor(uint8_t r, uint8_t g, uint8_t b){
    uint8_t rr = (uint8_t)(r / 255.0 * 31);
    uint8_t gg = (uint8_t)(g / 255.0 * 63);
    uint8_t bb = (uint8_t)(b / 255.0 * 31);
    uint16_t ret = (rr << 11) | (gg << 5) | (bb);
    return ret;
}*/

STIXXXWT::STIXXXWT()
{
    
}

void STIXXXWT::checkSerial(){
    int avail = serial->available();
    for(int i =0; i< avail; i++){
        inputBytes[readBytes++] = serial->read();
        if(readBytes >514){ //this shouldnt happen. but if it does we dont want the code to write somewhere in the memory or kill the mcu
            readBytes = 0; 
        }else if(readBytes >=6){
            if(inputBytes[readBytes-1] == 0x3C && inputBytes[readBytes-2] == 0xC3 && inputBytes[readBytes-3] == 0x33 && inputBytes[readBytes-4] == 0xCC){ //frame end detected
                //parse message
                processInputCommand();
                readBytes = 0; //start reading input again
            }
        }
    }
}

void STIXXXWT::connect(HardwareSerial *serial_,int32_t baudrate_){
    serial = serial_;
    baudrate = baudrate_;
    serial->begin(baudrate);
}

void STIXXXWT::disconnect(){
    serial->end();
}

void STIXXXWT::sendHandshake(){
    //sendCommand(0,0,0);
    beginSendCommand(0x00);
    endSendCommand();
}

void STIXXXWT::setForeAndBackgroundColor(uint8_t fgred,uint8_t fggreen,uint8_t fgblue,uint8_t bgred,uint8_t bggreen,uint8_t bgblue){
    setForeAndBackgroundColor(color16(fgred,fggreen,fgblue),color16(bgred,bggreen,bgblue));
}

void STIXXXWT::setForeAndBackgroundColor(color16 foreground,color16 background){
    /*uint16_t data[] = {foreground,background};
    foregroundColor = foreground;
    backgroundColor = background;
    sendCommand(0x40,(uint8_t*)data,sizeof(data));*/
    beginSendCommand(0x40);
    send(foreground);
    send(background);
    endSendCommand();
}

void STIXXXWT::setCharacterSpacing(int8_t colSpacing, int8_t lineSpacing){
    //uint8_t data[] = {colSpacing,lineSpacing};
    //sendCommand(0x41,data,sizeof(data));
    beginSendCommand(0x41);
    sendByte(colSpacing);
    sendByte(lineSpacing);
    endSendCommand();
}

void STIXXXWT::pickBackgroundColorFromPixel(point p){
    //sendCommand(0x42,(uint8_t*)&p,sizeof(p));
    beginSendCommand(0x42);
    send(p);
    endSendCommand();
}

void STIXXXWT::pickForegroundColorFromPixel(point p){
    //sendCommand(0x43,(uint8_t*)&p,sizeof(p));
    beginSendCommand(0x43);
    send(p);
    endSendCommand();
}

void STIXXXWT::displayCursor(bool enabled, point leftTop, int8_t width, int8_t height){
    /*uint8_t *ltp = (uint8_t*)&leftTop;
    uint8_t en = enabled?1:0;
    uint8_t data[] = {en,ltp[0],ltp[1],ltp[2],ltp[3],width,height};
    sendCommand(0x44,data,sizeof(data));*/
    beginSendCommand(0x44);
    sendByte(enabled?1:0);
    send(leftTop);
    sendByte(width);
    sendByte(height);
    endSendCommand();
}

void STIXXXWT::displayStandardText(standardTextType type,point leftTop, char* stringascii, int16_t stringlen){
    /*uint8_t *ltp = (uint8_t*)&leftTop;
    uint8_t *data = new uint8_t[4+stringlen];
    data[0] = ltp[0];
    data[1] = ltp[1];
    data[2] = ltp[2];
    data[3] = ltp[3];
    data += 4;
    for(int i =0; i< stringlen;i++){
        data[i] = (uint8_t)stringascii[i];
    }
    data-=4;
    sendCommand(type,data,4+stringlen);
    delete[] data;*/
    beginSendCommand(type);
    send(leftTop);
    for(int i =0; i< stringlen;i++){
        sendByte(stringascii[i]);
    }
    endSendCommand();
}

void STIXXXWT::displayText(point leftTop, fontType font, bool useForegroundColor, bool useBackgroundColor, bool horizontalDisplay, bool recoverBackground, textEncoding encoding, characterSize size, color16 foreground, color16 background, wchar_t* string, int16_t stringlen){
    /*uint8_t *ltp = (uint8_t*)&leftTop;
    uint8_t *data = new uint8_t[11+stringlen];
    data[0] = ltp[0];
    data[1] = ltp[1];
    data[2] = ltp[2];
    data[3] = ltp[3];
    data[4] = font;
    data[5] = (useForegroundColor?128:0) | (useBackgroundColor?64:0) | (horizontalDisplay?32:0) | (recoverBackground?16:0) | encoding;
    data[6] = size;
    uint8_t *fgp = (uint8_t*)&foreground, *bgp = (uint8_t*)&background;
    data[7] = fgp[0];
    data[8] = fgp[1];
    data[9] = bgp[0];
    data[10] = bgp[1];
    data+=11;
    uint8_t *str = (uint8_t*)string;
    for(int i = 0; i < stringlen*2; i++){
        data[i] = str[i];
    }
    data-=11;
    sendCommand(0x98,data,11+stringlen*2);*/
    beginSendCommand(0x98);
    send(leftTop);
    sendByte(font);
    sendByte((useForegroundColor?128:0) | (useBackgroundColor?64:0) | (horizontalDisplay?32:0) | (recoverBackground?16:0) | encoding);
    sendByte(size);
    send(foreground);
    send(background);
    uint8_t *str = (uint8_t*)string;
    for(int i =0; i< stringlen*2;i++){
        sendByte(str[i]);
    }
    endSendCommand();
}

void STIXXXWT::setTextBox(point from, point to){
    //point data[] = {from,to};
    //sendCommand(0x45,(uint8_t*)data,sizeof(data));
    beginSendCommand(0x45);
    send(from);
    send(to);
    endSendCommand();
}

void STIXXXWT::unsetTextBox(){
    //uint8_t data[] = {0x00};
    //sendCommand(0x45,data,sizeof(data));
    beginSendCommand(0x45);
    sendByte(0);
    endSendCommand();
}

void STIXXXWT::drawPointsBackground(point *points, int8_t pointCount){
    /*uint16_t *data = new uint16_t[count*2];
    for(int i =0; i< count;i++){
        data[i*2] = xs[i];
        data[i*2+1] = ys[i];
    }*/
    //sendCommand(0x50,(uint8_t*)points,pointCount*4);
    //delete[] data;
    beginSendCommand(0x50);
    for(int i =0; i< pointCount;i++){
        send(points[i]);
    }
    endSendCommand();
}

void STIXXXWT::drawPointBackground(point p){
    //sendCommand(0x50,(uint8_t*)&p, sizeof(p));
    beginSendCommand(0x50);
    send(p);
    endSendCommand();
}

void STIXXXWT::drawPointsForeground(point *points, int8_t pointCount){
    //sendCommand(0x51,(uint8_t*)points,pointCount*4);
    beginSendCommand(0x51);
    for(int i =0; i< pointCount;i++){
        send(points[i]);
    }
    endSendCommand();
}

void STIXXXWT::drawPointForeground(point p){
    //sendCommand(0x51,(uint8_t*)&p, sizeof(p));
    beginSendCommand(0x51);
    send(p);
    endSendCommand();
}

void STIXXXWT::drawDynamicCurvePoint(uint16_t x, uint16_t ys, uint16_t ye, color16 bg, dynamicCurvePoint point){
    /*uint16_t data[] = {x,ys,ye,bg,0,0};
    data[4] = point.y;
    data[5] = point.color;
    sendCommand(0x74,(uint8_t*)data,sizeof(data));*/
    beginSendCommand(0x74);
    sendShort(x);
    sendShort(ys);
    sendShort(ye);
    send(bg);
    send(point);
    endSendCommand();
}

void STIXXXWT::drawDynamicCurvePoints(uint16_t x, uint16_t ys, uint16_t ye, color16 bg, dynamicCurvePoint *points, uint8_t count){
    /*uint16_t *data = new uint16_t[4+2*count];
    data[0] = x;
    data[1] = ys;
    data[2] = ye;
    data[3] = bg;
    memcpy(data+4,points,sizeof(dynamicCurvePoint)*count);
    sendCommand(0x74,(uint8_t*)data,(4+2*count)*2);
    delete[] data;*/
    beginSendCommand(0x74);
    sendShort(x);
    sendShort(ys);
    sendShort(ye);
    send(bg);
    for(int i =0; i < count; i++){
        send(points[i]);
    }
    endSendCommand();
}

void STIXXXWT::drawLinesForeground(point* points, int8_t pointCount){
    //sendCommand(0x56,(uint8_t*)points, pointCount * sizeof(point));
    beginSendCommand(0x56);
    for(int i =0; i< pointCount;i++){
        send(points[i]);
    }
    endSendCommand();
}

void STIXXXWT::drawLineForeground(point from, point to){
    /*point points[] = {from,to};
    sendCommand(0x56,(uint8_t*)points, sizeof(points));*/
    beginSendCommand(0x56);
    send(from);
    send(to);
    endSendCommand();
}

void STIXXXWT::drawLinesBackground(point* points, int8_t pointCount){
    //sendCommand(0x5D,(uint8_t*)points, pointCount * sizeof(point));
    beginSendCommand(0x5D);
    for(int i =0; i< pointCount;i++){
        send(points[i]);
    }
    endSendCommand();
}

void STIXXXWT::drawLineBackground(point from, point to){
    /*point points[] = {from,to};
    sendCommand(0x5D,(uint8_t*)points, sizeof(points));*/
    beginSendCommand(0x5D);
    send(from);
    send(to);
    endSendCommand();
}

void STIXXXWT::drawSpectrum(point leftBottom, uint8_t height, uint8_t *values, uint16_t valueCount){
    beginSendCommand(0x75);
    send(leftBottom);
    sendByte(height);
    send(values,valueCount);
    endSendCommand();
}

void STIXXXWT::drawSpectrum(point leftBottom, uint16_t height, uint16_t *values, uint16_t valueCount){
    beginSendCommand(0x75);
    send(leftBottom);
    sendByte(0);
    sendShort(height);
    send(values,valueCount);
    endSendCommand();
}

void STIXXXWT::drawBrokenLine(uint16_t x, uint16_t xdif, uint16_t *ys, uint16_t yCount){
    beginSendCommand(0x76);
    sendShort(x);
    sendShort(xdif);
    send(ys,yCount);
    endSendCommand();
}

void STIXXXWT::drawCircles(circle *circles, int8_t circleCount){
    //sendCommand(0x57,(uint8_t*)circles, circleCount * sizeof(circle));
    beginSendCommand(0x57);
    for(int i=0; i< circleCount;i++){
        send(circles[i]);
    }
    endSendCommand();
}

void STIXXXWT::drawCircle(circle circle){
    //sendCommand(0x57,(uint8_t*)&circle, sizeof(circle));
    beginSendCommand(0x57);
    send(circle);
    endSendCommand();
}

void STIXXXWT::drawCircleSegment(circleSegment segment){
    /*uint8_t data[] = {0x04,0,0,0,0,0,0,0,0,0,0};
    circleSegment *cs = (circleSegment*)data+1;
    cs[0] = segment;
    sendCommand(0x57,data, sizeof(data));*/
    beginSendCommand(0x57);
    sendByte(0x04);
    send(segment);
    endSendCommand();
}

void STIXXXWT::drawRectanglesForeground(rectangle *rects, int8_t rectCount){
    //sendCommand(0x59,(uint8_t*)rects,rectCount * sizeof(rectangle));
    beginSendCommand(0x59);
    for(int i=0; i< rectCount;i++){
        send(rects[i]);
    }
    endSendCommand();
}

void STIXXXWT::drawRectangleForeground(rectangle rect){
    //sendCommand(0x59,(uint8_t*)&rect,sizeof(rectangle));
    beginSendCommand(0x59);
    send(rect);
    endSendCommand();
}

void STIXXXWT::drawRectanglesBackground(rectangle *rects, int8_t rectCount){
    //sendCommand(0x69,(uint8_t*)rects,rectCount * sizeof(rectangle));
    beginSendCommand(0x69);
    for(int i=0; i< rectCount;i++){
        send(rects[i]);
    }
    endSendCommand();
}

void STIXXXWT::drawRectangleBackground(rectangle rect){
    //sendCommand(0x69,(uint8_t*)&rect,sizeof(rectangle));
    beginSendCommand(0x69);
    send(rect);
    endSendCommand();
}

void STIXXXWT::fillRectanglesBackground(rectangle *rects, int8_t rectCount){
    //sendCommand(0x5A,(uint8_t*)rects,rectCount * sizeof(rectangle));
    beginSendCommand(0x5A);
    for(int i=0; i< rectCount;i++){
        send(rects[i]);
    }
    endSendCommand();
}

void STIXXXWT::fillRectangleBackground(rectangle rect){
    //sendCommand(0x5A,(uint8_t*)&rect,sizeof(rectangle));
    beginSendCommand(0x5A);
    send(rect);
    endSendCommand();
}

void STIXXXWT::fillRectanglesForeground(rectangle *rects, int8_t rectCount){
    //sendCommand(0x5B,(uint8_t*)rects,rectCount * sizeof(rectangle));
    beginSendCommand(0x5B);
    for(int i=0; i< rectCount;i++){
        send(rects[i]);
    }
    endSendCommand();
}

void STIXXXWT::fillRectangleForeground(rectangle rect){
    //sendCommand(0x5B,(uint8_t*)&rect,sizeof(rectangle));
    beginSendCommand(0x5B);
    send(rect);
    endSendCommand();
}

void STIXXXWT::invertRectangles(rectangle *rects, int8_t rectCount){
    //sendCommand(0x5C,(uint8_t*)rects,rectCount * sizeof(rectangle));
    beginSendCommand(0x5C);
    for(int i=0; i< rectCount;i++){
        send(rects[i]);
    }
    endSendCommand();
}

void STIXXXWT::invertRectangle(rectangle rect){
    //sendCommand(0x5C,(uint8_t*)&rect,sizeof(rectangle));
    beginSendCommand(0x5C);
    send(rect);
    endSendCommand();
}

void STIXXXWT::fillAreaFromPoint(point p,color16 color){
    //uint16_t data[] = {p.x,p.y,color};
    //sendCommand(0x64,(uint8_t*)data,sizeof(data));
    beginSendCommand(0x64);
    send(p);
    send(color);
    endSendCommand();
}

void STIXXXWT::clearScreen(){
    //sendCommand(0x52,0,0);
    beginSendCommand(0x52);
    endSendCommand();
}

void STIXXXWT::shiftAreaLeftCircular(shiftAreaData area){
    shiftArea(0x60,area);
}

void STIXXXWT::shiftAreasLeftCircular(shiftAreaData *areas, uint16_t count){
    shiftAreas(0x60,areas,count);
}

void STIXXXWT::shiftAreaRightCircular(shiftAreaData area){
    shiftArea(0x61,area);
}

void STIXXXWT::shiftAreasRightCircular(shiftAreaData *areas, uint16_t count){
    shiftAreas(0x61,areas,count);
}

void STIXXXWT::shiftAreaLeft(shiftAreaData area){
    shiftArea(0x62,area);
}

void STIXXXWT::shiftAreasLeft(shiftAreaData *areas, uint16_t count){
    shiftAreas(0x62,areas,count);
}

void STIXXXWT::shiftAreaRight(shiftAreaData area){
    shiftArea(0x63,area);
}

void STIXXXWT::shiftAreasRight(shiftAreaData *areas, uint16_t count){
    shiftAreas(0x63,areas,count);
}

void STIXXXWT::displayImage(uint16_t id){
    beginSendCommand(0x70);
    sendShort(id);
    endSendCommand();
    //sendCommand(0x70,(uint8_t*)&id,sizeof(id));
}

void STIXXXWT::displayImagePartAt(uint8_t imagePartDisplayType, uint16_t id, point sourceFrom,point sourceTo,point at){
    //uint16_t data[] = {id,sourceFrom.x,sourceFrom.y,sourceTo.x,sourceTo.y,at.x,at.y};
    beginSendCommand(imagePartDisplayType);
    sendShort(id);
    send(sourceFrom);
    send(sourceTo);
    send(at);
    //send(data,7);
    endSendCommand();
    //sendCommand(imagePartDisplayType,(uint8_t*)data,sizeof(data));
}

void STIXXXWT::displayIcons(uint16_t *ids, point *ats, uint8_t count){
    beginSendCommand(0x99);
    for(int i =0; i< count; i++){
        send(ats[i]);
        send(ids[i]);
    }
    endSendCommand();
    /*uint16_t *data = new uint16_t[count*3];
    for(int i =0; i< count;i++){
        data[i*3] = ats[i].x;
        data[i*3+1] = ats[i].y;
        data[i*3+2] = ids[i];
    }
    sendCommand(0x99,(uint8_t*)data, count*3);
    delete[] data;*/
}

void STIXXXWT::displayIcon(uint16_t id, point at){
    //uint16_t data[] = {at.x,at.y,id};
    //sendCommand(0x99,(uint8_t*)data, sizeof(data));
    beginSendCommand(0x99);
    send(at);
    sendShort(id);
    //send(data,3);
    endSendCommand();
}

void STIXXXWT::saveCurrentScreen(uint16_t id){
    beginSendCommand(0xE2);
    sendShort(id);
    endSendCommand();
}

void STIXXXWT::turnBacklightOff(){
    beginSendCommand(0x5E);
    endSendCommand();
}

void STIXXXWT::turnBacklightToMaxBrightness(){
    beginSendCommand(0x5F);
    endSendCommand();
}

void STIXXXWT::setBacklightBrightness(int8_t brightness){
    beginSendCommand(0X5F);
    sendByte(brightness);
    endSendCommand();
}

void STIXXXWT::calibrateTouchPanel(){
    uint8_t data[] = {0x55, 0xAA, 0x5A, 0xA5};
    beginSendCommand(0xE4);
    send(data,sizeof(data));
    endSendCommand();
}

void STIXXXWT::setWorkingMode(baudrateConfig baudrate,bool notifyOnTouchRelease,bool repeatTouchPressNotification,bool useTouchConfigFile,bool beepOnTouchPress,bool beepOnlyWhenValidAreaPressed,bool sendKeyDownWhenUsingTouchConfig,bool checkCRC,hardwareKeyboardType kbtype){
    uint8_t para1 = (notifyOnTouchRelease?0:128) | (repeatTouchPressNotification?0:64) | (useTouchConfigFile?32:0) | (beepOnTouchPress?0:8) | (beepOnlyWhenValidAreaPressed?2:0) | (sendKeyDownWhenUsingTouchConfig?1:0);
    uint8_t para2 = (checkCRC?8:0) | kbtype;
    uint8_t data[] = {baudrate,para1,para2};
    beginSendCommand(0xE0);
    send(data,sizeof(data));
    endSendCommand();
    //sendCommand(0xE0,data,sizeof(data));
}

void STIXXXWT::setCommandListener(incomingCommand cmd,void (*function)(uint8_t*, int16_t)){
    commandListeners.set(cmd,function);
}

void STIXXXWT::unsetCommandListener(incomingCommand cmd){
    commandListeners.set(cmd,0);
}

void STIXXXWT::processInputCommand(){
    uint8_t command = inputBytes[1];
    switch(command){
        case 0x00:
            majorVersion_ = inputBytes[6];
            minorVersion_ = inputBytes[8];
            uint8_t displayType = inputBytes[9];
            switch(displayType){
                case 0x00: //3.5" 320x240
                    resX_ = 320;
                    resY_ = 240;
                    break;
                case 0x01: //4.3" 480x272
                    resX_ = 480;
                    resY_ = 272;
                    break;
                case 0x02: //5" 640x480
                case 0x03: //5.6"  640x480
                    resX_ = 640;
                    resY_ = 480;
                    break;
                case 0x04: //7" 800x480
                    resX_ = 800;
                    resY_ = 480;
                    break;
                case 0x05: //8" 800x600
                case 0x06: //10.4" 800x600
                case 0x07: //12.1" 800x600
                    resX_ = 800;
                    resY_ = 600;
                    break;
                case 0x08: //15.1" 1024x768
                    resX_ = 1024;
                    resY_ = 768;
                    break;
            }
            screenRectangle.bottomRight.x = resX_-1;
            screenRectangle.bottomRight.y = resY_-1;
            //uint8_t configuredBaudrate = inputBytes[10]; //well if we can read this message we already know the correct baudrate eh?
            workingModeFlags_ = inputBytes[11];
            lastDisplayedPicture_ = *(int16_t*)inputBytes+12;
            //For example: AA 00 
            //4F 4B 5F 56  04  2E  06  03  07  2A  00  00
            // O  K  _  V x04   . x06 x03 x07 x2A x00 x00
            //CC 33 C3 3C (5.6’’ terminal with V4.6 version)
        break;
    }
    void (*listener)(uint8_t*, int16_t) = commandListeners.get(command);
    if(listener != 0){
        listener(inputBytes,readBytes);
    }
}

void STIXXXWT::beginSendCommand(uint8_t command){
    serial->write(messageHeader,sizeof(messageHeader));
    serial->write(command);
}

void STIXXXWT::endSendCommand(){
    serial->write(messageEnd,sizeof(messageEnd));
}

void STIXXXWT::sendByte(uint8_t b){
    serial->write(b);
}

void STIXXXWT::sendShort(uint16_t s){
    uint8_t *data = (uint8_t*)&s;
    serial->write(data[1]);
    serial->write(data[0]);
}

void STIXXXWT::sendInt(uint32_t i){
    uint8_t *data = (uint8_t*)&i;
    serial->write(data[3]);
    serial->write(data[2]);
    serial->write(data[1]);
    serial->write(data[0]);
}

void STIXXXWT::send(uint8_t* data, uint16_t count){
    serial->write(data,count);
}

void STIXXXWT::send(uint16_t* data, uint16_t count){
    for(int i = 0; i< count; i++){
        sendShort(data[i]);
    }
}

void STIXXXWT::send(uint32_t* data, uint16_t count){
    for(int i = 0; i< count; i++){
        sendInt(data[i]);
    }
}

void STIXXXWT::send(point pt){
    sendShort(pt.x);
    sendShort(pt.y);
}

void STIXXXWT::send(color16 c){
    sendShort(c.color);
}

void STIXXXWT::send(rectangle r){
    send(r.topLeft);
    send(r.bottomRight);
}

void STIXXXWT::send(circleSegment cs){
    send(cs.center);
    sendShort(cs.radius);
    sendShort(cs.startAngle);
    sendShort(cs.endAngle);
}

void STIXXXWT::send(circle c){
    sendByte(c.type);
    send(c.center);
    sendByte(c.radius);
}

void STIXXXWT::send(dynamicCurvePoint dcp){
    sendShort(dcp.y);
    send(dcp.color);
}

void STIXXXWT::shiftArea(uint8_t command, shiftAreaData area){
    beginSendCommand(command);
    send(area.area);
    sendShort(area.pixels);
    endSendCommand();
}

void STIXXXWT::shiftAreas(uint8_t command, shiftAreaData *areas, uint16_t count){
    beginSendCommand(command);
    for(int i = 0; i < count; i++){
        send(areas[i].area);
        sendShort(areas[i].pixels);
    }
    endSendCommand();
}

/*void STIXXXWT::sendCommand(uint8_t command, uint8_t* data, int16_t datalen){
    serial->write(messageHeader,sizeof(messageHeader));
    serial->write(command);
    serial->write(data,datalen);
    serial->write(messageEnd,sizeof(messageEnd));
}*/
 


