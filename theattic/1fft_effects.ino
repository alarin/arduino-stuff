arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
const uint16_t samples = 8; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 1000; //Hz, must be less than 10000 due to ADC

unsigned int sampling_period_us = round(1000000*(1.0/samplingFrequency));
unsigned long microseconds;

double vReal[samples];
double vImag[samples];


double getPeak() {
  for(int i=0; i<samples; i++)
  {
      microseconds = micros();    //Overflows after around 70 minutes!

      vReal[i] = analogRead(MIC_PIN);
      vImag[i] = 0;
      while(micros() < (microseconds + sampling_period_us)){
        //empty loop
      }
  }
  /* Print the results of the sampling according to time */
//  Serial.println("Data:");
//  PrintVector(vReal, samples, SCL_TIME);
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
//  Serial.println("Weighed data:");
//  PrintVector(vReal, samples, SCL_TIME);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
//  Serial.println("Computed Real values:");
//  PrintVector(vReal, samples, SCL_INDEX);
//  Serial.println("Computed Imaginary values:");
//  PrintVector(vImag, samples, SCL_INDEX);
  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
//  Serial.println(vReal[0], 6);
//  Serial.println("Computed magnitudes:");
//  PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
  double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
  //Serial.println(x, 6); //Print out what frequency is the most dominant.  
  return x;
}

double maxPeak = 0;

void equalizer() {
  for (int i=0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, vReal[i/samples], 0);
  }
  FastLED.show();  
}

void peakBlink() {
  double peak = getPeak();
  if (peak > maxPeak) {
    maxPeak = peak;
  }
  peak = MAX_BRIGHT/maxPeak * peak;
  //Serial.println(peak, 6);
  
  setAll(CRGB(peak, 0, 0));  
}

double minMag = 0;
double maxMag = 0;
double avgAvgMag = 0;
int cycle = 0;

void peakBlinkAvgFreqColor() {
  double peak = getPeak();
  if (peak > maxPeak) {
    maxPeak = peak;
  }
  peak = MAX_BRIGHT/maxPeak * peak;
  //Serial.println(peak, 6);

  double avgMag = 0;
  for (int i=0; i<samples; i++) avgMag += vReal[i];
  avgMag = avgMag/samples;
  avgAvgMag = (avgMag + avgAvgMag)/2;
  avgMag=avgMag-avgAvgMag;

  Serial.println(avgMag);

  if (avgMag > maxMag or avgMag < minMag) {
    setAll(CRGB(peak, 0, 0));  
  } else {
    if (avgMag > 0) {
      setAll(CRGB(0, peak, 0));  
    } else {
      setAll(CRGB(0, 0, peak));  
    }
  }
  if (avgMag > maxMag) maxMag = avgMag;
  if (avgMag < minMag) minMag = avgMag;  
  cycle++;
  if (cycle > 100) {
    maxMag = 0;
    minMag = 0;
    cycle = 0;
  }  
  
}

