#include "stm32f4xx.h"

#define RS 0x20     /* PA5 mask for reg select */
#define RW 0x40     /* PA6 mask for read/write */
#define EN 0x80     /* PA7 mask for enable */

void delayMs(int n);
void LCD_command(unsigned char command);
void LCD_data(char data);
void LCD_init(void);
void PORTS_init(void);
void pedestrianCrossing(void);
void display(char);
char toArray(int);
void LCD_displayOut(char[], char[]);

int main(void) {
		LCD_init();											/* note: this enables A, B, and C clocks */
	
		int result;
	
    /* set up pin PA1 for analog input */
    GPIOA->MODER |=  0xC;           /* PA1 analog */

    /* setup ADC1 */
    RCC->APB2ENR |= 0x00000100;     /* enable ADC1 clock */
    ADC1->CR2 = 0;                  /* SW trigger */
    ADC1->SQR3 = 1;                 /* conversion sequence starts at ch 1 */
    ADC1->SQR1 = 0;                 /* conversion sequence length 1 */
    ADC1->CR2 |= 1;                 /* enable ADC1 */
		
    while (1) {
        ADC1->CR2 |= 0x40000000;        /* start a conversion */
        while(!(ADC1->SR & 2)) {}       /* wait for conv complete */
        result = ADC1->DR;              /* read conversion result */
				
				char degree[20];
				sprintf(degree, "Angle: %f ",result*240.0/4095);
				char voltage[20];
				sprintf(voltage, "Volts: %f ",result*3.3/4095);
					
				LCD_displayOut(degree, voltage);
				delayMs(500);
    }
}

void LCD_displayOut(char degree[], char voltage[]) {
		LCD_command(0x01);
		for (int i = 0; i < 20; i++) {
			LCD_data(degree[i]);
		}
		LCD_command(0xC0);
		for (int i = 0; i < 20; i++) {
			LCD_data(voltage[i]);
		}
}

/* initialize port pins then initialize LCD controller */
void LCD_init(void) {
    PORTS_init();

    delayMs(30);            /* initialization sequence */
    LCD_command(0x30);
    delayMs(10);
    LCD_command(0x30);
    delayMs(1);
    LCD_command(0x30);

    LCD_command(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

void PORTS_init(void) {
    RCC->AHB1ENR |=  0x07;          /* enable GPIOB/C clock */

    /* PB5 for LCD R/S */
    /* PB6 for LCD R/W */
    /* PB7 for LCD EN */
    GPIOB->MODER &= ~0x0000FC00;    /* clear pin mode */
    GPIOB->MODER |=  0x00005400;    /* set pin output mode */
    GPIOB->BSRR = 0x00C00000;       /* turn off EN and R/W */

    /* PC0-PC7 for LCD D0-D7, respectively. */
    GPIOC->MODER &= ~0x0000FFFF;    /* clear pin mode */
    GPIOC->MODER |=  0x00005555;    /* set pin output mode */
}

void LCD_command(unsigned char command) {
    GPIOB->BSRR = (RS | RW) << 16;  /* RS = 0, R/W = 0 */
    GPIOC->ODR = command;           /* put command on data bus */
    GPIOB->BSRR = EN;               /* pulse E high */
    delayMs(0);
    GPIOB->BSRR = EN << 16;         /* clear E */

    if (command < 4)
        delayMs(2);         /* command 1 and 2 needs up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(char data) {
    GPIOB->BSRR = RS;               /* RS = 1 */
    GPIOB->BSRR = RW << 16;         /* R/W = 0 */
    GPIOC->ODR = data;              /* put data on data bus */
    GPIOB->BSRR = EN;               /* pulse E high */
    delayMs(0);
    GPIOB->BSRR = EN << 16;         /* clear E */

    delayMs(1);
}

/* 16 MHz SYSCLK */
void delayMs(int n) {
    int i;

    /* Configure SysTick */
    SysTick->LOAD = 16000;  /* reload with number of clocks per millisecond */
    SysTick->VAL = 0;       /* clear current value register */
    SysTick->CTRL = 0x5;    /* Enable the timer */

    for(i = 0; i < n; i++) {
        while((SysTick->CTRL & 0x10000) == 0) /* wait until the COUNTFLAG is set */
            { }
    }
    SysTick->CTRL = 0;      /* Stop the timer (Enable = 0) */
}