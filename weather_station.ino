#include <GyverPower.h>

#include "common.h"
#include "sensor.h"
#include "screen.h"
#include "menu.h"

#define SLEEP_SCREEN_TIME 5

#define BTN_PIN 2

boolean buttonWasUp = true;
boolean buttonPressed = false;

extern TFT TFTscreen;
extern encoderStruct encoder;
extern DHT dht;

extern Widget screen;
extern Widget mainMenu;

unsigned long encoder_timer;
unsigned long dht_timer;
unsigned long timer_delta;

extern int timer_period;

extern bool sleepScreen;
extern bool goSleepScreen;
extern unsigned long power_timer;

unsigned long sleep_screen_timer;

extern bool flRedraw;

void checkEncoder() {
  encoder.tick();  
  //
  buttonPressed = false;
  boolean buttonIsUp = digitalRead(BTN_PIN);
  if (buttonWasUp && !buttonIsUp) {    
    delay(50);
    buttonIsUp = digitalRead(BTN_PIN);
    if (!buttonIsUp)
      buttonPressed = true;
  }
}

void setup() {
  initEncoder();
  //
  TFTscreen.begin();
  //
  pinMode(BTN_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  encoder.setPosition(0);
  //
  attachInterrupt(0, wakeUpInterrupt, FALLING);
  //
  dht.begin();
  //
  screenInit();
  //
  power.setSleepMode(POWERDOWN_SLEEP);
  power_timer = millis();
}

void wakeUpInterrupt() {
  if (!power.inSleep()) return;
  //
  power.wakeUp();
  screenWakeUp();
}

void loop() {
  checkEncoder();
  //
  if (buttonPressed && sleepScreen)
    screenWakeUp();
  //
  bool fl = false;
  if (!sleepScreen) {
    if (millis() - dht_timer > 1000) {
      readSensors();
      drawHTWindows();
      dht_timer = millis();
    }
    //
    if (millis() - encoder_timer > 50) {
      fl = screen.handleEncoder();
      if (buttonPressed) {
        screen.handleButton();
        fl = true;
      }
      if (fl) {
        power_timer = millis();
        //
        flRedraw = true;
        screen.draw();
        flRedraw = false;
      }
      //
      if (mainMenu.redraw)
        mainMenu.draw();
      //
      encoder_timer = millis();
    }
  }
  //
  if (!goSleepScreen)
    goSleepScreen = (millis() - power_timer > (timer_period - SLEEP_SCREEN_TIME) * 1000);
  if (!sleepScreen && goSleepScreen) {
    screenDrawSleep();
    sleepScreen = true;
    sleep_screen_timer = millis();
  }
  if (sleepScreen && (millis() - sleep_screen_timer > SLEEP_SCREEN_TIME * 1000)) {
    TFTscreen.background(CLR_BLACK);
    power.sleep(SLEEP_FOREVER);
    screenWakeUp();
  }
  //
}
