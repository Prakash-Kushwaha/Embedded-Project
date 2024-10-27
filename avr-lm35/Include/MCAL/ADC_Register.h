/*
*    DIO_Register.h
*    Version: 1.0
*    Date: 24-10-2024
*    Desc: Header file contains registers addresses for ADC driver 
*/

#ifndef ADC_REGISTER_H
#define ADC_REGISTER_H

// Macros to define registers addreses
#define ADMUX     *((volatile u8 *) 0x27)
#define ADCSRA    *((volatile u8 *) 0x26)
#define ADCH      *((volatile u8 *) 0x25)
#define ADCL      *((volatile u1 *) 0x24)
#define SFIOR     *((volatile u8 *) 0x50)
#define ADC_D     *((volatile u16 *) 0x24)    // 16 to read ADCL and ADCH

#endif
