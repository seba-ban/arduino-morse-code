#pragma once
#include <LiquidCrystal.h>
#include "globals.h"

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

void shiftArrayLeft(char *c)
{
    for (int i = 1; i < 16; i++)
    {
        c[i - 1] = c[i];
    }
}

void initializeLcdBuffer(char *c)
{
    for (int i = 0; i < 16; i++)
    {
        c[i] = ' ';
    }
}

char bufTop[16] = {};
void writeLetterToLCD(char c)
{
    shiftArrayLeft(bufTop);
    bufTop[15] = c;
    lcd.setCursor(0, 0);
    lcd.print(bufTop);
}

char bufBottom[16] = {};
void writeSignalToLCD(char c)
{
    return;
    shiftArrayLeft(bufBottom);
    bufBottom[15] = c;
    lcd.setCursor(0, 1);
    lcd.print(bufBottom);
}

void initializeLcd()
{
    lcd.begin(16, 2);
    lcd.clear();
    initializeLcdBuffer(bufTop);
    initializeLcdBuffer(bufBottom);
}
