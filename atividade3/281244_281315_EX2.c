#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//registradores de interrupção PCINT
unsigned char *p_pcicr  = (unsigned char *) 0x68; //PCINT Control Register
unsigned char *p_pcmsk2 = (unsigned char *) 0x6D; //PCINT Mask Register 2 (Porta D)
unsigned char *p_pcifr  = (unsigned char *) 0x3B; //PCINT Flag Register

unsigned char *p_ddrb = (unsigned char *) 0x24;
unsigned char *ponteiro_ddrd = (unsigned char *) 0x2A;
unsigned char *ponteiro_portb = (unsigned char *) 0x25;
unsigned char *ponteiro_pind = (unsigned char *) 0x29;

int atual;
int atual_interrompe;
int anterior=0;
int interrompeu=0;

void inicializa(){
    cli();

    *p_ddrb = *p_ddrb | 0x20;
    *ponteiro_ddrd = *ponteiro_ddrd & (~0x4);

    //habilita a interrupção PCINT para o grupo 2 e pino específico
    *p_pcicr = *p_pcicr | 0x04;   //habilita o bit PCIE2 (Interrupções PCINT no grupo da Porta D)
    *p_pcmsk2 = *p_pcmsk2 | 0x04; //habilita o bit PCINT18 (correspondente ao pino PD2)

    sei();
}

ISR (PCINT2_vect) {
    interrompeu = 1;
    atual_interrompe = *ponteiro_pind & 0x4; //faz leitura para ser comparada 
    *p_pcmsk2 = *p_pcmsk2 & (~0x04); //desabilita interrupção
}

int main(void) {
    inicializa();

    //leitura inicial
    anterior = *ponteiro_pind & 0x4;

    while (1) {
        if(interrompeu) { //mudança de estado
            
            _delay_ms(15); //espera transiente
            atual = *ponteiro_pind & 0x4; //faz leitura depois do delay      
            if((atual==atual_interrompe) && anterior==0x4 && atual == 0){ //Confer se houve uma borda de descida e se foi ruído conferindo se o valor é mantido após o delay
                // Alterna o estado do LED
                if((*ponteiro_portb & 0x20)==0){
                    *ponteiro_portb = *ponteiro_portb | 0x20; // Liga
                }
                else{
                    *ponteiro_portb = *ponteiro_portb & (~0x20); // Desliga
                }
            }

            anterior=atual;
            
            interrompeu = 0;
            
            *p_pcifr = *p_pcifr | 0x04; //zera a flag
            
            //habilita interrupção
            *p_pcmsk2 = *p_pcmsk2 | 0x04; 
        }
    }

    return 0;
}