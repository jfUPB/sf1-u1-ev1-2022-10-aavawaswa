#include "SSD1306Wire.h"
#define LED1BOMB_OUT 25
#define LED2_COUNT 26
#define UP_BTN1 13
#define DOWN_BTN3 32
#define ARM_BTN2 33
#define LED3_DISSARMED 27

SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);

void task();

void setup() {
  task();
}

void task() {
  enum class BombStates {INIT, CONFIG, ARMED, PREP};
  static BombStates bombState =  BombStates::INIT;
  static uint8_t counter;
  static int x;
  static int y;
  static uint32_t previousMillis1 = 0;
  static uint8_t ledState1 = LOW;
  uint32_t currentMillis1 = millis();
  const uint32_t interval1 = 500;
  const uint32_t interval2 = 1000;
  uint32_t currentMillis2 = millis();
  static uint32_t previousMillis2 = 0;


  switch (bombState) {
    case BombStates::INIT: {
        counter = 20;
        x = 0;
        y = 0;
        pinMode(LED1BOMB_OUT, OUTPUT);
        pinMode(LED2_COUNT, OUTPUT);
        pinMode(UP_BTN1, INPUT_PULLUP);
        pinMode(DOWN_BTN3, INPUT_PULLUP);
        pinMode(ARM_BTN2, INPUT_PULLUP);
        pinMode(LED3_DISSARMED, OUTPUT);

        display.init();
        display.setContrast(255);
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.setFont(ArialMT_Plain_16);

        display.clear();
        display.drawString(0, 5, String(counter));
        display.display();



        bombState = BombStates::CONFIG;

        break;
      }
    case BombStates::CONFIG: {
        if ((digitalRead(UP_BTN1) == LOW) && (counter < 60) && (x == 0)) {
          counter++ ;
          display.clear();
          display.drawString(0, 5, String(counter));
          display.display();
          x = 1;
          delay(100);
        }
        else if (digitalRead(UP_BTN1) == HIGH ) {
          x = 0;
          delay(100);
        }
        if ((digitalRead(DOWN_BTN3) == LOW) && (counter > 10) && (y == 0)) {
          counter-- ;
          display.clear();
          display.drawString(0, 5, String(counter));
          display.display();
          delay(100);
        }
        else if (digitalRead(DOWN_BTN3) == HIGH ) {
          y = 0;
          delay(100);
        }
        if (digitalRead(ARM_BTN2) == LOW ) {
          bombState = BombStates::ARMED;
        }
        break;
      }



    case BombStates::ARMED: {

        if ((currentMillis1 - previousMillis1 >= interval1) && (counter > 0)) {
          previousMillis1 = currentMillis1;
          if (ledState1 == LOW) {
            ledState1 = HIGH;
          } else {
            ledState1 = LOW;
          }
          digitalWrite(LED2_COUNT, ledState1);
        }
        if ((currentMillis2 - previousMillis2 >= interval2) && (counter > 0)) {
          previousMillis2 = currentMillis2;
          counter-- ;
          display.clear();
          display.drawString(0, 5, String(counter));
          display.display();
        }
        if (counter == 0) {
          digitalWrite(LED2_COUNT, LOW);
          digitalWrite(LED1BOMB_OUT, HIGH);
          display.clear();
          display.drawString(0, 5, "BOOM");
          display.display();
        }
        //UP,UP,DOWN, DOWN, UP, DOWN, ARM.


        /*if ((digitalRead(UP_BTN1) == LOW) || (digitalRead(DOWN_BTN3) == LOW) || (digitalRead(ARM_BTN2) == LOW )) {
          bombState = BombStates::PREP;
          */
        }



      



    case BombStates::PREP: {

        break;
      }
  }
}
void loop() {
  task();
}
