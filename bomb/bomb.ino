#include "SSD1306Wire.h"

#define LED1BOMB_OUT 25
#define LED2_COUNT 26
#define UP_BTN1 13
#define DOWN_BTN3 32
#define ARM_BTN2 33

// Selecciona uno según tu display.
SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);
//SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_64_48);

void task();

void setup() {
  task();
}

void task() {
  enum class BombStates {INIT, CONFIG, ARMED};
  static BombStates bombState =  BombStates::INIT;
  static uint8_t counter;
  static int x;
  static int y;



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
        if ((digitalRead(DOWN_BTN3) == LOW) && (counter > 0)&& (y == 0)) {
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

if (digitalRead(ARM_BTN2) == LOW ){
  bombState = BombStates::ARMED;
  }

        break;
      }

    case BombStates::ARMED: {

      digitalWrite(LED2_COUNT,HIGH);
        break;
      }

  }

}

void loop() {
  task();

}
