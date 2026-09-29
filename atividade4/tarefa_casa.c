/*
Iago Lucini da Silva 281244
Maria Clara Martinez 281315
*/

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Configurações de UART 
unsigned char *p_UDR0 = (unsigned char *) 0xC6;

// USART Baud Rate 0 Register Low / High
unsigned char *p_UBRR0L = (unsigned char *) 0xC4; 
unsigned char *p_UBRR0H = (unsigned char *) 0xC5;

// USART0 Control and Status Register A
unsigned char *p_UCSR0A = (unsigned char *) 0XC0;
// USART0 Control and Status Register B
unsigned char *p_UCSR0B = (unsigned char *) 0XC1;
// USART0 Control and Status Register C
unsigned char *p_UCSR0C = (unsigned char *) 0XC2;

// Configurações de LED
unsigned char *p_PORTD = (unsigned char *) 0X2B;
unsigned char *p_DDRD = (unsigned char *) 0X2A;

// Variaveis globais:
char char_recebido;
int interrompeu = 0;

// transmissao
volatile char tx_buffer[20];





//
void configuracoes_inicias() 
{
    cli()

    /*
    CONFIGURANDO OS BITS:
        1) Velocidade de transmissão normal (i.e., modo double-speed desativado);
        2) Modo de transmissão multi-processador desabilitado;
        3) Número de bits de dados por frame igual a 8;
        4) Modo assíncrono de funcionamento da USART;
        5) Sem bits de paridade;
        6) Uso de um bit de parada;
        7) Baud rate igual a 19.200 bps.
    */

    *p_UBRR0H = 0;
    *p_UBRR0L = 16;

    *p_UCSR0A = 0x20;
    *p_UCSR0B = 0xD8; 
    *p_UCSR0C = 0x07;

    // LEDS
    // Vermelho PD3
    // Verde PD5
    // Azul PD6
    *p_DDRD |=  (1 << 3) | (1 << 5) | (1 << 6);
    *p_PORTD &= ~((1 << 3) | (1 << 5) | (1 << 6));

    rei()

}

void enviar_mensagem(const char* mensagem)
{

}


// Recepção
ISR (USART_RX_vect) // USART RECEIVE COMPLETE
{
    //guarda o valor recebido
    char_recebido= *p_UDR0;
    if (char_recebido == '\r' || char_recebido == '\n')
    {
        if ()
        {

        }
    } else {

    }
    interrompeu=1;

}

// Transmissão
ISR (USART_TX_vect) // USART TRANSMIT COMPLETE
{

}


//
int main () 
{
    configuracoes_inicias();

    while (1)
    {
        if(interrompeu) 
        {
            // red=pd3; green=pd5; blue=pd6
            if (strcmp((char*)rx_buffer, "d") == 0)         // "Desligado\n"
            {
                // Desliga todos os leds
                *p_PORTD &= ~((1 << 3) | (1 << 5) | (1 << 6));
            } else if (strcmp((char*)rx_buffer, "r") == 0)  // "Pisca vermelho\n"
            {   
                // Desliga verde e azul
                // Liga vermelho
                *p_PORTD &= ~((1 << 5) | (1 <<6));
                *p_PORTD |= (1 << 3);


            } else if (strcmp((char*)rx_buffer, "g") == 0)  // "Pisca verde\n"
            {
                // Desliga vermelho e azul
                // Liga verde
                *p_PORTD &= ~((1 << 3) | (1 <<6));
                *p_PORTD |= (1 << 5);
            } else if(strcmp((char*)rx_buffer, "b") == 0)   // "Pisca azul\n"
            {
                // Desliga vermelho e verde
                // Liga azul
                *p_PORTD &= ~((1 << 3) | (1 <<5));
                *p_PORTD |= (1 << 6);
            } else if(strcmp((char*)rx_buffer, "t") == 0)   // "Pisca todos\n"
            {
                // Liga vermelho, verde, azul
                *p_PORTD |= (1 << 3) | (1 << 5) | (1 << 6);

            } else                                          // "Comando incorreto\n"
            {

            }

            interrompeu = 0;
        }

    }

}