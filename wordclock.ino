#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#include "FastLED.h"
#include "Wire.h"
#include "TroykaRTC.h"

#define NUM_LEDS 97
#define DATA_PIN 3

#define DHT_PIN 9
#define DHTTYPE           DHT11

#define LETTER_DELAY 0
#define WORD_DELAY 0

#define DEBUG true

RTC clock;
DHT_Unified dht(DHT_PIN, DHTTYPE);

CRGB leds[NUM_LEDS];
void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  clock.begin();
  dht.begin();
  //clock.set(__TIMESTAMP__);
}

int itis[] = {4,0,1,3,4};
int to[] = {2, 31, 32}; 
int past[] = {4, 41, 40, 39, 38};
int oclock[] = {7, 90, 91, 92, 93, 94, 95, 96};

int five[] = {4, 12, 13, 14, 15};
int ten[] = {3, 27, 28, 29};
int twenty[] = {6, 16, 17, 18, 19, 20, 21};
int quarter[] = {7,5,6,7,8,9,10,11}; 
int half[] = {4, 22, 23, 24, 25};

//len and starting position of hour-leds (-1 means backward order)
int hours[][2] = {
  {0, 0}, //fake hour to use 1-based indexes
  {-3, 63},
  {3, 42},
  {-5, 57},
  {4, 49},
  {4, 45},
  {-3, 60},
  {-5, 85},
  {5, 64},
  {-4, 36},
  {3, 86},
  {6, 69},
  {-6, 80}
};

void setWordColor(int what[], CRGB color) {
  for(int i=0; i<what[0]; i++) {
    leds[what[i+1]] = color;
    //FastLED.show();
    delay(LETTER_DELAY);
  }
  delay(WORD_DELAY);
}


void setHourColor(int hour, CRGB color) {
  int delta = 1;
  int len = hours[hour][0]; 
  if (hours[hour][0] < 0) {
    delta = -1;
    len = -len;
  }
  for(byte i=0; i<len; i++) {
    leds[hours[hour][1] + i*delta] = color;
    //FastLED.show();
    delay(LETTER_DELAY);
  }
  delay(WORD_DELAY);
}

void setTime(uint8_t hour, uint8_t minute, CRGB color) {
  setWordColor(itis, color);
  bool halfhour = false;
  
  if (minute < 30) {
    if (minute >= 5) {
      setWordColor(past, color);
    }
  } else if (minute > 30) {
    halfhour = true;
    minute = 60 - minute;
    setWordColor(to, color);
    hour++;
    if (hour>12) {
      hour = 1;
    }
  }
  
  //5,10,15,20,25,30
  if (minute < 5 && !halfhour) {
    setWordColor(oclock, color);
  }
  if (minute >= 5 && minute < 10) {
    setWordColor(five, color);
  }
  if (minute >= 10 && minute < 15) {
    setWordColor(ten, color);
  }
  if (minute >= 15 && minute < 20) {
    setWordColor(quarter, color);
  }    
  if (minute >= 20 && minute <25) {
    setWordColor(twenty, color);
  }
  if (minute >= 25 && minute < 30) {
    setWordColor(twenty, color);
    setWordColor(five, color);
  }
  if (minute == 30 || (minute > 25 && halfhour)) {
    setWordColor(half, color);
    setWordColor(past, color);
  }
  setHourColor(hour, color);
}


uint8_t hour = 1;
uint8_t minute = 0;

void loop() { 
  for(int i=0;i<NUM_LEDS;i++) {
    leds[i] = CRGB::Black;
  }

  sensors_event_t event;  
  dht.humidity().getEvent(&event);
  Serial.println(event.relative_humidity);
  
  CRGB color;

  if (event.relative_humidity <= 30) {
    color = CRGB::Red;
  } 
  if (event.relative_humidity > 30) {
    color = CRGB::Yellow;
  } 
  if (event.relative_humidity >= 40) {
    color = CRGB::White;
  }
  
  if (DEBUG) {
    minute++;
    if (minute>60) {
      minute = 0;
      hour++;
      if (hour > 12) {
        hour = 1;
      }
    }
    setTime(hour, minute, color);  
    FastLED.show();
    delay(500);
  } else {
    clock.read();
    minute = clock.getMinute();
    hour = clock.getHour();
    if (hour > 12) {
      hour = hour - 12;
    }
    if (hour == 0) {
      hour = 1;
    }
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    setTime(hour, minute, color);
    FastLED.show();
    delay(1000);
  }
}
