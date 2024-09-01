#ifndef __SYSTEM_H
#define __SYSTEM_H

typedef int size_t;

/* Este define qual pilha estamos olhando depois de um ISR estar rodando */

struct regs
{
    unsigned int ds, es, fs, gs;                          /* últimos segmentos puxados */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* puxado por 'pusha' */
    unsigned int int_no, err_code;                        /* nosso 'push byte #' e ecodes (códigos de erro) faz isto */
    unsigned int eip, cs, eflags, useresp, ss;            /* puxados pelo processador automaticamente */ 
};

/* MAIN.C */
extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern int strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

/* CONSOLE.C */
extern void cls();
extern void putch(unsigned char c);
extern void puts(unsigned char *str);
extern void init_video();
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);

/* GDT.C */

extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();

/* IDT.C */
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();

/* ISRS.C */
extern void isrs_install();



#endif
