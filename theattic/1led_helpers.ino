void setAll(CRGB color) {
    for(int dot = 0; dot < NUM_LEDS; dot++) { 
        leds[dot] = color;        
    }  
    FastLED.show();
}

void ledRotateLeft() {
  CRGB last = leds[0];
  for (int i=0; i < NUM_LEDS-1; i++) {
    CRGB swap = leds[i+1];
    leds[i+1] = leds[i];
    leds[i] = swap;
  }
  leds[NUM_LEDS-1] = last;
  FastLED.show();
}

