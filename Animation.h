enum lightTemp {
  WARM,
  COOL
};

enum rgbColor {
  RED,
  GREEN,
  BLUE
};

typedef bool (*TapDetect) ();

class Animation {
public:
  Animation(TapDetect);
  void awaken(Adafruit_NeoPixel);
  void cycle(Adafruit_NeoPixel);
  void breath(Adafruit_NeoPixel, rgbColor);
  void rainbowLightning(Adafruit_NeoPixel);
  void lightning(Adafruit_NeoPixel);
  void cracklingLightning(Adafruit_NeoPixel);
  void cycleHue(Adafruit_NeoPixel);
  void igniteFlashlight(Adafruit_NeoPixel, lightTemp);

private:
  bool _CycleUp;
  int _CurrentGreen;
  int _CurrentRed;
  int _CurrentBlue;
  int _CurrentBreath;
  int _CurrentHue;
  TapDetect _TapDetect;
};
