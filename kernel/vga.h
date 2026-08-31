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

void scroll(void) {
    void* dest = (void*)VGA;
    const void* src = (const void*)(VGA + (80 * 2));
    mcpy64(dest, src, (24 * 80 * 2) / 8);
    
    void* clear_line = (void*)(VGA + (24 * 80 * 2));
    unsigned long long empty_val = 0x0720072007200720ULL;
    mset64(clear_line, empty_val, (80 * 2) / 8);
    
    p = 24 * 80;
}

void pr(const char* s) {
    volatile unsigned short* b = (volatile unsigned short*)VGA;
    unsigned short attr = COL << 8;
    int cur = p;

    while (*s) {
        if ((unsigned int)cur >= 2000) {
            p = cur;
            scroll();
            cur = p;
            b = (volatile unsigned short*)VGA;
        }
        b[cur++] = attr | (unsigned char)*s++;
    }
    p = cur;
    mv(p);
}

void newline(void) {
    p = ((p / 80) + 1) * 80;
    if (p >= 2000) {
        scroll();
    }
    mv(p);
}

void vga_backspace(void) {
    if (p > 0) {
        p--;
        volatile unsigned short* b = (volatile unsigned short*)VGA;
        b[p] = 0x0700;
        mv(p);
    }
}

#endif