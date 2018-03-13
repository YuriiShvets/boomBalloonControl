//Додано налаштування режиму очікування і покращено налаштування часу

#ifndef Project
#define Project

#include <SdFat.h>
#include <SdFatUtil.h>

void(* resetFunc) (void) = 0;

SdFat sd;

int numberOfDemo;
int numberOfGames;

#include "Clock.h"
#include "Statistics.h"
#include "Music.h"
#include "Sekyurotron.h"
#include "GameCounter.h"
#include "AnalogButton.h"
#include "DigitalOut.h"
#include "Config.h"
#include "Program.h"


void setup() {
  Serial.begin(9600);
  Program *program = new Program();
  Clock *Time = new Clock();
  while(true) {
    program -> Update();
    //Serial.println(Time -> getYear());
  }
}

void loop() {
  
}

#endif
