/*
Iago Lucini da Silva 281244   
Maria Clara Martinez 281315
*/


unsigned char *p_ddrb = (unsigned char *) 0x24;
unsigned char *p_portb = (unsigned char *) 0x25;

unsigned char *p_ddrd = (unsigned char *) 0x2A;
unsigned char *p_pind = (unsigned char *) 0x29;

int estado = 0;

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
    switch (estado)
    {
        case (0):

            // Detecta borda de descida
            if (filtrado == 0)
            {
                estado = 1;
                inverte_led();
            }
            else if (filtrado == 1)
            {
                estado = 0;
            }
            break;
        
        case (1):
            if (filtrado == 0)
            {
                estado = 1;
            }
            else if (filtrado == 1)
            {
                estado = 2;
                inverte_led();
            }
            break;
        
        case (2):
            if (filtrado == 0)
            {
                estado = 3;
                inverte_led();
            }
            else if (filtrado == 1)
            {
                estado = 2;
            }
        case (3):
            if (filtrado == 0)
            {
                estado = 3;
            }
            else if (filtrado == 1)
            {
                estado = 0;
                inverte_led();
            }
    }
}

void inicializar()
{
    *p_ddrb |= 0x20;
    *p_ddrd &= (~0x4);
    *p_portb &= ~(0x20);
}


int main () {
    inicializar();

    while (1) {
        int filtrado = *p_pind & 0x04;

        if (filtrado == 0x04) {
            filtrado = (filtrado >> 2);
        }

        deteccao_borda(filtrado);
    };
}