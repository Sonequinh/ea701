/*
Alunos: Iago Lucini da Silva 281244, Maria Clara Martinez 281315
*/


#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Definição dos ponteiros para os registradores (Substituindo as macros)
unsigned char *p_pcicr  = (unsigned char *) 0x68;
unsigned char *p_pcmsk1 = (unsigned char *) 0x6C;
unsigned char *p_ddrb   = (unsigned char *) 0x24;
unsigned char *p_portb  = (unsigned char *) 0x25;
unsigned char *p_ddrc   = (unsigned char *) 0x27;
unsigned char *p_portc  = (unsigned char *) 0x28;
unsigned char *p_pinc   = (unsigned char *) 0x26;
unsigned char *p_ddrd   = (unsigned char *) 0x2A;
unsigned char *p_portd  = (unsigned char *) 0x2B;

// Posições 0 a 9 representam os bits para acender os segmentos: a,b,c,d,e,f,g
// display_7seg[] deve mostrar no display {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
unsigned char display_7seg[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};


// Variável global para contabilizar os pulsos lidos pelo encoder
volatile unsigned int contador_pulsos = 0;

// Rotina de Serviço de Interrupção (RSI) para Pin Change Interrupt da Porta C (PCINT1)
ISR(PCINT1_vect) {
    // Verifica se a mudança de estado foi uma borda de subida no pino A3 (PC3)
    if ((*p_pinc & 0x08) != 0) { 
        contador_pulsos++;
    }
}

void inicializa() {
    cli(); // Desabilita as interrupções globais durante a configuração

    // 1. Configuração do pino A3 (PC3) como entrada (Encoder)
    *p_ddrc  = (*p_ddrc & ~(0x08)) | 0x33; // Zera o bit 3 do DDRC (configura como entrada)
    *p_portc = *p_portc | 0x08;  // Habilita o resistor de pull-up interno no PC3

    // 2. Configuração dos pinos dos Displays de 7 segmentos como saídas
    *p_ddrd = 0xFF;           // Pinos 0 a 7 do Arduino como saída (Display Direito + Seg. A do Esquerdo)
    *p_ddrb = *p_ddrb | 0x3F; // Pinos 8 a 13 do Arduino como saída (Segmentos B a G do Display Esquerdo)

    // 3. Configuração da interrupção (PCINT11 - mapeado no A3)
    *p_pcicr = *p_pcicr | 0x02;   // Habilita interrupção no grupo PCIE1 (Porta C)
    *p_pcmsk1 = *p_pcmsk1 | 0x08; // Habilita a máscara especificamente para o pino PCINT11 (PC3/A3)

    sei(); // Reabilita as interrupções globais
}

// Função para aplicar os valores calculados aos registradores dos displays
void escreve_displays(int centena, int dezena) {
    unsigned char seg_centena = display_7seg[centena];
    unsigned char seg_dezena = display_7seg[dezena];
    /*
    O display das unidades está com os segmentos (a, b, c, d, e, f, g)
    ligados em  (PC1, PC4, PB0, PD5, PD2, PD3, PB2)

    O display das dezenas está com os segmentos (a, b, c, d, e, f, g)
    ligados em  (PC5, PB1, PD4, PB3, PD7, PB5, PC0) 
    */

    // Dezena => Display da DIREITA
    if (seg_dezena & (1 << 0)) *p_portc |= (1 << 1); else *p_portc &= ~(1 << 1); // Seg a -> PC1
    if (seg_dezena & (1 << 1)) *p_portc |= (1 << 4); else *p_portc &= ~(1 << 4); // Seg b -> PC4
    if (seg_dezena & (1 << 2)) *p_portb |= (1 << 0); else *p_portb &= ~(1 << 0); // Seg c -> PB0
    if (seg_dezena & (1 << 3)) *p_portd |= (1 << 5); else *p_portd &= ~(1 << 5); // Seg d -> PD5
    if (seg_dezena & (1 << 4)) *p_portd |= (1 << 2); else *p_portd &= ~(1 << 2); // Seg e -> PD2
    if (seg_dezena & (1 << 5)) *p_portd |= (1 << 3); else *p_portd &= ~(1 << 3); // Seg f -> PD3
    if (seg_dezena & (1 << 6)) *p_portb |= (1 << 2); else *p_portb &= ~(1 << 2); // Seg g -> PB2

    // Centena => Display da ESQUERDA
    if (seg_centena & (1 << 0)) *p_portc |= (1 << 5); else *p_portc &= ~(1 << 5); // Seg a -> PC5
    if (seg_centena & (1 << 1)) *p_portb |= (1 << 1); else *p_portb &= ~(1 << 1); // Seg b -> PB1
    if (seg_centena & (1 << 2)) *p_portd |= (1 << 4); else *p_portd &= ~(1 << 4); // Seg c -> PD4
    if (seg_centena & (1 << 3)) *p_portb |= (1 << 3); else *p_portb &= ~(1 << 3); // Seg d -> PB3
    if (seg_centena & (1 << 4)) *p_portd |= (1 << 7); else *p_portd &= ~(1 << 7); // Seg e -> PD7
    if (seg_centena & (1 << 5)) *p_portb |= (1 << 5); else *p_portb &= ~(1 << 5); // Seg f -> PB5
    if (seg_centena & (1 << 6)) *p_portc |= (1 << 0); else *p_portc &= ~(1 << 0); // Seg g -> PC0


}

int main(void) {
    inicializa();
    
    int frequencia_rpm = 0;
    unsigned int centena = 0;
    unsigned int dezena = 0;
    unsigned int pulsos_lidos = 0;

    while (1) {
        // Zera os pulsos e aguarda a janela de tempo de 1 segundo (1000 ms)

        //
        cli();
        pulsos_lidos = contador_pulsos;
        contador_pulsos = 0;
        sei();


        _delay_ms(1000); 

        // Cálculo da frequência usando a fórmula do roteiro:
        // f_rpm = numero_pulsos / (pulsos_por_volta * intervalo_em_minutos)
        // Intervalo = 1 segundo = (1/60) minutos. 
        // f_rpm = (pulsos / 49) / (1/60) => f_rpm = (pulsos * 60) / 49
        frequencia_rpm = ((uint32_t)contador_pulsos * 60) / 49;

        // Extrai a centena e a dezena do RPM calculado
        centena = (frequencia_rpm / 100) % 10;
        dezena = (frequencia_rpm / 10) % 10;

        // Imprime os valores atuais nos displays correspondentes
        escreve_displays(centena, dezena);
    }

    return 0;
}