#include <system.h>

/* Este define nosso ponteiro de texto, nossas cores de fundo e
*  primeiro plano (atributos), e as coordenadas x e y */

unsigned short *textmemptr;
int attrib = 0x07;
int csr_x = 0, csr_y = 0;

/* Rola a tela */

void scroll(void) {
    unsigned blank, temp;

    /* Um vazio é definido com um espaço... nós devemos por isto
    *  com a cor de fundo também */

    blank = 0x20 | (attrib << 8);

    /* Linha 25 é o fim, estes nós são necessários para rolar para cima */

    if(csr_y >= 25) {

        /* Move o pedaço de texto corrente para cima da tela
        *  para fazer o buffer voltar linha */

        temp = csr_y - 25 + 1;
        memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

        /* Finalmente, nós atribuímos o pedaço de memória que ocupa
        *  a ultima linha do texto para nosso caractere 'vazio' */

        memsetw (textmemptr + (25 - temp) * 80, blank, 80);
        csr_y = 25 - 1;
    }
}

/* Atualiza o cursor do hardware: a pequena linha piscando
*  na tela sobre o ultimo caractere pressionado! */
void move_csr(void)
{
    unsigned temp;

    /* A equação para descobrir o índice de um pedaço
    *  linear da memória pode ser representada por:
    *  Índice = [(y * largura) + x] */
    if(csr_x != 0) {

    }
    temp = csr_y * 80 + csr_x;

    /* Este manda um comando para os índices 14 e 15 no
    *  Registrado de Controle CRT do controle VGA. Esses
    *  são os bytes superior e inferior do índice que mostra
    *  onde o cursor do hardware está 'piscando'. Para
    *  aprender mais, você deveria procurar alguns documentos específicos
    *  de programação para VGA. Um grande começo para gráficos:
    *  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

/* Limpa a tela */
void cls() {
    unsigned blank;
    int i;

    /* Outra vez, nós necessitamos o 'short' que vai ser usada para
    *  representar um espaço com cor */

    blank = 0x20 | (attrib << 8);

    /* Atribui a tela inteira com espaços para nossa cor
    *  corrente */

    for(i = 0; i < 25; i++) {
        memsetw (textmemptr + i * 80, blank, 80);
    }
        

    /* Atualiza o cursor virtual, e então move o
    *  cursor do hardware */

    csr_x = 0;
    csr_y = 0;
    move_csr();
}


/* Coloca um caractere único na tela */

void putch(unsigned char c) {
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Trata um backspace, recuando o cursor um espaço */

    if(c == 0x08) {
        if(csr_x != 0) csr_x--;
    }
    /* Trata um tab incrementando o cursor x, mas somente
    *  para um ponto que seja divisível por 8 */

    else if(c == 0x09) {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* Trata um 'Retorno de Carruagem', que simplesmente traz o
    *  cursor novamente para a margem */

    else if(c == '\r') {
        csr_x = 0;
    }
    /* Nós tratamos nossas novas linhas (newlines) do mesmo modo do DOS e BIOS fazem: nós
    *  trataremos isso como se um 'CR' estivesse lá também, assim nós trazemos o
    *  cursor para a margem e nós incrementamos o valor 'y' */

    else if(c == '\n') {
        csr_x = 0;
        csr_y++;
    }
    /* Qualquer caractere maior que e incluindo um espaço, é um
    *  caractere que pode ser impresso. A equação para achar o índice
    *  em um pedaço da memória linear pode ser representado por:
    *  Índice = [(y * largura) + x] */

    else if(c >= ' ') {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;	/* Caractere E atributos: cor */
        csr_x++;
    }

    /* Se o cursor alcançou a margem de largura da tela, nós
    *  inserimos uma nova linha lá */

    if(csr_x >= 80) {
        csr_x = 0;
        csr_y++;
    }

    /* Rola a tela se necessário, e finalmente move o cursor */

    scroll();
    move_csr();
}

void print_at_position(unsigned int x, unsigned int y, unsigned char text) {
    csr_x = x;
    csr_y = y;
    print_decimal(text);
}

/* Usa a rotina acima para escrever uma string... */

void puts(unsigned char *text) {
    for (int i = 0; i < strlen(text); i++) {
        putch(text[i]);
    }
    
    putch('\n');

}

void decrement_csr_x() {
    csr_x--;
}

int get_csr_x() {
    return csr_x;
}

void print_decimal(unsigned int num) {
    if(num == 0) return;
    print_decimal(num/10);
    putch(num % 10 + '0');
}

void print_time(unsigned int hours, int minutes, int seconds) {
    csr_x = 63;
    csr_y = 0;
    print_decimal(hours);
    putch(':');
    if(minutes<10) {
        putch('0');
    }
    print_decimal(minutes);
    putch(':');
    if(seconds < 10) {
        putch('0');
    }
    print_decimal(seconds);

}

void print_date(unsigned int day, unsigned int month, unsigned int year) {
    print_decimal(day);
    putch('/');
    print_decimal(month);
    putch('/');
    print_decimal(year);
    csr_x = 0;
    
}

void putch_at(unsigned char text, unsigned int px, unsigned int py) {
    csr_x = px;
    csr_y = py;
    putch(text);
}

void clear_area(unsigned int x_start, unsigned int y_start, unsigned int width, unsigned int height) {
    int x, y;
    for(y = y_start; y < y_start + height; y++) {
         for(x = x_start; x < x_start + width; x++) {
            putch_at(' ', x, y);
         }
    }
}

void save_cursor_position(int *x, int *y) {
    *x = csr_x;
    *y = csr_y;
}

void restore_cursor_position(int x, int y) {
    csr_x = x;
    csr_y = y;
    move_csr();
}

/* Atribui o primeiro plano e o fundo que nós usaremos */

void settextcolor(unsigned char forecolor, unsigned char backcolor) {
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Atribui nosso ponteiro de modo-texto VGA, então limpa a tela para nós */
void init_video(void) {
    textmemptr = (unsigned short *)0xB8000;
    cls();
}
