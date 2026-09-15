#include <stdio.h>

const unsigned long int sequencia = 0x034A2D19; // ob00000011...00011001

int main()
{
    int comparador = 0xFF; 
    unsigned long int byte0, byte1, byte2, byte3;

    byte0 = (sequencia) & comparador;       // byte0 = 0x19 = 0b00011001
    byte1 = (sequencia >> 8) & comparador;  // byte1 = 0x2D
    byte2 = (sequencia >> 16) & comparador; // byte2 = 0x4A
    byte3 = (sequencia >> 24) & comparador; // byte3 = 0x03
    
    // byte0
    unsigned long int resp0 = byte0;
    unsigned long int resp1 = byte1;
    unsigned long int resp2 = byte2;
    unsigned long int resp3 = byte3;

    // Laço para comparar os bits com xor
    for (int i = 1; i < 8; i++)
    {
        resp0 = resp0 ^ (byte0 << i);
        resp1 = resp1 ^ (byte1 << i);
        resp2 = resp2 ^ (byte2 << i);
        resp3 = resp3 ^ (byte3 << i);
    }

    // O bit que mostra a paridade está na posição do algarismo mais significativo
    // Então demos bit shift right 7x para posicionar na posição do menos significativo
    // E damos and com o valor para garantir que não aja nenhum erro e o valor seja apenas o que quremos
    resp0 = resp0 >> 7;
    resp0 = resp0 & 1;

    resp1 = resp1 >> 7;
    resp1 = resp1 & 1;

    resp2 = resp2 >> 7;
    resp2 = resp2 & 1;

    resp3 = resp3 >> 7;
    resp3 = resp3 & 1;   

    printf("0x%02X", byte0);
    printf(" - paridade par = %d\n", resp0);

    printf("0x%02X", byte1);
    printf(" - paridade par = %d\n", resp1);   

    printf("0x%02X", byte2);
    printf(" - paridade par = %d\n", resp2);

    printf("0x%02X", byte3);
    printf(" - paridade par = %d\n", resp3);     

    return 0;
}