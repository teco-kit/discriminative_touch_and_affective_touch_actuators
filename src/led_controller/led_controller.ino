#include <FastLED.h>

#define LED_PIN     7
#define NUM_LEDS    120 // 119 is maximum

#define C_NOTE_INDEX 48
#define D_NOTE_INDEX 51
#define E_NOTE_INDEX 55
#define F_NOTE_INDEX 58
#define G_NOTE_INDEX 61

CRGB leds[NUM_LEDS];

CRGB on = CRGB(50,50,50);
CRGB off = CRGB(0,0,0);
CRGB calibration_on = CRGB(5,5,5);

void setup() {
  // Add LEDs to library so that they can be controlled
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS); 
}

// C G F E D C G

void loop() {
  // Show calibaration light on start
  delay(100);
  leds[0] = calibration_on;
  FastLED.show();
  delay(600);
  leds[0]= off;
  FastLED.show();

  // C: on, wait, off
  leds[C_NOTE_INDEX] = on;
  FastLED.show();
  delay(500);
  leds[C_NOTE_INDEX] = off;


  // G: on, wait, off
  leds[G_NOTE_INDEX] = on;
  FastLED.show();
  delay(500);
  leds[G_NOTE_INDEX] = off;

  // F: on, wait, off
  leds[F_NOTE_INDEX] = on;
  FastLED.show();
  delay(500);
  leds[F_NOTE_INDEX] = off;

  // E: on, wait, off
  leds[E_NOTE_INDEX] = on;
  FastLED.show();
  delay(500);
  leds[E_NOTE_INDEX] = off;

  // D: on, wait, off
  leds[D_NOTE_INDEX] = on;
  FastLED.show();
  delay(500);
  leds[D_NOTE_INDEX] = off;

  // C: on, wait, off
  leds[C_NOTE_INDEX] = on;
  FastLED.show();
  delay(500);
  leds[C_NOTE_INDEX] = off;

  // G: on, wait, off
  leds[G_NOTE_INDEX] = on;
  FastLED.show();
  delay(500);
  leds[G_NOTE_INDEX] = off;
  FastLED.show();
}
