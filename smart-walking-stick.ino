#include <Adafruit_LSM303DLH_Mag.h>
#include "Adafruit_LSM303_Accel.h"
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define PIXELPIN  3
// 0: Center; 1-6: Ring
#define NUMPIXELS 7

/* Click Configurations for CLICK_CFG_A
 * -----------------------------------------
 * | -- | -- | ZD | ZS | YD | YS | XD | XS |
 * -----------------------------------------
 * ZD: Double click Z axis
 * ZS: Single click Z axis
 * YD: Double click Y axis
 * YS: Single click Y axis
 * XD: Double click X axis
 * XS: Single click X axis
 */
#define  DOUBLE_Z   0b00100000
#define  SINGLE_Z   0b00010000
#define  DOUBLE_Y   0b00001000
#define  SINGLE_Y   0b00000100
#define  DOUBLE_X   0b00000010
#define  SINGLE_X   0b00000001
#define  DOUBLE_ALL 0b00101010
#define  SINGLE_ALL 0b00010101

/* Click Configurations for CLICK_SRC_A
 * -------------------------------------------------------
 * | -- | IA | DBCLICK | SCLICK | Sign |  Z  |  Y  |  X  |
 * -------------------------------------------------------
 * IA: Interrupt Active?
 * DBCLICK: Double Click Enabled?
 * SCLICK: Single Click Enabled?
 * Sign: (default: 0) 0 = Positive detect, 1 = Negative detect
 * Z: Z click detected?
 * Y: Y click detected?
 * X: X click detected?
 */
#define Z_DOUBLE_CLICK 0b01100100
#define Y_DOUBLE_CLICK 0b01100010
#define X_DOUBLE_CLICK 0b01100001

enum functionTypes {
  CALIBRATE,
  COMPASS,
  FLASHLIGHT_COOL,
  FLASHLIGHT_WARM,
  LIGHTNING_COOL,
  LIGHTNING_WARM,
  BREATH,
  RAINBOW,
  RAINBOW_BREATH,
  LAST
};

enum lightTemp {
  WARM,
  COOL
};

Adafruit_LSM303DLH_Mag_Unified lsm = Adafruit_LSM303DLH_Mag_Unified(12345);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_NeoPixel jewel = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_RGBW + NEO_KHZ800);

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
    animationAwake();
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
      igniteFlashlight(COOL);
      break;
    case FLASHLIGHT_WARM:
      igniteFlashlight(WARM);
      break;
    case LIGHTNING_COOL:
      break;
    case LIGHTNING_WARM:
      break;
    case BREATH:
      break;
    case RAINBOW:
      break;
    case RAINBOW_BREATH:
      break;
  }
}

void tapDetect() {
  bool source = accel.getTapped() == Y_DOUBLE_CLICK;

  if (TappedState != source && !TappedState) {
    rotateFunction();
  }

  TappedState = source;
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
 * ANIMATIONS
 */
void animationAwake() {
  for(uint16_t i = 1; i<jewel.numPixels(); i++) {
    jewel.clear();
    jewel.setPixelColor(i, jewel.Color(0, 55, 0));
    jewel.show();
    delay(100);
  }
}

/*
 * FLASHLIGHTS
 */
void igniteFlashlight(lightTemp temp) {
  //Serial.println("Flashlight");
  switch (temp) {
    case WARM:
      for(uint16_t i = 0; i<jewel.numPixels(); i++) {
        jewel.setPixelColor(i, jewel.Color(255, 255, 255, 255));
      }
      break;
    case COOL:
      for(uint16_t i = 0; i<jewel.numPixels(); i++) {
        jewel.setPixelColor(i, jewel.Color(255, 255, 255));
      }
      break;
  }
  jewel.show();
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
