/*
Iago Lucini da Silva    281244
Maria Clara martinez    281315
*/

#include <avr/interrupt.h>

void enviar_mensagem(const char *mensagem);

//interrupções do temporizador
volatile unsigned char *p_TCCR0A = (unsigned char *) 0x44;
volatile unsigned char *p_TCCR0B = (unsigned char *) 0x45;
volatile unsigned char *p_TIMSK0 = (unsigned char *) 0x6E;

// USART Baud Rate 0 Register Low / High
volatile unsigned char *p_UDR0 = (unsigned char *) 0xC6;

volatile unsigned char *p_UBRR0L = (unsigned char *) 0xC4; 
volatile unsigned char *p_UBRR0H = (unsigned char *) 0xC5;

// USART0 Control and Status Register A
volatile unsigned char *p_UCSR0A = (unsigned char *) 0XC0;
// USART0 Control and Status Register B
volatile unsigned char *p_UCSR0B = (unsigned char *) 0XC1;
// USART0 Control and Status Register C
volatile unsigned char *p_UCSR0C = (unsigned char *) 0XC2;

//registradores do led
unsigned char *p_ddrb = (unsigned char *) 0x24;
volatile unsigned char *p_portb = (unsigned char *) 0x25;


volatile int contador_USART = 0, contador_12 = 0, contador_13 = 0;

volatile const char *tx_buffer = 0;
volatile uint8_t tx_posicao = 0;    // Guarda a posição do caracter de transmissao

const char msg[] = "Atividade 5 – Interrupcoes periodicas do temporizador permitem a temporização de processos do sistema sem espera ativa! \n\n";

void inicializa()
{
    //habilita ligar o led pelo software dos pinos 12 (bit 4) e pino 13 (bit 5)
    *p_ddrb |= 0x30;    // 0b00110000

    
    cli();
    
    // Temporizador
    /*
    f_cpu = 16000000 Hz
    f_desejada = 1000 Hz -> queremos uma base de 1ms
    P = 64

    f_desejada = P * (1/f_cpu) * (OCR0A) ->
    OCR0A = (f_cpu / P * f_desejada) - 1
    OCR0A = 249
    */
    
    OCR0A = 249;
    
    *p_TIMSK0 = 0x02;
    *p_TCCR0A = 0x02;
    *p_TCCR0B = 0b00000011;

    // USART
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
    *p_UCSR0B = 0x48;   // Habilitando TX e transmissão completa
    *p_UCSR0C = 0x06;

    sei();
}

// Parte responsavel por enviar/transmitir a mensagem

ISR (USART_TX_vect) // USART TRANSMIT COMPLETE
{
    tx_posicao++;                           // Avança para o próximo caracter

    // Verfica se não ao final da string
    if(tx_buffer[tx_posicao] != '\0')
    {
        *p_UDR0 = tx_buffer[tx_posicao];    // Escreve o caracter
    }
}


void enviar_mensagem(const char *mensagem)
{    
    tx_buffer = mensagem;

    tx_posicao = 0;
    *p_UDR0 = tx_buffer[tx_posicao];
}

ISR (TIMER0_COMPA_vect) 
{

    contador_12++;
    contador_13++;
    contador_USART++;


    // Pisca o pino 13 (bit 5): 0,5s aceso e 0,5s apagado
    if(contador_13 >= 500)
    {
        if((*p_portb & 0x20) == 0)
        {
            *p_portb |= 0x20;       //liga
        }
        else
        {
            *p_portb &= (~0x20);    //desliga    
        }

        contador_13 = 0;
    }

    // Pisca o pino 12 (bit 4): 0,78s aceso e 0,78s apagado
    if(contador_12 >= 780)
    {
        if((*p_portb & 0x10) == 0)
        {
            *p_portb |= 0x10;       //liga
        }
        else
        {
            *p_portb &= (~0x10);    //desliga    
        }

        contador_12 = 0;
    }

    if(contador_USART >= 5000)
    {
        enviar_mensagem(msg);
        contador_USART = 0;
    }
}






/*
O desafio proposto nesta atividade é desenvolver um programa que faça com que dois LEDs pisquem em
diferentes frequências, ao mesmo tempo em que uma mensagem de texto é transmitida pela UART a cada
5s. Mais especificamente, desejamos que o LED conectado ao pino 13 pisque com uma frequência de 1 Hz
(0,5 s aceso, 0,5 s apagado), e que o LED conectado ao pino 12, pisque de forma a ficar 0,78 s aceso e 0,78 s
apagado.

Em relação à UART, vamos utilizar a mesma configuração do exercício para casa entregue na atividade 4
(interrupções). A cada 5s, a seguinte mensagem deve ser enviada:
*/

int main(void) {
    inicializa();

    while (1);

    return 0;
}