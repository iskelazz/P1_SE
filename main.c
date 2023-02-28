#include "MKL46Z4.h"

// LED (RG)
// LED_GREEN = PTD5
// LED_RED = PTE29

void delay(void)
{
  volatile int i;

  for (i = 0; i < 1000000; i++);
}

// LED_GREEN = PTD5
void led_green_init()
{
  SIM->COPC = 0; // Deshabilitar WatchDog
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK; 
  // SIM->SCGC5 |= (1 << 12) //Esto coloca el pin12 a 1. Ademas no sobreescribe el registro por eso se hace de esta manera
  //Es equivalente a la puerta logica OR que en C es "|=" a nivel de bit (bitwise). La instruccion realiza un OR bit a bit con un 1 en la pos 12
  //AND seria SIM->SCG5 &~ (1 << 12)
  PORTD->PCR[5] = PORT_PCR_MUX(1);//Funcionamiento como multiplexor. O rexistro pcr de cada un dos portos DPORT->PCR[numPIN]
  GPIOD->PDDR |= (1 << 5);
  GPIOD->PSOR = (1 << 5);
}

void led_red_init()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
  PORTE->PCR[29] = PORT_PCR_MUX(1);
  GPIOE->PDDR |= (1 << 29);//Rexistro comun para todo o porto
  GPIOE->PSOR = (1 << 29);
}

void config_sw1()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
  PORTC->PCR[3] = PORT_PCR_MUX(1);
  GPIOC->PDDR |= (1 << 3);//Rexistro comun para todo o porto
  GPIOC->PSOR = (1 << 3);
}

void config_sw2()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
  PORTC->PCR[12] = PORT_PCR_MUX(1);
  GPIOC->PDDR |= (1 << 12);//Rexistro comun para todo o porto
  GPIOC->PSOR = (1 << 12);
}


int sw1_check(){
  return (GPIOC->PDIR & ~(1 << 3));
}

int sw2_check(){
  return (GPIOC->PDIR & ~(1 << 12));
}

// LED_RED = PTE29
void led_red_toggle(void)
{
  GPIOE->PTOR = (1 << 29);
}

void led_green_toggle(void)
{
  GPIOE->PTOR = (1 << 5);
}

int main(void)
{
  int sw1_count = 0;
  int sw2_count = 0;
  // Configure input switches
  config_sw1();
  config_sw2();
    
  led_green_init();
  led_red_init();
  

  while (1) {
    if (sw1_check()){
      sw1_count = (sw1_count + 1) % 2;
    }
    if (sw2_check()){
      sw2_count = (sw2_count + 1) % 2;
    }
    if (sw1_count || sw2_count){
      led_red_toggle();
    } else {
      led_green_toggle();
    }
    delay();
  }

  return 0;
}
