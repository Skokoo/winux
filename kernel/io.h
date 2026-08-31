#ifndef IO_H
#define IO_H

static inline unsigned char inb(unsigned short p) {
    unsigned char r;
    __asm__ volatile ("inb %1, %0" : "=a"(r) : "Nd"(p));
    return r;
}

static inline void outb(unsigned short p, unsigned char v) {
    __asm__ volatile ("outb %0, %1" :: "a"(v), "Nd"(p));
}

#endif