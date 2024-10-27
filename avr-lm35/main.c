/*
 * LM35
 * Date: 24.10.2024
 * Author: Prakash Kushwaha
 */

// Include drivers
#define F_CPU 16000000UL

#include "LIB/STD_TYPES.h"
#include "HAL/LCD_Interface.h"
#include "MCAL/DIO_Config.h"
#include "MCAL/DIO_Interface.h"
#include "MCAL/ADC_Interface.h"
#include <util/delay.h>

// Main function
int main(void)
{
	// Initialize the LCD
	LCD_VoidInit();

	// Initialize ADC
	ADC_VoidInit();

	// Initialize ADC reading value
	u16 digital_value, analog_value;

	// Program loop
	while(1)
	{
		// Get ADC reading
		digital_value = ADC_u16ReadValue(0);

		// Get analog value
		analog_value = ADC_u16ToTemp(digital_value);

		// Clear LCD screen
		LCD_VoidWriteCommand(0);
		_delay_ms(2);

		// Reposition LCD writing position
		LCD_VoidSetPosition(0,0);

		// Display value in LCD then delay for 500ms
		LCD_VoidWriteNumber(analog_value);
		_delay_ms(500);
	}
}
