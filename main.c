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


void sw1_check(int count)
{
  if ((GPIOC->PDIR & (1 << 3)) == 0){
    delay();
    if ((GPIOC->PDIR & (1<<3))==0){
      count++;
      while ((GPIOC->PDIR & (1 << 3)) == 0); 
    }
  }
  return count;
}

void sw2_check(int count)
{
  if ((GPIOC->PDIR & (1 << 12)) == 0){
    delay();
    if ((GPIOC->PDIR & (1<<12))==0){
      count++;
      while ((GPIOC->PDIR & (1 << 12)) == 0); 
    }
  }
  return count;
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

  // Configure input switches
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  PORTC->PCR[3] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // Enable pull-up resistor
  PORTC->PCR[12] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // Enable pull-up resistor
  
  led_green_init();
  led_red_init();
  

  while (1) {
    sw1_count = sw1_check(sw1_count)
    if ((sw1_count % 2) == 0){
      GPIOE->PSOR = (1 << 29);
    }
    if ((sw1_count % 2) == 1){
      GPIOE->PTOR = (1 << 29)
    }
    delay();
  }

  return 0;
}

