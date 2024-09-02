
#include <system.h>

int timer_ticks = 0;

void timer_handler(struct regs *r)
{
    timer_ticks++;

    if (timer_ticks % 18 == 0)
    {
    }
}

void timer_wait(int ticks)
{
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install()
{
    /* Installs 'timer_handler' to IRQ0 */
    irq_install_handler(0, timer_handler);
}