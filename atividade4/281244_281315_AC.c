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
unsigned char *p_PORTB = (unsigned char *) 0X25;
unsigned char *p_DDRB = (unsigned char *) 0X24;

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
    // Vermelho PINO 12 -> PB4
    // Verde PINO 11 -> PB3
    // Azul PINO 13 -> PB5
    *p_DDRB |=  (1 << 4) | (1 << 3) | (1 << 5);
    *p_PORTB &= ~((1 << 4) | (1 << 3) | (1 << 5));

    sei();

}

void enviar_mensagem(const char* mensagem)
{    

    tx_buffer = mensagem;

    tx_posicao = 0;
    *p_UDR0 = tx_buffer[0];
}


// Recepção
ISR (USART_RX_vect) // USART RECEIVE COMPLETE
{
    //guarda o valor recebido
    char_recebido= *p_UDR0;
    // Verifica se é final de linha
    if (char_recebido != '\r' && char_recebido != '\n')
    {   
        rx_buffer[rx_posicao] = char_recebido;   // Finaliza a string
        interrompeu = 1;                
    } 

}

// Transmissão
ISR (USART_TX_vect) // USART TRANSMIT COMPLETE
{
    tx_posicao++;                           // Avança para o próximo caracter

    // Verfica se não ao final da string
    if(tx_buffer[tx_posicao] != '\0')
    {
        *p_UDR0 = tx_buffer[tx_posicao];    // Escreve o caracter
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
                    // Vermelho PINO 12 -> PB4
                    // Verde PINO 11 -> PB3
                    // Azul PINO 13 -> PB5
                    case 'd': // Desligado
                        enviar_mensagem("Desligado\n"); 
                        *p_PORTB &= ~((1 << 4) | (1 << 3) | (1 << 5));
                    break;

                    case 'r': // Vermelho
                        
                        *p_PORTB |= (1 << 4);
                        _delay_ms(200;);
                        *p_PORTB &= ~(1 << 4);
                        _delay_ms(200;);

                        enviar_mensagem("Pisca vermelho\n");
                        break;

                    case 'g': // Verde

                        *p_PORTB |= (1 << 3);
                        _delay_ms(200;);
                        *p_PORTB &= ~(1 << 3);
                        _delay_ms(200;);

                        enviar_mensagem("Pisca verde\n");
                        break;

                    case 'b': // Azul

                        *p_PORTB |= (1 << 5);
                        _delay_ms(200;);
                        *p_PORTB &= ~(1 << 5);
                        _delay_ms(200;);

                        enviar_mensagem("Pisca azul\n");
                        break;

                    case 't': // Todos
                        *p_PORTB |= (1 << 3) | (1 << 4) | (1 << 5);
                        _delay_ms(200);
                        *p_PORTB &= ~((1 << 3) | (1 << 4) | (1 << 5));
                        _delay_ms(200);

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

            // Reseta a recepção
            rx_posicao = 0; 
            interrompeu = 0;
        }

    }
}
