/*
Iago Lucini da Silva 281244
Maria Clara Martinez 281315
*/
#define F_CPU 16000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Iniciando os ponteiros
// interrupções do temporizador
volatile unsigned char *p_TIMSK1 = (unsigned char *) 0x6F;

volatile unsigned char *p_TCCR1A = (unsigned char *) 0x80;
volatile unsigned char *p_TCCR1B = (unsigned char *) 0x81;
volatile unsigned char *p_DDRB   = (unsigned char *) 0x24;



// Variaveis Globais
// Lista que percorre os angulos de 0 até 180 e depois volta para 30
const int angulos[] = {
    1000, 1667, 2333, 3000, 3667, 4333, 
    5000, 4333, 3667, 3000, 2333, 1667
};
// Contador de interrupções
volatile int contador = 0;
volatile int indice = 1;        // Iniciado como 1 pois o servomotor já começa na posição 0


// Inicializa os pinos e bits
void inicializar () 
{
    // Habilita o pino ~10 como saída
    *p_DDRB |= 0x04;

    OCR1B = angulos[0];

    cli();

    ICR1 = 39999;
    
    *p_TIMSK1 = 0x01;                  
    *p_TCCR1A = 0x22;             
    *p_TCCR1B = 0x1A;             

    sei();

    // Definindo posição inicial do servo
}

ISR(TIMER1_OVF_vect) {
    contador++;

    // 40 ciclos de 20 ms = 800 ms
    if (contador >= 40) {
        contador = 0;

        // Atualiza o registrador OCR1B com o novo ângulo
        OCR1B = angulos[indice];

        // Passa para o proximo angulo
        indice++;
        if (indice >= 12) {
            indice = 0; // Volta ao início do vetor
        }
    }
}

/*
Construa um programa que repita indefinidamente o seguinte movimento do eixo: deslocamento do
ângulo mínimo até o máximo e depois retorno até o ângulo mínimo, em passos de 30°. Ou seja, cada ciclo
do movimento deve ser composto pela seguinte sequência de ângulos: 0°, 30°, 60°, 90°, 120°, 150°, 180°,

150°, 120°, 90°, 60° e 30°. O eixo deve permanecer 800ms em cada ângulo e os ciclos devem se repetir
indefinidamente.
Para definir as configurações que são necessárias para o temporizador gerar o sinal de controle conforme
especificado, os seguintes passos podem ser seguidos:
    1. Defina o modo de operação a ser usado;
    2. Verifique qual é o valor de prescaler que permite a geração de um sinal PWM de 50Hz, ou seja,
    considerando a configuração escolhida, qual é o máximo período que pode ser obtido para cada
    possível valor de prescaler;
    3. Ajuste a faixa de contagem de forma a obter exatamente um período de 20ms (50Hz);    
    4. Calcule a largura do pulso associada a cada ângulo do ciclo;
    5. Considerando o prescaler escolhido, calcule o número de ciclos de contagem necessários para se
    obter cada uma das larguras de pulso.
    Utilizar temporizador 1
*/
int main ()
{

    inicializar();
    while (1);
    
}