#ifndef PAGING_H
#define PAGING_H

__attribute__((aligned(4096))) unsigned long long l4[512];
__attribute__((aligned(4096))) unsigned long long dp[512];
__attribute__((aligned(4096))) unsigned long long pd[512];

void init(void) {
    __asm__ volatile ("xor %%rax, %%rax\n\tmov $512, %%rcx\n\trep stosq" : : "D"(l4) : "rax", "rcx", "di", "memory");
    __asm__ volatile ("xor %%rax, %%rax\n\tmov $512, %%rcx\n\trep stosq" : : "D"(dp) : "rax", "rcx", "di", "memory");
    __asm__ volatile ("xor %%rax, %%rax\n\tmov $512, %%rcx\n\trep stosq" : : "D"(pd) : "rax", "rcx", "di", "memory");

    l4[0] = (((unsigned long long)dp) & 0x000FFFFFFFFFF000ULL) | 0x03ULL;
    dp[0] = (((unsigned long long)pd) & 0x000FFFFFFFFFF000ULL) | 0x03ULL;

    unsigned long long addr = 0x83ULL | (1ULL << 63); 
    for (int i = 0; i < 512; i++) {
        pd[i] = addr;
        addr += 0x200000ULL;
    }

    __asm__ volatile ("mov %0, %%cr3" :: "r"(l4) : "memory");
    __asm__ volatile (
        "mov %%cr4, %%rax\n\t"
        "or $0x000000B0, %%rax\n\t" 
        "mov %%rax, %%cr4\n\t"
        "mov $0xC0000080, %%ecx\n\t"
        "rdmsr\n\t"
        "or $0x00000900, %%eax\n\t" 
        "wrmsr\n\t"
        "mov %%cr0, %%rax\n\t"
        "and $0x9FFFFFFF, %%rax\n\t" 
        "or $0x80010023, %%rax\n\t" 
        "mov %%rax, %%cr0"
        ::: "rax", "rcx", "rdx", "memory"
    );
}

#endif