#ifndef VGA_H
#define VGA_H

#include "io.h"

#define VGA 0xB8000
#define COL 0x0A

int p = 0;

void mv(int pos) {
    if ((unsigned int)pos >= 2000) return;
    outb(0x3D4, 0x0F); outb(0x3D5, (unsigned char)(pos & 0xFF));
    outb(0x3D4, 0x0E); outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

void pr(const char* s) {
    volatile unsigned short* b = (volatile unsigned short*)VGA;
    unsigned short attr = COL << 8;
    int cur = p;
    while (*s) {
        if ((unsigned int)cur >= 2000) cur = 0;
        b[cur++] = attr | (unsigned char)*s++;
    }
    p = cur;
    mv(p);
}

void newline(void) {
    p = ((p / 80) + 1) * 80;
    if (p >= 2000) p = 0;
    mv(p);
}

#endif