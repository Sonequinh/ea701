#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Configurações de UART
unsigned char *p_UDR0 = (unsigned char *) 0xC6;

unsigned char *p_UBRR0L = (unsigned char *) 0xC4;
unsigned char *p_UBRR0H = (unsigned char *) 0xC5;

unsigned char *p_UCSR0A = (unsigned char *) 0XC0;
unsigned char *p_UCSR0B = (unsigned char *) 0XC1;
unsigned char *p_UCSR0C = (unsigned char *) 0XC2;

void configuracoes_inicias() 
{
    
}

ISR ()
{

}

int main () 
{

}