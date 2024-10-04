#include "MKL25Z4.h"


int leds[7]={8,9,10,11,0,1,2};
int velocidadInicial=3000000;
int velocidadAumentada=3000000;

int main(){
    SIM->SCGC5|=SIM_SCGC5_PORTB_MASK|SIM_SCGC5_PORTA_MASK;

    //Pines como GPIO

    PORTB->PCR [8]|=(1<<8); 
    PORTB->PCR [9]|=(1<<8);
    PORTB->PCR [10]|=(1<<8);
    PORTB->PCR [11]|=(1<<8); //LED rojo
    PORTB->PCR [0]|=(1<<8);
    PORTB->PCR [1]|=(1<<8);
    PORTB->PCR [2]|=(1<<8);

    PORTA->PCR [3]|=(1<<8)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;

    //Pines como OUTPUT
    
    PTB->PDDR|=(1<<8);
    PTB->PDDR|=(1<<9);
    PTB->PDDR|=(1<<10);
    PTB->PDDR|=(1<<11); 
    PTB->PDDR|=(1<<0);
    PTB->PDDR|=(1<<1);
    PTB->PDDR|=(1<<2);

    PTA->PDDR&=~(1<<3); //Pulsador como INPUT


    //Funcionamiento

    NVIC_EnableIRQ(PORTA_IRQn); //Habilita interrupciones en el puerto A
    NVIC_SetPriority(PORTA_IRQn, 1);
    for(int i=0;i<7;i++){
        PTA -> PSOR|=(1<<leds[i]);
        PTA -> PSOR&=~(1<<leds[i-1]);
        for(velocidadAumentada;velocidadAumentada<0;velocidadAumentada--);
        if(i>7){
            i=0;
        }
    }
    PORTA -> PCR [3] = PORT_PCR_MUX(1) | PORT_PCR_IRQC( 0b1101 ); //Interrupcion por flanco de subida

    return 0;
    }

void PORTA_IRQHandler(){
        if(PORTA -> ISFR & (1<<3)){
            PORTA -> ISFR |= (1<<3);
            if(((PTB -> PDIR&(1u<<11)) && ((PTA -> PDIR&(1u<<3)))) == 1){
                velocidadAumentada = velocidadInicial * 1.5;
            }else{
                velocidadAumentada = velocidadInicial;
            }
        }
}