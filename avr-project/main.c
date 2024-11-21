/*
 * lm35-final.c
 *
 * Created: 21-11-2024 10:07:18
 * Author : Prakash Kushwaha
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>

#define LCD_DPRT PORTD    // LCD DATA PORT
#define LCD_DDDR DDRD     // LCD DATA DDR
#define LCD_DPIN PIND     // LCD DATA PIN
#define LCD_CPRT PORTB    // LCD COMMANDS PORT
#define LCD_CDDR DDRB     // LCD COMMANDS DDR
#define LCD_CPIN PINB     // LCD COMMANDS PIN
#define LCD_RS 0          // LCD RS
#define LCD_RW 1          // LCD RW
#define LCD_EN 2          // LCD EN

void lcdCommand(unsigned char cmnd)
{
    LCD_DPRT = cmnd;    // Send command to data port
    LCD_CPRT &= ~(1 << LCD_RS);    // RS = 0 for command
    LCD_CPRT &= ~(1 << LCD_RW);    // RW = 0 for write
    LCD_CPRT |= (1 << LCD_EN);    // EN = 1 for H-to-L pulse
    _delay_us(1);
    LCD_CPRT &= ~ (1 << LCD_EN);    // EN = 0 for H-to-L pulse
    _delay_us(100);
}

void lcdData(unsigned char data)
{
    LCD_DPRT = data;    // Send data to port
    LCD_CPRT |= (1 << LCD_RS);    // RS = 1 for data
    LCD_CPRT &= ~(1 << LCD_RW);    // RW = for write
    LCD_CPRT |= (1 << LCD_EN);    // EN = 1 for H-to-L pulse
    _delay_us(1);
    LCD_CPRT &= ~ (1 << LCD_EN);    // EN = 0 for H-to-L pulse
    _delay_us(100);
}

void lcd_init()
{
    LCD_DDDR = 0xFF;
    LCD_CDDR = 0xFF;
    
    LCD_CPRT &= ~(1 << LCD_EN);    // LCD_EN = 0
    _delay_us(2000);
    
    lcdCommand(0x38);    // initialize the LCD
    // lcdCommand(0x0E);    // display on, cursor on
	lcdCommand(0x0C);	// display on, cursor off
    lcdCommand(0x01);    // clear LCD
    _delay_us(2000);    // wait
    lcdCommand(0x06);    // shift cursor right
}

void lcd_gotoxy(unsigned char x, unsigned char y)
{
    unsigned char firstCharAdr[] = {0x80, 0xC0, 0x94, 0xD4};
    lcdCommand(firstCharAdr[y-1] + x-1);
    _delay_us(100);
}

void lcd_print(char *str)
{
    unsigned char i = 0;
    while (str[i] != 0)
    {
        lcdData(str[i]);
        i++;
    }
}

void adc_init()
{
	// Set ADC to use AVCC as reference voltage, and select ADC0
	ADMUX = (0 << REFS1) | (1 << REFS0); // AVCC with external capacitor at AREF pin
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC and set prescaler to 64
}


uint16_t adc_read(uint8_t channel)
{
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Select ADC channel
    ADCSRA |= (1 << ADSC);  // Start conversion
    while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
    return ADC;
}

int main(void)
{
	char buffer[16];
	uint16_t adc_value;
	uint16_t temperature; // Store temperature
	
	lcd_init();
	adc_init();
	
	lcd_gotoxy(1, 1);
	lcd_print("Temperature: ");

	while (1)
	{
		adc_value = adc_read(0); // Read ADC value from ADC0
		temperature = (adc_value * 5000.0 / 1024.0); // Convert to temperature in Celsius
		
		// Format temperature as string (e.g., "25.2 °C")
		snprintf(buffer, sizeof(buffer), "%u.%u %cC", temperature / 10, temperature % 10, 0xDF); 
		lcd_gotoxy(1, 2);
		lcd_print(buffer); // Display temperature on LCD
		
		_delay_ms(1000); // Update every second
	}
	
	return 0;
}
