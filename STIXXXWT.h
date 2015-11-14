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

#ifndef STIXXXWT_h
#define STIXXXWT_h
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <stdint.h>
#include <simplemap.h>

typedef uint8_t standardTextType;
typedef uint8_t textEncoding;
typedef uint8_t fontType;
typedef uint8_t characterSize;
//typedef uint16_t color16;
typedef uint8_t baudrateConfig;
typedef uint8_t hardwareKeyboardType;
typedef uint8_t incomingCommand;

typedef uint8_t circleType;

struct color16 {
	uint16_t color;

	color16() {}

	color16(uint16_t value) : color(value) {}

	color16(const color16& other) : color(other.color) {}

	color16(uint8_t r, uint8_t g, uint8_t b) {
		uint8_t rr = (uint8_t)(r / 255.0 * 31);
		uint8_t gg = (uint8_t)(g / 255.0 * 63);
		uint8_t bb = (uint8_t)(b / 255.0 * 31);
		color = (rr << 11) | (gg << 5) | (bb);
	}
};

struct point {
	uint16_t x;
	uint16_t y;

	point() {}
	point(uint16_t x, uint16_t y) :x(x), y(y) {}
	point(const point& other) :x(other.x), y(other.y) {}
};

struct circle {
	circleType type;
	point center;
	uint8_t radius;

	circle() {}
	circle(circleType type, const point& center, uint8_t radius) :type(type), center(center), radius(radius) {}
	circle(const circle& other) :type(other.type), center(other.center), radius(other.radius) {}

	static const circleType drawInvertColor, drawForegroundColor, fillInvertColor, fillForegroundColor;
};

struct circleSegment {
	point center;
	uint16_t radius;
	uint16_t startAngle;
	uint16_t endAngle;

	circleSegment() {}
	circleSegment(point center, uint16_t radius, uint16_t startAngle, uint16_t endAngle) :center(center), radius(radius), startAngle(startAngle), endAngle(endAngle) {}
	circleSegment(const circleSegment& other) :center(other.center), radius(other.radius), startAngle(other.startAngle), endAngle(other.endAngle) {}
};

struct rectangle {
	point topLeft;
	point bottomRight;

	rectangle() {}
	rectangle(point topLeft, point bottomRight) :topLeft(topLeft), bottomRight(bottomRight) {}
	rectangle(uint16_t top, uint16_t left, uint16_t width, uint16_t height) :topLeft(left, top), bottomRight(left + width, top + height) {}
	rectangle(const rectangle& other) :topLeft(other.topLeft), bottomRight(other.bottomRight) {}

	uint16_t left() { return topLeft.x; }
	uint16_t top() { return topLeft.y; }
	uint16_t width() { return bottomRight.x - topLeft.x + 1; }
	uint16_t height() { return bottomRight.y - topLeft.y + 1; }
	uint16_t right() { return bottomRight.x; }
	uint16_t bottom() { return bottomRight.y; }

	bool contains(point p) {
		return p.x >= left() && p.x <= right() && p.y >= top() && p.y <= bottom();
	}
};

struct dynamicCurvePoint {
	uint16_t y;
	color16 color;

	dynamicCurvePoint() {}
	dynamicCurvePoint(uint16_t y, color16 color) :y(y), color(color) {}
	dynamicCurvePoint(const dynamicCurvePoint& other) :y(other.y), color(other.color) {}
};

struct shiftAreaData {
	rectangle area;
	uint16_t pixels;

	shiftAreaData() {}
	shiftAreaData(rectangle area, uint16_t pixels) :area(area), pixels(pixels) {}
	shiftAreaData(const shiftAreaData& other) :area(other.area), pixels(other.pixels) {}
};

class STIXXXWT
{
public:
	int32_t baudrate;
	HardwareSerial* serial;
	rectangle screenRectangle;
	color16 foregroundColor, backgroundColor;

	STIXXXWT();

	//methods
	void loop();
	void connect(HardwareSerial *serial, int32_t baudrate);
	void disconnect();

	void sendHandshake();

