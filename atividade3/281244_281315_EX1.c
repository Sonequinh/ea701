#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char *eimsk = (unsigned char *) 0x3D;
unsigned char *eifr = (unsigned char *) 0x3C; //flags
unsigned char *eicra = (unsigned char *) 0x69;
unsigned char *p_ddrb = (unsigned char *) 0x24;
unsigned char *ponteiro_ddrd = (unsigned char *) 0x2A;
unsigned char *ponteiro_portb = (unsigned char *) 0x25;
unsigned char *ponteiro_pind = (unsigned char *) 0x29;
int atual;
int interrompeu=0;


void inicializa(){
cli();

*p_ddrb = *p_ddrb | 0x20;
*ponteiro_ddrd = *ponteiro_ddrd & (~0x4);

*eicra=0b10; //define qual evento é detectado para uma certa interrupção (borda de descida)
*eimsk=0b01; //habilita ou desabilita uma interrupção (neste caso int0)

sei();
}

ISR (INT0_vect) {
interrompeu=1;
}

int main(void) {
inicializa();

while (1) {
if(interrompeu){//borda de descida
_delay_ms(15); //espera transiente
atual=*ponteiro_pind & 0x4;
if(atual == 0){//mede de novo depois do delay
if((*ponteiro_portb & 0x20)==0){
*ponteiro_portb = *ponteiro_portb | 0x20;//liga
}
else{
*ponteiro_portb = *ponteiro_portb & (~0x20);//desliga
}
}
interrompeu=0;
}

}


return 0;
}