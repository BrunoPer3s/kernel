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
extern void scroll(void);
extern void putch(unsigned char c);
extern void puts(unsigned char *str);
extern void print_decimal(unsigned int num);
extern void print_time(unsigned int hours, int minutes, int seconds);
extern void print_date(unsigned int day, unsigned int month, unsigned int year);
extern void init_video();
extern void clear_area(unsigned int x_start, unsigned int y_start, unsigned int width, unsigned int height);
extern void putch_at(unsigned char text, unsigned int px, unsigned int py);
extern void print_at_position(unsigned int x, unsigned int y, unsigned char text);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);

/* GDT.C */

extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();
extern void showTime();

/* IDT.C */
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();

/* ISRS.C */
extern void isrs_install();

/* IRQ.C */
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_install();

/* KEYBOARD.C */
extern void keyboard_install();

/* TIMER.C */
extern void timer_wait(int ticks);
extern void timer_install();

/* RTC.C */
extern unsigned char read_rtc();
void printValues();
void init_timer(int frequency);
void timer_callback();
void init_timer_interrupt();




#endif
