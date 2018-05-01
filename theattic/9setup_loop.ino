void setup() { 
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS); 

  setupIRL();

  for(int i=0; i<NUM_LEDS; i++) {
    leds[i] = CRGB(i % 60, i % 120, i % 180);
  }
  FastLED.show();
    
  Serial.begin(115200);
  Serial.println("Ready");
}


#define CDELTA 10;
void loop() {
//  if (IRLremote.available()) {
//    readRemoteChangeColor();
//  }
//
// if (!IRLremote.receiving()) {
//    setAll(CRGB(colors[0], colors[1], colors[2]));
// }
  ledRotateLeft();
  delay(10);
  // peakBlinkAvgFreqColor();
 //peakBlink();
}