	//control display
	void setForeAndBackgroundColor(uint8_t fgred, uint8_t fggreen, uint8_t fgblue, uint8_t bgred, uint8_t bggreen, uint8_t bgblue);
	void setForeAndBackgroundColor(color16 foreground, color16 background);
	void setCharacterSpacing(int8_t colSpacing, int8_t lineSpacing);
	void pickBackgroundColorFromPixel(point p);
	void pickForegroundColorFromPixel(point p);
	void displayCursor(bool enabled, point leftTop, int8_t width, int8_t height);
	void displayStandardText(standardTextType type, point leftTop, char* stringascii, int16_t stringlen);
	void displayText(point leftTop, fontType font, bool useForegroundColor, bool useBackgroundColor, bool horizontalDisplay, bool recoverBackground, textEncoding encoding, characterSize size, color16 foreground, color16 background, char* utf8String);
	void setTextBox(point from, point to);
	void unsetTextBox();
	void drawPointsBackground(point *points, int8_t pointCount);
	void drawPointBackground(point p);
	void drawPointsForeground(point *points, int8_t pointCount);
	void drawPointForeground(point p);
	void drawDynamicCurvePoint(uint16_t x, uint16_t ys, uint16_t ye, color16 bg, dynamicCurvePoint point);
	void drawDynamicCurvePoints(uint16_t x, uint16_t ys, uint16_t ye, color16 bg, dynamicCurvePoint *points, uint8_t count);
	//void directDisplayOperation(); //also no clue 0x72 i think it is used to upload pictures to the display?
	void drawLinesForeground(point* points, int8_t pointCount);
	void drawLineForeground(point from, point to);
	void drawLinesBackground(point* points, int8_t pointCount);
	void drawLineBackground(point from, point to);
	void drawSpectrum(point leftBottom, uint8_t height, uint8_t *values, uint16_t valueCount);
	void drawSpectrum(point leftBottom, uint16_t height, uint16_t *values, uint16_t valueCount);
	void drawBrokenLine(uint16_t x, uint16_t xdif, uint16_t *ys, uint16_t yCount);
	void drawCircles(circle *circles, int8_t circleCount);
	void drawCircle(circle circle); //test this with the types to see what exactly they do!
	void drawCircleSegment(circleSegment segment);
	void drawRectanglesForeground(rectangle *rects, int8_t rectCount);
	void drawRectangleForeground(rectangle rect);
	void drawRectanglesBackground(rectangle *rects, int8_t rectCount);
	void drawRectangleBackground(rectangle rect);
	void fillRectanglesBackground(rectangle *rects, int8_t rectCount);
	void fillRectangleBackground(rectangle rect);
	void fillRectanglesForeground(rectangle *rects, int8_t rectCount);
	void fillRectangleForeground(rectangle rect);
	void invertRectangles(rectangle *rects, int8_t rectCount);
	void invertRectangle(rectangle rect);
	void fillAreaFromPoint(point p, color16 color);
	void clearScreen();
	void shiftAreaLeftCircular(shiftAreaData area);
	void shiftAreasLeftCircular(shiftAreaData *areas, uint16_t count);
	void shiftAreaRightCircular(shiftAreaData area);
	void shiftAreasRightCircular(shiftAreaData *areas, uint16_t count);
	void shiftAreaLeft(shiftAreaData area);
	void shiftAreasLeft(shiftAreaData *areas, uint16_t count);
	void shiftAreaRight(shiftAreaData area);
	void shiftAreasRight(shiftAreaData *areas, uint16_t count);
	void displayImage(uint16_t id);
	void displayImagePartAt(uint8_t imagePartDisplayType, uint16_t id, point sourceFrom, point sourceTo, point at); //have to test display types and what they do (71, 9c, 9d)
	void displayIcons(uint16_t *ids, point *ats, uint8_t count);
	void displayIcon(uint16_t id, point at);
	void saveCurrentScreen(uint16_t id);
	void redisplayLastImage();
	void turnBacklightOff();
	void startScreenSave();
	void turnBacklightToMaxBrightness();
	void setBacklightBrightness(int8_t brightness);
	void calibrateTouchPanel();
	void setWorkingMode(baudrateConfig baudrate, bool notifyOnTouchRelease, bool repeatTouchPressNotification, bool useTouchConfigFile, bool beepOnTouchPress, bool beepOnlyWhenValidAreaPressed, bool sendKeyDownWhenUsingTouchConfig, bool checkCRC, hardwareKeyboardType kbtype);

