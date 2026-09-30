/*
Iago Lucini da Silva 281244
Maria Clara Martinez 281315
*/

#define F_CPU 16000000UL

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
volatile int interrompeu = 0;

// transmissão
volatile char tx_buffer[20];
volatile uint8_t tx_posicao = 0;    // Guarda a posição do caracter de transmissao
volatile uint8_t tx_ocupado = 0;

// recepção
volatile char rx_buffer[20];        // ->
volatile uint8_t rx_posicao = 0;    // Guarda a posição de onde o próximo caracter será salvo




//
void configuracoes_inicias() 
{
    cli();

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
    *p_UBRR0L = 51;

    *p_UCSR0A = 0x20;
    *p_UCSR0B = 0xD8; 
    *p_UCSR0C = 0x06;

    // LEDS
    // Vermelho PD3
    // Verde PD5
    // Azul PD6
    *p_DDRD |=  (1 << 3) | (1 << 5) | (1 << 6);
    *p_PORTD &= ~((1 << 3) | (1 << 5) | (1 << 6));

    sei();

}

void enviar_mensagem(const char* mensagem)
{
    // Aguarda o canal de transmissão ser liberado
    while (tx_ocupado);
    

    uint8_t i = 0;
    while (mensagem[i] != '\0' && i < (sizeof(tx_buffer) - 1)) 
    {
        tx_buffer[i] = mensagem[i];
        i++;
    }
    tx_buffer[i] = '\0'; // Garante o caractere nulo no final

    tx_posicao = 0;
    tx_ocupado = 1;

    // Dispara a transmissão do primeiro caractere
    *p_UDR0 = tx_buffer[0];
}


// Recepção
ISR (USART_RX_vect) // USART RECEIVE COMPLETE
{
    //guarda o valor recebido
    char_recebido= *p_UDR0;
    if (char_recebido == '\r' || char_recebido == '\n')
    {
        if (rx_posicao > 0 && !interrompeu)
        {
            rx_buffer[rx_posicao] = '\0';
            interrompeu = 1;
        }
    } else 
    {
        if ((rx_posicao < (sizeof(rx_buffer) - 1)) && (!interrompeu))
        {
            rx_buffer[rx_posicao] = char_recebido;
            rx_posicao += 1;
        }   
    }

}

// Transmissão
ISR (USART_TX_vect) // USART TRANSMIT COMPLETE
{
    tx_posicao++;                           // Avança para o próximo caracter

    if(tx_buffer[tx_posicao] != '\0')
    {
        *p_UDR0 = tx_buffer[tx_posicao];    // Escreve o caracter
    } else
    {
        tx_ocupado = 0;                     // Libera o canal
    }
}


//
int main () 
{
    configuracoes_inicias();

    while (1)
    {
        if (interrompeu) 
        {
            
            if (rx_buffer[1] == '\0') 
            {
                switch (rx_buffer[0]) 
                {
                    case 'd': // Desligado
                        *p_PORTD &= ~((1 << 3) | (1 << 5) | (1 << 6));
                        enviar_mensagem("Desligado\n");
                        break;

                    case 'r': // Vermelho
                        *p_PORTD &= ~((1 << 5) | (1 << 6));
                        *p_PORTD |= (1 << 3);
                        enviar_mensagem("Pisca vermelho\n");
                        break;

                    case 'g': // Verde
                        *p_PORTD &= ~((1 << 3) | (1 << 6));
                        *p_PORTD |= (1 << 5);
                        enviar_mensagem("Pisca verde\n");
                        break;

                    case 'b': // Azul
                        *p_PORTD &= ~((1 << 3) | (1 << 5));
                        *p_PORTD |= (1 << 6);
                        enviar_mensagem("Pisca azul\n");
                        break;

                    case 't': // Todos
                        *p_PORTD |= (1 << 3) | (1 << 5) | (1 << 6);
                        enviar_mensagem("Pisca todos\n");
                        break;

                    default:
                        enviar_mensagem("Comando incorreto\n");
                        break;
                }
            } 
            else 
            {
                        enviar_mensagem("Comando incorreto\n");
            }

            rx_posicao = 0; 
            interrompeu = 0;
        }

    }
}