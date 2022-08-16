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
