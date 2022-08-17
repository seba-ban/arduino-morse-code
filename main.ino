#include "src/globals.h"
#include "src/lcd.h"
#include "src/morse.h"

void setup()
{
    Serial.begin(115200);
    initializeMorseCodes();
    initializeLcd();
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
}

void loop()
{
    // morseCodeWriteLoop();
    morseCodeReadLoop();
}
