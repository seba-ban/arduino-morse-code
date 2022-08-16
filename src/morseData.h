#pragma once
#include "globals.h"
#include "lcd.h"
#define _MORSE_SIGNAL(X) 0b1##X
#define _CHARS_SUPPORTED 36
#define _BIN_TREE_LEN (1 << 6) - 1
#define _SIGNAL_MASK 0b10000000
#define _MAX_SIGNAL_SHIFTS 7
#define _LOOP_SIGNAL(signal)                                            \
    int shifts = 0;                                                     \
    _MorseSignal currentSignal;                                         \
    while (shifts < _MAX_SIGNAL_SHIFTS && (signal & _SIGNAL_MASK) == 0) \
    {                                                                   \
        shifts++;                                                       \
        signal = signal << 1;                                           \
    }                                                                   \
    while (shifts < _MAX_SIGNAL_SHIFTS)                                 \
    {                                                                   \
        shifts++;                                                       \
        signal = signal << 1;                                           \
        currentSignal = (signal & _SIGNAL_MASK) == 0 ? _MORSE_SHORT : _MORSE_LONG;
#define _LOOP_SIGNAL_END }

typedef enum
{
    _MORSE_SHORT,
    _MORSE_LONG
} _MorseSignal;

// ========== BNARY TREE STRUCTURE ===========
// supposed to be used when analyzing
// incoming signals

char binaryTree[(1 << 6) - 1] = {0};

int getNextBinaryIndex(int lastIndex, _MorseSignal signal)
{
    int i = 2 * lastIndex + (signal == _MORSE_SHORT ? 1 : 2);
    if (i >= _BIN_TREE_LEN) {
        return -1;
    }
    return i;
}

void addCharBinary(char letter, unsigned char signal)
{
    int i = 0;
    _LOOP_SIGNAL(signal)
    i = getNextBinaryIndex(i, currentSignal);
    _LOOP_SIGNAL_END
    binaryTree[i] = letter;
}

// note this works on signals with the additional bit
char analyzeSignalBinary(char signal)
{
    int i = 0;
    _LOOP_SIGNAL(signal)
    i = getNextBinaryIndex(i, currentSignal);
    if (i == -1)
    {
        return 0;
    }
    _LOOP_SIGNAL_END
    return binaryTree[i];
}

// ========== ARRAY OF SIGNALS ================
// to be used when convertng chars to morse

// 0-9 - numbers
// 10-35 - capital letters
unsigned char signals[_CHARS_SUPPORTED] = {};

int getIndex(char c)
{
    int initialAscii;

    // 0 - 48
    // 9 - 57
    // A - 65
    // Z - 90
    if (c < 48)
    { // not a number
        return -1;
    }
    else if (c <= 57)
    {
        initialAscii = 48; // 0 value in ascii
    }
    else if (c < 65)
    {
        return -1;
    }
    else if (c <= 90)
    {
        initialAscii = 65 - 10; // A value in ascii - numbers len in the array
    }
    else
    {
        return -1;
    };

    return (c - initialAscii) % _CHARS_SUPPORTED;
}

// ========= INITIALIZATION ==============

void registerSignal(char letter, unsigned char bits)
{
    signals[getIndex(letter)] = bits;
    addCharBinary(letter, bits);
}

void initializeMorseCodes()
{
    registerSignal('0', _MORSE_SIGNAL(11111));
    registerSignal('1', _MORSE_SIGNAL(01111));
    registerSignal('2', _MORSE_SIGNAL(00111));
    registerSignal('3', _MORSE_SIGNAL(00011));
    registerSignal('4', _MORSE_SIGNAL(00001));
    registerSignal('5', _MORSE_SIGNAL(00000));
    registerSignal('6', _MORSE_SIGNAL(10000));
    registerSignal('7', _MORSE_SIGNAL(11000));
    registerSignal('8', _MORSE_SIGNAL(11100));
    registerSignal('9', _MORSE_SIGNAL(11110));

    registerSignal('A', _MORSE_SIGNAL(01));
    registerSignal('B', _MORSE_SIGNAL(1000));
    registerSignal('C', _MORSE_SIGNAL(1010));
    registerSignal('D', _MORSE_SIGNAL(100));
    registerSignal('E', _MORSE_SIGNAL(0));
    registerSignal('F', _MORSE_SIGNAL(0010));
    registerSignal('G', _MORSE_SIGNAL(110));
    registerSignal('H', _MORSE_SIGNAL(0000));
    registerSignal('I', _MORSE_SIGNAL(00));
    registerSignal('J', _MORSE_SIGNAL(0111));
    registerSignal('K', _MORSE_SIGNAL(101));
    registerSignal('L', _MORSE_SIGNAL(0100));
    registerSignal('M', _MORSE_SIGNAL(11));
    registerSignal('N', _MORSE_SIGNAL(10));
    registerSignal('O', _MORSE_SIGNAL(111));
    registerSignal('P', _MORSE_SIGNAL(0110));
    registerSignal('Q', _MORSE_SIGNAL(1101));
    registerSignal('R', _MORSE_SIGNAL(010));
    registerSignal('S', _MORSE_SIGNAL(000));
    registerSignal('T', _MORSE_SIGNAL(1));
    registerSignal('U', _MORSE_SIGNAL(001));
    registerSignal('V', _MORSE_SIGNAL(0001));
    registerSignal('W', _MORSE_SIGNAL(011));
    registerSignal('X', _MORSE_SIGNAL(1001));
    registerSignal('Y', _MORSE_SIGNAL(1011));
    registerSignal('Z', _MORSE_SIGNAL(1100));
}
