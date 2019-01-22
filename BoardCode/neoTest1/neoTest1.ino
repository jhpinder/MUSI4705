#include <Adafruit_NeoPixel.h>
#define NP 3
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, A15, NEO_RGB + NEO_KHZ800);
void setup() {
  // put your setup code here, to run once:

  strip.begin();
  strip.setBrightness(80);
  strip.setPixelColor(0,0,100,100);
  strip.setPixelColor(1,10,60,10);
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 3; i++) {
    strip.setPixelColor(i,75,75,0);
    if (i == 1) {
      strip.setPixelColor(i - 1,0,0,0);
    } else if (i == 0) {
      strip.setPixelColor(i,100,0,0);
      strip.setPixelColor(2,0,0,0);
    } else if (i == 2) {
      strip.setPixelColor(i,0,100,0);
      strip.setPixelColor(i - 1,0,0,0);
    }
    strip.show();
    delay(200);
  }
}
