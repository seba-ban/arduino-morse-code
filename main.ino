#include "src/globals.h"
#include "src/lcd.h"
#include "src/morse.h"

void setup()
{
    Serial.begin(9600);

    initializeMorseCodes();
    initializeLcd();
    // initialize buzzer
    pinMode(BUTTON, INPUT);
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
}

void loop()
{
    // unsigned long duration;
    // duration = pulseIn(BUTTON, HIGH);
    // // Serial.println(duration);
    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print(duration);
    // morseCodeWriteLoop();
}
