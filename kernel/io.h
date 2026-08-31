#ifndef IO_H
#define IO_H

static inline unsigned char inb(unsigned short p) {
    unsigned char r;
    __asm__ volatile (
        "inb %1, %0" 
        : "=a"(r) 
        : "Nd"(p) 
        : "memory"
    );
    return r;
}

static inline void outb(unsigned short p, unsigned char v) {
    __asm__ volatile (
        "outb %0, %1" 
        : 
        : "a"(v), "Nd"(p) 
        : "memory"
    );
}

static inline void io_wait(void) {
    __asm__ volatile (
        "outb %%al, $0x80" 
        : 
        : "a"(0) 
        : "memory"
    );
}

static inline void insw(unsigned short p, void* addr, unsigned int count) {
    __asm__ volatile (
        "cld\n\t"
        "rep insw" 
        : "+D"(addr), "+c"(count) 
        : "d"(p) 
        : "memory"
    );
}

static inline void outsw(unsigned short p, const void* addr, unsigned int count) {
    __asm__ volatile (
        "cld\n\t"
        "rep outsw" 
        : "+S"(addr), "+c"(count) 
        : "d"(p) 
        : "memory"
    );
}

#endif