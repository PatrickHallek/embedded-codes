#include "FastLED.h"
#include "FastLED_RGBW.h"

#define NUM_LEDS 30
#define DATA_PIN 6

CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];

uint8_t brightness_max = 50;
int brightness = brightness_max;
uint8_t brightness_min = 10;
int init_r = 58;
int init_g = 255;
int init_b = 255;
int init_w = 0;

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
  FastLED.setBrightness(brightness_max);
  FastLED.show();
  Serial.begin(9600);
  fillColor(init_r, init_g, init_b, init_w);
}

void loop() {
  pulse();
}

void randomBlink() {
  int z = random(1, NUM_LEDS);
  for (int i = 0; i < 255; i++) {
    leds[z] = CRGBW(init_r, init_g, init_b, i);
    FastLED.show();
    delay(3);
  }
  delay(20);
  for (int i = 0; i < 255; i++) {
    leds[z] = CRGBW(init_r, init_g, init_b, 255 - i);
    FastLED.show();
    delay(3);
  }
}

void shifter() {
  for (int z = 0; z < NUM_LEDS; z++) {
    leds[z] = CRGBW(255, 0, 0, 0);
    FastLED.show();
    delay(10);
    leds[z] = CRGBW(init_r, init_g, init_b, 0);
    FastLED.show();
    delay(10);
  }
}

void pulse() {
  int i = 0;
  if (brightness == brightness_max) {
    for (i = 0; i <= brightness - brightness_min; i++) {
      FastLED.setBrightness(brightness - i);
      FastLED.show();
      delay(50);
    }
    brightness = brightness_min;
    delay(200);
  } else {
    for (i = 0; i < brightness_max - brightness_min; i++) {
      FastLED.setBrightness(brightness + i);
      FastLED.show();
      delay(50);
    }
    brightness = brightness_max;
  }
}

void fillColor(int r, int g, int b, int w) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGBW(r, g, b, w);
    FastLED.show();
    delay(50);
  }
}
