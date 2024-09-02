/* KBDUS é o Layout de Teclado dos EUA. Este é uma tabela de scancode
*  usado para o layout padrão do teclado dos EUA. Eu tenho adicionado alguns
*  comentários para você ter uma idéia de que tecla é qual, ainda
*  que Eu atribui este índice do arranjo para 0. Você pode mudar para você usar
*  qualquer que você deseja usando um macro, se você quiser! */

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=',
 '\b',	/* Backspace */
  '\t', /* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']',
 '\n',	/* Tecla enter */
    0,	/* 29 - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',
    0, /* Shift esquerdo */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',
    0, /* Shift direito */
  '*',
    0,	/* Alt */
  ' ',	/* Barra de espaço */
    0,	/* Caps lock */
    0,	/* tecla 59 - F1 ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock */
    0,	/* Scroll Lock */
    0,	/* Tecla Home */
    0,	/* Seta para cima */
    0,	/* Page Up */
  '-',
    0,	/* Seta esquerda */
    0,
    0,	/* Seta direita */
  '+',
    0,	/* tecla 79 - End */
    0,	/* Seta para baixo */
    0,	/* Page Down */
    0,	/* Tecla Insert */
    0,	/* Tecla Delete */
    0,   0,   0,
    0,	/* Tecla F11 */
    0,	/* Tecla F12 */
    0,	/* Todas as outras teclas são indefinidas */
};

/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r)
{
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inportb(0x60);

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80)
    {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
    }
    else
    {
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */

        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */
        putch(kbdus[scancode]);
    }
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install()
{
    irq_install_handler(1, keyboard_handler);
}