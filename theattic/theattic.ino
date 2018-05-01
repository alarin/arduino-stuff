#include "FastLED.h"
#include "arduinoFFT.h"

#define NUM_LEDS 60*3
#define LED_PIN 6

#define IR_PIN 2
#define MIC_PIN A0

#define MAX_BRIGHT 50

CRGB leds[NUM_LEDS];
