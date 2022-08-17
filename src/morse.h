#pragma once
#include "globals.h"
#include "morseData.h"
#include "lcd.h"

void buzz(long time)
{
    digitalWrite(BUZZER, HIGH);
    delay(time);
    digitalWrite(BUZZER, LOW);
}

void buzzLetter(char c)
{
    int index = getIndex(c);

    if (index == -1)
    {
        return;
    }

    writeLetterToLCD(c);

    unsigned char signal = signals[index];

    _LOOP_SIGNAL(signal)
    if (currentSignal == _MORSE_SHORT)
    {
        writeSignalToLCD(SHORT_CHAR);
        buzz(SHORT);
    }
    else
    {
        writeSignalToLCD(LONG_CHAR);
        buzz(LONG);
    }
    if (shifts < _MAX_SIGNAL_SHIFTS)
    {
        delay(INTRA_LETTER_SPACE);
    }
    _LOOP_SIGNAL_END
}

String message;

bool checkMessage()
{
    if (Serial.available())
    {  
        message = Serial.readStringUntil('\n');
        message.toUpperCase();
        if (!message.endsWith(" ")) {
            message += " ";
        }
        return true;
    }
    return false;
}

void morseCodeWriteLoop()
{
    checkMessage();
    int sLen = message.length();
    for (int i = 0; i < sLen; i++)
    {
        if (checkMessage())
        {
            break;
        }

        if (message[i] == ' ')
        {
            writeSignalToLCD(WORD_SPACE_CHAR);
            writeLetterToLCD(' ');
            delay(WORDS_SPACE);
        }
        else
        {
            buzzLetter(message[i]);
            if (i < sLen - 1)
            {
                writeSignalToLCD(LETTER_SPACE_CHAR);
                delay(LETTERS_SPACE);
            }
        }
    }
}

unsigned long shortSignal = 200000;
unsigned long shortSignalFrom = shortSignal * 0.5;
unsigned long shortSignalTo = shortSignal * 1.5;
unsigned long longSignal = shortSignal * 3;
unsigned long longSignalFrom = longSignal * 0.5;
unsigned long longSignalTo = longSignal * 1.5;

typedef enum
{
    CLICK_DURATION_TIMER,
    PAUSE_DURATION_TIMER,
    IDLE,
} Phase;

Phase currentPhase = IDLE;

unsigned char currentInput = 1;
unsigned long dur = 0;
unsigned long lastMicros = 0;

void morseCodeReadLoop()
{

    if (digitalRead(BUTTON) == HIGH)
    { // not clicked
        digitalWrite(BUZZER, LOW);

        switch (currentPhase)
        {
        case CLICK_DURATION_TIMER:
            dur = micros() - lastMicros;
            Serial.println(dur);

            if (dur >= shortSignalFrom && shortSignalTo >= dur)
            {
                currentInput = (currentInput << 1) + 0;
                writeSignalToLCD(SHORT_CHAR);
                writeLetterToLCD(' ');
            }
            else if (dur >= longSignalFrom && longSignalTo >= dur)
            {
                currentInput = (currentInput << 1) + 1;
                writeSignalToLCD(LONG_CHAR);
                writeLetterToLCD(' ');
            }
            else
            {
                currentPhase = IDLE;
                writeLetterToLCD('?');
                currentInput = 1;
                break;
            }

            if (currentInput > 127)
            {
                currentPhase = IDLE;
                writeLetterToLCD('?');
                currentInput = 1;
                break;
            }

            lastMicros = micros();
            currentPhase = PAUSE_DURATION_TIMER;

            break;

        case PAUSE_DURATION_TIMER:
            dur = micros() - lastMicros;
            if (dur >= longSignalFrom && longSignalTo >= dur)
            {
                char c = analyzeSignalBinary(currentInput);
                Serial.println("analyzing...");

                Serial.println(c);
                if (c == 0)
                {
                    c = '?';
                }
                writeLetterToLCD(c);
                writeSignalToLCD(' ');
                currentInput = 1;
                currentPhase = IDLE;
            }
            break;

        default:
            break;
        }
    }
    else
    { // clicked
        digitalWrite(BUZZER, HIGH);

        switch (currentPhase)
        {
        case IDLE:
        case PAUSE_DURATION_TIMER:
            currentPhase = CLICK_DURATION_TIMER;
            lastMicros = micros();
            break;

        default:
            break;
        }
    }
    delay(25);
}

