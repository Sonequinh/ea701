// Iago Lucini da Silva RA 281244
// Maria Clara Martinez Oliveira RA 281315

#include <avr/interrupt.h>

//interrupções do temporizador
unsigned char *p_TCCR0A = (unsigned char *) 0x44;
unsigned char *p_TCCR0B = (unsigned char *) 0x45;
unsigned char *p_TIMSK0 = (unsigned char *) 0x6E;

//registradores do led
unsigned char *p_ddrb = (unsigned char *) 0x24;
volatile unsigned char *p_portb = (unsigned char *) 0x25;

//registradores botão
unsigned char *p_pind = (unsigned char *) 0x29;
unsigned char *p_ddrd = (unsigned char *) 0x2A;

//variaveis maquina debouncer
int estadoDB=0;
volatile int contador=0;
volatile int filtrado=1;
int contagem_init=1;

//variaveis maquina LED
int estadoLED=0;

void inverte_led()
{
    if((*p_portb & 0x20) == 0){
        *p_portb |= 0x20;       // Liga
    }
    else
    {
        *p_portb &= (~0x20);    // Desliga
    }
}

void deteccao_borda(int filtrado) 
{
    switch (estadoLED)
    {
        case (0):

            // Detecta borda de descida
            if (filtrado == 0)
            {
                estadoLED = 1;
                inverte_led();
            }
            else if (filtrado == 1)
            {
                estadoLED = 0;
            }
            break;
        
        case (1):
            if (filtrado == 0)
            {
                estadoLED = 1;
            }
            else if (filtrado == 1)
            {
                estadoLED = 2;
                inverte_led();
            }
            break;
        
        case (2):
            if (filtrado == 0)
            {
                estadoLED = 3;
                inverte_led();
            }
            else if (filtrado == 1)
            {
                estadoLED = 2;
            }
        case (3):
            if (filtrado == 0)
            {
                estadoLED = 3;
            }
            else if (filtrado == 1)
            {
                estadoLED = 0;
                inverte_led();
            }
    }
}

void inicializa(){
    
    //habilita ligar o led pelo software
    *p_ddrb = *p_ddrb | 0x20;
    //quem influencia o valor eh o botão, n a entrada do código
    *p_ddrd = *p_ddrd & (~0x4);
    //desligado
    *p_portb &= ~(0x20);
    cli();
    *p_TCCR0A=0x00;
    *p_TCCR0B=0x02;
    *p_TIMSK0=0x01;
    sei();
}

ISR (TIMER0_OVF_vect) {
    contador++;
}

int main(void) {
    inicializa();

    while (1){
        int bt=*p_pind & 0x4;
        switch(estadoDB){
            case 0:
                filtrado=1;
                if(bt){
                    estadoDB=0;
                }
                else{
                    estadoDB=1;
                    if(contagem_init){
                        contagem_init=0;
                        contador=0;
                    }
                }
            case 1:
                if(contador>=156){
                    contagem_init=1;
                    if(bt){
                        estadoDB=0;
                        filtrado=1;
                    }
                    else{
                        estadoDB=2;
                        filtrado=0;
                    }
                }
                break;
            case 2:
                filtrado=0;
                if(bt){
                    estadoDB=3;
                    if(contagem_init){
                        contagem_init=0;
                        contador=0;
                    }
                }
                else{
                    estadoDB=2;
                }
                break;
            case 3:
                if(contador>=156){
                    contagem_init=1;
                    if(bt){
                        estadoDB=0;
                        filtrado=1;
                    }
                    else{
                        estadoDB=2;
                        filtrado=0;
                    }
                }
                break;
        }
        deteccao_borda(filtrado);
    } 

    return 0;
}