	void executeCommandPeriodically(uint8_t packID);
	void stopExecuteCommandPeriodically();

	/*void writeTemporaryBuffer(uint16_t address, uint16_t *data, uint8_t count);
	void readTemporaryBuffer(uint16_t address, uint8_t count);

	void writeUserMemoryRandom(uint16_t address, uint8_t *data, uint16_t count);
	void writeUserMemorySequential(uint32_t address, uint8_t *data, uint16_t count);
	void readUserMemory(uint32_t address, uint8_t *buffer, uint16_t count);*/

	void displayVirtualKeypad(point at, uint8_t maxStrLen, bool password, bool clearScreenWhenExit);

	//other
	void setCommandListener(incomingCommand, void(*function)(void* arg, uint8_t*, int16_t), void* arg);
	void unsetCommandListener(incomingCommand);

	//getter
	uint8_t getMajorVersion() { return majorVersion_; }
	uint8_t getMinorVersion() { return minorVersion_; }
	uint8_t getWorkingModeFlags() { return workingModeFlags_; }
	int16_t getResX() { return resX_; }
	int16_t getResY() { return resY_; }
	int16_t getLastDisplayedPicture() { return lastDisplayedPicture_; }

	//statics

	static const standardTextType ASCII8_16, ASCII16_32, ASCII6_12, ASCII12_24;
	static const textEncoding ASCII, GB2312, GBK, BIG5, SJIS, UTF16Unicode;
	static const fontType smallCustomFont0, smallCustomFont1, smallCustomFont2, smallCustomFont3, smallCustomFont4, smallCustomFont5, smallCustomFont6, smallCustomFont7;
	static const fontType bigCustomFont0, bigCustomFont1, bigCustomFont2, bigCustomFont3, bigCustomFont4, bigCustomFont5, bigCustomFont6, bigCustomFont7;
	static const fontType font6_12, font8_16, font12_24, font16_32, font24_48, font32_64, font48_96, font64_128;
	static const characterSize size6_12, size8_16, size12_24, size16_32, size20_40, size24_48, size28_58, size32_64, size12_12, size16_16, size24_24, size32_32, size40_40, size48_48, size56_56, size64_64, size40_80, size48_96, size56_112, size64_128, size80_80, size96_96, size112_112, size128_128, size6_8, size8_10, size8_12, size100_200, size200_200, size64_48;
	static const baudrateConfig br1200, br2400, br4800, br9600, br19200, br38400, br57600, br115200;
	static const hardwareKeyboardType matrix8_8, matrix4_4IO8, IO16, matrix4_8IO4;

	static const incomingCommand touchPressHold, touchRelease;

private:
	static const uint8_t messageHeader[];
	static const uint8_t messageEnd[];

	// status of software    
	int16_t readBytes;
	uint8_t inputBytes[514]; //514 is the max frame size according to the spec

	simplemap<uint8_t, void(*)(void*, uint8_t*, int16_t)> commandListeners;
	simplemap<uint8_t, void*> commandArgs;

	// status of display
	uint8_t majorVersion_, minorVersion_, workingModeFlags_;

	int16_t resX_, resY_, lastDisplayedPicture_;

	//methods
	void beginSendCommand(uint8_t command);
	void endSendCommand();
	void sendByte(uint8_t b);
	void sendShort(uint16_t s);
	void sendInt(uint32_t i);
	void send(uint8_t *data, uint16_t count);
	void send(uint16_t *data, uint16_t count);
	void send(uint32_t *data, uint16_t count);
	void send(point pt);
	void send(color16 c);
	void send(rectangle r);
	void send(circleSegment cs);
	void send(circle c);
	void send(dynamicCurvePoint dcp);

	//these are used by the shiftArea commands because all that changes direction is the command. so i unified this here
	void shiftArea(uint8_t command, shiftAreaData area);
	void shiftAreas(uint8_t command, shiftAreaData *areas, uint16_t count);

	void processInputCommand();
};

#endif

