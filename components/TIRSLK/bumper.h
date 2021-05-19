#ifndef BUMPER_H
#define BUMPER_H

/**
 * Bump1 | Bump2 | Bump3 | Bump4 | Bump5 | Bump6 |
 * -----------------------------------------------
 * P4.0  | P4.2  | P4.3  | P4.5  | P4.6  | P4.7  |
 * -----------------------------------------------
 */
#include "FreeRTOS.h"
#include "queue.h"

struct BumperQueueData {
    int bumper1;
    int bumper2;
    int bumper3;
    int bumper4;
};

constexpr int bumperQueueLen = sizeof(BumperQueueData);
QueueHandle_t bumperQueue = xQueueCreate(1, bumperQueueLen);

static struct BumperQueueData queueData = {0};

class Bumper{
public:

    Bumper(){
        /* All Bumper switches to inputs */
        P4->SEL1 &= ~0xED;
        P4->SEL0 &= ~0xED;
        P4->DIR &= ~0xED;
        P4->REN |= 0xED;
        P4->OUT |= 0xED;

        /* Enable Interrupts on All Switches */
        P4->IES |= 0xED; 
        P4->IFG = 0; 
        P4->IE |= 0xED; 
        NVIC_SetPriority(PORT4_IRQn, 5);
        NVIC_EnableIRQ(PORT4_IRQn);
    }

    ~Bumper() = default;

private:
};

#endif 
extern "C" {

    void PORT4_IRQHandler(void){

        if(P4->IFG & BIT0){ // Bumper 1
            
            P4->IFG &= ~BIT0; // Clear interrupt

            BaseType_t higherTaskWoken = pdFALSE;

            if((P4->IES & BIT0)){ // falling edge
                
                queueData.bumper1 = 1;
            }
            else{
                queueData.bumper1 = 0;
            }
            
            xQueueSendFromISR(bumperQueue, &queueData, &higherTaskWoken);
        
            portYIELD_FROM_ISR(higherTaskWoken);
        }
        else if(P4->IFG & BIT2){ // Bumper 2
            
            P4->IFG &= ~BIT2; // Clear interrupt

            BaseType_t higherTaskWoken = pdFALSE;

            if((P4->IES & BIT2)){ // falling edge
                
                queueData.bumper2 = 1;
            }
            else{
                queueData.bumper2 = 0;
            }
            
            xQueueSendFromISR(bumperQueue, &queueData, &higherTaskWoken);
        
            portYIELD_FROM_ISR(higherTaskWoken);
        }

        P4->IFG = 0;

    }

}