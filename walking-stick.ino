#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define PIXELPIN  3
#define NUMPIXELS 10

Adafruit_LSM303DLH_Mag_Unified lsm = Adafruit_LSM303DLH_Mag_Unified(12345);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_NeoPixel jewel = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_RGBW + NEO_KHZ800);

float XMin = -69.09;
float XMax = 46.09;
float YMin = -98;
float YMax = 0.64;
float ZMin = -122.65;
float ZMax = 0;

void setup() {
  jewel.begin();
  jewel.show();
  //jewel.setBrightness(5);

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

  accel.setRange(LSM303_RANGE_4G);
  accel.setMode(LSM303_MODE_LOW_POWER);
}

void loop() {
  sensors_event_t magEvent;
  sensors_event_t accelEvent;
  
  accel.getEvent(&accelEvent);

  lsm.getEvent(&magEvent);
  
  setCompass(magEvent);

  checkForStrike(accelEvent);
}

void animationAwake() {
  for(uint16_t i = 1; i<jewel.numPixels(); i++) {
    jewel.clear();
    jewel.setPixelColor(i, jewel.Color(0, 55, 0));
    jewel.show();
    delay(100);
  }
}

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
      jewel.setPixelColor(j, jewel.Color(0, 55, 0));
    } else if( j == k ) {
      jewel.setPixelColor(k, jewel.Color(0, 0, 55));
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

void checkForStrike(sensors_event_t event) {
  if(event.acceleration.y <= 2.00) {
    for(int i = 0; i<5; i++) {
      jewel.clear();
      jewel.show();
      delay(50);
      jewel.setPixelColor(0, jewel.Color(0, 0, 255));
      jewel.setPixelColor(1, jewel.Color(0, 0, 255));
      jewel.setPixelColor(2, jewel.Color(0, 0, 255));
      jewel.setPixelColor(3, jewel.Color(0, 0, 255));
      jewel.setPixelColor(4, jewel.Color(0, 0, 255));
      jewel.setPixelColor(5, jewel.Color(0, 0, 255));
      jewel.setPixelColor(6, jewel.Color(0, 0, 255));
      jewel.show();
      delay(200);
    }
    jewel.clear();
  }
}
