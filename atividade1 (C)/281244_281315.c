#include <stdio.h>

// Trecho do discurso do presidente da assembléia constituinte, Ulysses Guimarães, proferido por ocasião da promulgação da Constituição de 1988.
char texto[] = "A Constituição certamente não é perfeita. Ela própria o confessa ao admitir a reforma. Quanto a ela, discordar, sim. Divergir, sim. Descumprir, jamais. Afrontá-la, nunca.\nTraidor da Constituição é traidor da Pátria. Conhecemos o caminho maldito. Rasgar a Constituição, trancar as portas do Parlamento, garrotear a liberdade, mandar os patriotas para a cadeia, o exílio e o cemitério.\nQuando após tantos anos de lutas e sacrifícios promulgamos o Estatuto do Homem da Liberdade e da Democracia bradamos por imposição de sua honra.\nTemos ódio à ditadura. Ódio e nojo.\nAmaldiçoamos a tirania aonde quer que ela desgrace homens e nações. Principalmente na América Latina.\n\nUlysses Guimarães, Presidente da Assembleia Nacional Constituinte.\n";

// A função conta_palavras recebe a string cadeia_de_caracteres como argumento e deve retornar o número de palavras presentes nessa string.
int conta_palavras(char * cadeia_de_caracteres)
{
    int numero_de_palavras = 0;
    int palavra = 0;
    char *p;
    p = cadeia_de_caracteres;
    // Insira aqui o programa que implementa a função
    // Uma string simboliza em C que termina com o caracter '\0'
    

    while (*p != '\0')          
    {
        
        if (*p == '\n' && *p == ' ' && *p == '.' && *p == ',') // Sai de uma palavra quando encontra um desses casos
        {
            palavra = 0; // saiu de uma palavra
        }
        else if (palavra == 0) {
            palavra = 1; // está em uma palavra
            numero_de_palavras++;
        }
        p++;
    }


		
    return numero_de_palavras;
}

// A função imprime_palavras_c deve imprimir todas as palavras presentes em cadeia_de_caracteres na sequência em que aparecem no texto, uma por linha.
void imprime_palavras_c(char * cadeia_de_caracteres)
{
	// Insira aqui o programa que implementa a função
	
}

int main() {
	printf("%s\n",texto);
	printf("O número de palavras no texto é: %d\n", conta_palavras(texto));
	printf("As palavra que começam com a letra c no texto são:\n");
	imprime_palavras_c(texto);
	return 0;
}
