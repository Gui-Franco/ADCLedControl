    #include "MKL25Z4.h"

    void init_LEDs(void) {
        SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

        PORTB->PCR[19] = PORT_PCR_MUX(1); // LED Verde
        PORTD->PCR[1]  = PORT_PCR_MUX(1); // LED Azul

        GPIOB->PDDR |= (1 << 19);
        GPIOD->PDDR |= (1 << 1);

        GPIOB->PSOR = (1 << 19);
        GPIOD->PSOR = (1 << 1);
    }
    void init_ADC(void) {
        SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
        SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

        PORTB->PCR[0] &= ~PORT_PCR_MUX_MASK; // PTB0 = analógico (MUX=0)

        ADC0->CFG1 = ADC_CFG1_MODE(1); // 12 bits
    }

    uint16_t read_ADC(uint8_t channel) {
        ADC0->SC1[0] = channel;
        while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
        return ADC0->R[0];
    }
    int main(void) {
        init_LEDs();
        init_ADC();

        for (;;) {
            uint16_t v = read_ADC(8); // Canal 8 = PTB0 (ADC0_SE8)

            if (v > 3800) {            // ~3.3V → LED Azul
                GPIOD->PCOR = (1 << 1);
                GPIOB->PSOR = (1 << 19);
            } else if (v < 300) {      // ~0V   → LED Verde
                GPIOB->PCOR = (1 << 19);
                GPIOD->PSOR = (1 << 1);
            } else {                   // Intermediário → ambos apagados
                GPIOB->PSOR = (1 << 19);
                GPIOD->PSOR = (1 << 1);
            }
        }
    }
        