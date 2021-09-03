#include <Adafruit_LSM303DLH_Mag.h>
#include "Adafruit_LSM303_Accel.h"
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "LSM303_Config.h"
#include "Animation.h"

#define PIXELPIN  3
// 0: Center; 1-6: Ring
#define NUMPIXELS 7

Adafruit_LSM303DLH_Mag_Unified lsm = Adafruit_LSM303DLH_Mag_Unified(12345);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_NeoPixel jewel = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_RGBW + NEO_KHZ800);
Animation animation = Animation(tapDetect);

float XMin = -69.09;
float XMax = 46.09;
float YMin = -98;
float YMax = 0.64;
float ZMin = -122.65;
float ZMax = 0;




bool TappedState = false;
functionTypes FunctionState = FLASHLIGHT_WARM; // Initial Function

void setup() {
  jewel.begin();
  jewel.show();
  //Max = 255
  jewel.setBrightness(255);

  Serial.begin(115200);
  
  if(lsm.begin()) {
    animation.awaken(jewel);
  }
  if (!accel.begin()) {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1)
      ;
  }

  accel.setRange(LSM303_RANGE_8G);
  accel.setMode(LSM303_MODE_LOW_POWER);
  accel.setTap(3, 20, 10, 20, 20, DOUBLE_Y);
}

void loop() {
  tapDetect();

  //sensors_event_t accelEvent;
  //accel.getEvent(&accelEvent);

  executeFunction();
}

void executeFunction() {
  //Serial.print("Execute: ");
  //Serial.println(FunctionState);
  
  switch (FunctionState) {
    case CALIBRATE:
      break;
    case COMPASS:
      sensors_event_t magEvent;
      lsm.getEvent(&magEvent);
      setCompass(magEvent);
      break;
    case FLASHLIGHT_COOL:
      animation.igniteFlashlight(jewel, COOL);
      break;
    case FLASHLIGHT_WARM:
      animation.igniteFlashlight(jewel, WARM);
      break;
    case LIGHTNING_COOL:
      animation.lightning(jewel);
      break;
    case LIGHTNING_WARM:
      animation.cracklingLightning(jewel);
      break;
    case BREATH:
      animation.breath(jewel, BLUE);
      break;
    case RAINBOW:
      animation.cycleHue(jewel);
      break;
    case RAINBOW_BREATH:
      animation.rainbowLightning(jewel);
      break;
    case OFF:
      jewel.clear();
      jewel.show();
      break;
  }
}

bool tapDetect() {
  bool source = accel.getTapped() == Y_DOUBLE_CLICK;
  bool tapped = false;

  if (TappedState != source && !TappedState) {
    rotateFunction();
    tapped = true;
  }

  TappedState = source;
  
  return tapped;
}

void rotateFunction() {
  int currFunction = FunctionState;

  currFunction++;
  
  if (currFunction == (int)LAST) {
    currFunction = 1; //skip Calibrate as an option for now
  }

  FunctionState = (functionTypes)currFunction;
  Serial.print("Rotate to: ");
  Serial.println(FunctionState);
}

/*
 * COMPASS ROUTINES
 */

void setCompass(sensors_event_t magEvent) {
  float Pi = 3.14159;
  float XMean = (XMax + XMin)/2;
  float YMean = (YMax + YMin)/2;
  float ZMean = (ZMax + ZMin)/2;
  float XLeng = (XMax - XMin)/2;
  float YLeng = (YMax - YMin)/2;
  float ZLeng = (ZMax - ZMin)/2;
  float XN = (magEvent.magnetic.x - XMean)/XLeng;
  float YN = (magEvent.magnetic.y - YMean)/YLeng;
  float ZN = (magEvent.magnetic.z - ZMean)/ZLeng;
  float AN = sqrt(pow(XN,2)+pow(YN,2)+pow(ZN,2));
  float heading = (atan2(ZN, XN) * 180) / Pi;
  float northHeading = 0;

  if( heading < 0 ) {
    heading += 360;
  }
  if( heading > 360 ) {
    heading -= 360;
  }

  northHeading = invertPixel(heading / 60 + 1);

  lightCompass(northHeading);

  //Serial.print("Compass Heading: ");
  //Serial.println(heading);
  //Serial.print ("Pixel: ");
  //Serial.println(northHeading);
  //delay(100);
}

void lightCompass(float currPixelHeading) {
  float i;
  float k;
  if( currPixelHeading >= 1 && currPixelHeading < 1.5 ) {
    i = 1;
    k = 4;
  }
  if( currPixelHeading >= 1.5 && currPixelHeading < 2.5 ) {
    i = 2;
    k = 5;
  }
  if( currPixelHeading >= 2.5 && currPixelHeading < 3.5 ) {
    i = 3;
    k = 6;
  }
  if( currPixelHeading >= 3.5 && currPixelHeading < 4.5 ) {
    i = 4;
    k = 1;
  }
  if( currPixelHeading >= 4.5 && currPixelHeading < 5.5 ) {
    i = 5;
    k = 2;
  }
  if( currPixelHeading >= 5.5 && currPixelHeading < 6.5 ) {
    i = 6;
    k = 3;
  }
  if( currPixelHeading > 6.5 ) {
    i = 1;
    k = 4;
  }
  for(uint16_t j = 1; j<jewel.numPixels(); j++) {
    if( j == i ) {
      //North indicator Red
      jewel.setPixelColor(j, jewel.Color(0, 255, 0));
    } else if( j == k ) {
      jewel.setPixelColor(k, jewel.Color(0, 0, 255));
    } else {
      //Everything else off
      jewel.setPixelColor(j, jewel.Color(0, 0, 0));
    }
    jewel.show();
  }
}

/*
 * UTILITIES
 */

float invertPixel(float currPixelHeading) {
  //Pole Shift
  for(int i = 1; i<4; i++) {
    if(currPixelHeading + 1 >= 7) {
      currPixelHeading -= 6;
    } else {
      currPixelHeading += 1;
    }
  }

  return currPixelHeading;
}
