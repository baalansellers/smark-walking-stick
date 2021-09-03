#include <Adafruit_NeoPixel.h>

#include "Animation.h"

Animation::Animation(TapDetect tapDetect) {
  _CycleUp = true;
  _CurrentGreen = 0;
  _CurrentRed = 0;
  _CurrentBlue = 0;
  _CurrentBreath = 0;
  _CurrentHue = 0;
  _TapDetect = tapDetect;
}

/*
 * ANIMATIONS
 */
void Animation::awaken(Adafruit_NeoPixel jewel) {
  for(uint16_t i = 1; i<jewel.numPixels(); i++) {
    jewel.clear();
    jewel.setPixelColor(i, jewel.Color(0, 55, 0));
    jewel.show();
    delay(100);
  }
}

void Animation::cycle(Adafruit_NeoPixel jewel) {
  if (_CycleUp) {
    if (_CurrentGreen < 255) {
      _CurrentGreen++;
    } else if (_CurrentRed < 255) {
      _CurrentRed++;
    } else if (_CurrentBlue < 255) {
      _CurrentBlue++;
    } else {
      _CycleUp = false;
    }
  } else {
    if (_CurrentGreen > 0) {
      _CurrentGreen--;
    } else if (_CurrentRed > 0) {
      _CurrentRed--;
    } else if (_CurrentBlue > 0) {
      _CurrentBlue--;
    } else {
      _CycleUp = true;
    }
  }
  
  for(uint16_t i = 0; i<jewel.numPixels(); i++) {
    jewel.setPixelColor(i, jewel.Color(_CurrentGreen, _CurrentRed, _CurrentBlue));
  }
  
  jewel.show();
  delay(50);
}

void Animation::breath(Adafruit_NeoPixel jewel, rgbColor color) {
  if (_CycleUp) {
    if (_CurrentBreath < 255) {
      _CurrentBreath++;
    } else {
      _CycleUp = false;
    }
  } else {
    if (_CurrentBreath > 0) {
      _CurrentBreath--;
    } else {
      _CycleUp = true;
    }
  }
  
  for(uint16_t i = 0; i<jewel.numPixels(); i++) {
    switch (color) {
      case RED:
        jewel.setPixelColor(i, jewel.Color(0, _CurrentBreath, 0));
        break;
      case GREEN:
        jewel.setPixelColor(i, jewel.Color(_CurrentBreath, 0, 0));
        break;
      case BLUE:
        jewel.setPixelColor(i, jewel.Color(0, 0, _CurrentBreath));
        break;
    }
  }
  
  jewel.show();
  delay(20);
}

void Animation::rainbowLightning(Adafruit_NeoPixel jewel){
  for(uint16_t i = 0; i<jewel.numPixels(); i++) {
    if (random(0,100) > 90) {
      jewel.setPixelColor(i, jewel.ColorHSV(random(0,255), 255, 255) ); 
    }
    else{
      jewel.setPixelColor(i, 0, 0, 0);
    }
  }
  
  jewel.show();
  
  delay(random(5,100));
  
  jewel.clear();
  jewel.show();
}

void Animation::lightning(Adafruit_NeoPixel jewel){
  for(int r = 0; r<random(2,16); r++) {
    for(uint16_t i = 0; i<jewel.numPixels(); i++) {
      if (random(0,100) > 90) {
        jewel.setPixelColor(i, jewel.ColorHSV(0, 0, 255));
      }
      else{
        jewel.setPixelColor(i, 0, 0, 0);
      }
    }

    if (_TapDetect()) {
      return;
    }
    
    jewel.show();
    
    delay(random(5,100));
    
    jewel.clear();
    jewel.show();
  }

  delay(random(200,800));
}

//Sucks... fix later
void Animation::cracklingLightning(Adafruit_NeoPixel jewel){
   for(uint16_t i = 0; i<jewel.numPixels(); i++) {
      jewel.setPixelColor(i, jewel.ColorHSV(0, 0, 255)); 
   }
   
   jewel.show();
   
   delay(random(10,100));
   
   jewel.clear();
   jewel.show();
}

void Animation::cycleHue(Adafruit_NeoPixel jewel) {
  _CurrentHue++;

  if (_CurrentHue > 65535) {
    _CurrentHue = 0;
  }
  
  for(uint16_t i = 0; i<jewel.numPixels(); i++) {
    jewel.setPixelColor(i, jewel.ColorHSV(_CurrentHue, 255, 255)); 
  }

  jewel.show();
}


/*
 * FLASHLIGHTS
 */
void Animation::igniteFlashlight(Adafruit_NeoPixel jewel, lightTemp temp) {
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
