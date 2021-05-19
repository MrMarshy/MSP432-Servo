#ifndef MSP432_SERVO_H
#define MSP432_SERVO_H

#include <algorithm>
#include <driverlib.h>

class Servo{

public:
    
    explicit Servo(){

        PwmInit();
    }

    /**
     * @brief Set duty cycle of pwm as a percentage
     * @param duty The percentage to set the duty cycle to (0 to 100% i.e. 0.0 to 1.0)
     */
    constexpr void setDutyCycle(const float& duty) const {
        float new_duty = 0.0f;
        
        new_duty = std::min(duty, 1.0f);
        new_duty = std::max(new_duty, 0.0f);
        uint16_t iduty = static_cast<uint16_t>(static_cast<float>(_period) * new_duty);
        TIMER_A0->CCR[1] = iduty; // CCR1 PWM duty cycle
    }

    constexpr void goToPositive90Deg() const {
        TIMER_A0->CCR[1] = pos90;
    }

    constexpr void goToNegative90Deg() const {
        TIMER_A0->CCR[1] = neg90;
    }

    constexpr void goToZeroDeg() const {
        TIMER_A0->CCR[1] = zero0;
    }

    ~Servo() = default;

protected:

private:

    void PwmInit() const {
        
        P2->SEL1 &= ~BIT4;
        P2->SEL0 |= BIT4; // Set P2.4 as PWM Output
        P2->DIR |= BIT4;

        TIMER_A0->CCR[0] = _period; // PWM Period

        TIMER_A0->CCR[1] = zero0; // CCR1 PWM duty cycle

        TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7; // CCR1 reset/set on P2.4

        // ACLK (32768kHz), Up Mode, Clear TAR
        TIMER_A0->CTL = TIMER_A_CTL_TASSEL_1 | TIMER_A_CTL_MC_1 | TIMER_A_CTL_CLR;
    }

    static constexpr uint16_t _period = UINT16_C(655 - 1); // 32768 / 655 = 50Hz
    static constexpr float conv_factor = 20.0f/655.0f;
    static constexpr uint16_t neg90 = static_cast<uint16_t>(0.6f / conv_factor);
    static constexpr uint16_t pos90 = static_cast<uint16_t>(2.4f / conv_factor);
    static constexpr uint16_t zero0 = static_cast<uint16_t>(1.5f / conv_factor);
};

#endif // MSP432_SERVO_H