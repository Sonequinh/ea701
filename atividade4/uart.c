/*
 * File:   exercicio.c
 * Author: i281244
 *
 * Created on September 16, 2026, 9:43 AM
 */
#define F_CPU 16000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Configurações de UART
unsigned char *p_UDR0 = (unsigned char *) 0xc6;

unsigned char *p_UBRR0L = (unsigned char *) 0xc4;
unsigned char *p_UBRR0H = (unsigned char *) 0xc5;

unsigned char *p_UCSR0A = (unsigned char *) 0XC0;
unsigned char *p_UCSR0B = (unsigned char *) 0XC1;
unsigned char *p_UCSR0C = (unsigned char *) 0XC2;

// Outras configurações (led)
unsigned char *p_portb = (unsigned char *) 0x25;
unsigned char *p_ddrb = (unsigned char *) 0x24;

unsigned char *ponteiro_pind = (unsigned char *) 0x29; // 

// Variaveis globais
char msg_TXC0 [] = "Transmissao serial utilizando a interrupcao USART Transmit Complete. \n\n";
unsigned int indice = 0;
char letra;
char val_escrito;


// Funções

void configuracoes_iniciais() 
{
    *p_ddrb |= 0x20;
    
    cli(); // Interrompendo interrupções
    *p_UBRR0H = 0;
    *p_UBRR0L = 16;
    
    *p_UCSR0A = 0x22;
    *p_UCSR0B = 0xC8; //tcham
    *p_UCSR0C = 0x06;
    sei(); // Liberando interrupções

}


ISR (USART_TX_vect) {
    
    if (msg_TXC0[indice] == '\0')
    {
        indice = 0;
    } else
    {
        letra = msg_TXC0[indice];
        *p_UDR0 = letra;
        indice++;    
    }
    
}

ISR (USART_RX_vect) {



}


int main(void) 
{
    
    configuracoes_iniciais();

    
    
    while (1) 
    {
        
        if(indice == 0)
        {
            // Escrevendo a primeira letra
            letra = msg_TXC0[indice];
            *p_UDR0 = letra;
            indice++;
        } 
        
        
         
        // Acender o led e desligar led com o delay
        *p_portb |= 0x20;
        _delay_ms(500);
        *p_portb &= (~0x20);
        
    }
}


