#include "pitches.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define LED_PIN 6
#define SOUND_PIN 9
#define PUNCH_PIN 10
#define LED_NUM 10

int score [10][3] = {
    {246,0,255},
    {74,0,255},
    {2,255,198},
    {6,143,2},
    {82,250,6},
    {214,247,2},
    {247,194,2},
    {247,137,2},
    {247,80,2},
    {255,2,2}
};
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_NUM, LED_PIN, NEO_GRB + NEO_KHZ800);

// notes in the melody:
int melody[] = {
  NOTE_A1, NOTE_F1, NOTE_A2, NOTE_F2, NOTE_A3, NOTE_F3, NOTE_A4, NOTE_F4,NOTE_A5, NOTE_F5, NOTE_A7
};

void setup() {
  // iterate over the notes of the melody:
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  pixels.begin();
  Serial.begin(9600);
}

int punchValue = 0;
int punchScore = 0;
boolean lock = 0;
void loop() {
  punchValue = analogRead(PUNCH_PIN);
  if(punchValue>5){
    //divide 1023 into 10 pieces
    punchScore = punchValue/60;
    Serial.println(punchScore);
    //if the value is out of range
    if(punchScore>LED_NUM)
      punchScore = LED_NUM;
    //clean LED color
    for(int i=0;i<=LED_NUM;i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0));
      pixels.show();
    }
  }
  if(punchScore > 0){
    //run score LED
    for(int i=0;i<punchScore && i<=LED_NUM;i++){
      pixels.setPixelColor(i, pixels.Color(score[i][0],score[i][1],score[i][2]));
      pixels.show();
      tone(SOUND_PIN, melody[i], i*150+200);
      delay(i*150+200);
    }
    //LED blink at the final
    for(int t=1;t<=7;t++){
      if((t%2)==0){
        for(int i=0;i<punchScore && i<=LED_NUM;i++){
          pixels.setPixelColor(i, pixels.Color(score[i][0],score[i][1],score[i][2]));
          pixels.show();
        }
        tone(SOUND_PIN, melody[9], 500);
        delay(500);
      }else{
        for(int i=0;i<=LED_NUM;i++){
          pixels.setPixelColor(i, pixels.Color(0,0,0));
          pixels.show();
        }
        delay(500);
      }
    }
  }
  noTone(SOUND_PIN);
  punchValue = 0;
  punchScore = 0;
}
