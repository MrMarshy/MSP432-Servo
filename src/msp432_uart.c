#include <uart.h>
#include "msp432_uart.h"

static volatile uint8_t UART_rXData = 0;

/* For tinyprintf */
void _putchar(char c){

#if 1
	// While Trasmit Flag is not set (wait until ready)
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));   
    EUSCI_A0->TXBUF = c;
#endif 

#if 0
    while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A2->TXBUF = c;
#endif
}

void UART0_init(){

    /* Set P1.2 and P1.3 as UCA0RXD and UCA0TXD */
    P1->SEL0 |= BIT2 | BIT3;   

    /* Set P3.2 and P3.3 as UART for non usb debugging. */
    P3->SEL0 |= BIT2 | BIT3;

/** UART Configuration for 115200 with Clk @ SMCLK of 3MHz or 48MHz.
 *   Value generated using the tool provided on the following page:
 * 
 *  http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 * 
 */ 

    if(SystemCoreClock == 3000000U){
        /* 8-bit, 1 stop bit, no parity P1.2 and P1.3 */
        EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;         // Put in reset mode while configuring 
        EUSCI_A0->MCTLW = 0xA0 | EUSCI_A_MCTLW_OS16;    // First Modulation is 10, Oversampling enabled
        EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;   // SMCLK
        EUSCI_A0->BRW = 0x01;                           // Clock prescalar 1

        /* 8-bit, 1 stop bit, no partiy P3.2 and P3.3 */
        EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;
        EUSCI_A2->MCTLW = 0xA0 | EUSCI_A_MCTLW_OS16;    // First Modulation is 10, Oversampling enabled
        EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;   // SMCLK
        EUSCI_A2->BRW = 0x01;                           // Clock prescalar 1
    }
    else if(SystemCoreClock == 48000000U){
        /* 8-bit, 1 stop bit, no parity P1.2 and P1.3*/
        EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;         // Put in reset mode while configuring 
        EUSCI_A0->MCTLW = 0x09 | EUSCI_A_MCTLW_OS16;    // First Modulation is 9, Oversampling enabled
        EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;   // SMCLK
        EUSCI_A0->BRW = 0x1A;                           // Clock prescalar 26

        /* 8-bit, 1 stop bit, no parity P3.2 and P3.3*/
        EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;         // Put in reset mode while configuring 
        EUSCI_A2->MCTLW = 0x09 | EUSCI_A_MCTLW_OS16;    // First Modulation is 9, Oversampling enabled
        EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;   // SMCLK
        EUSCI_A2->BRW = 0x1A;                           // Clock prescalar 26
    }
    /* Clear SWRST to resume */
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;

    /* Enable Rx interrupts */
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE; 
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;

    NVIC_SetPriority(EUSCIA0_IRQn, 4); /* set priority to 4 in NVIC */
    NVIC_SetPriority(EUSCIA2_IRQn, 4);
    NVIC_EnableIRQ(EUSCIA0_IRQn);      /* enable interrupt in NVIC */
    NVIC_EnableIRQ(EUSCIA2_IRQn);

}

void UART_clearScreen(void){
    printf("%s", CLEAR_SCREEN_HOME);
    
}


void EUSCIA0_IRQHandler(void){

    if(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG){

         // Get Received UART data , rx interrupt flag is cleared by read
        (void)EUSCI_A0->RXBUF;              
    }
}

void EUSCIA2_IRQHandler(void){

    if(EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG){

         // Get Received UART data , rx interrupt flag is cleared by read
        (void)EUSCI_A2->RXBUF;              
    }
}