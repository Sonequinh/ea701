

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Configurações de UART
// 
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
    *p_UCSR0B = 0xD8; //
    *p_UCSR0C = 0x07;

    rei()

}

// Tratar quando o comando é enviado pelo terminal
ISR (USART_RX_vect) // USART RECEIVE COMPLETE
{

}

// Enviar mensagem
ISR (USART_TX_vect) // USART TRANSMIT COMPLETE
{

}


//
int main () 
{
    configuracoes_inicias();

    while (1)
    {

    }

}