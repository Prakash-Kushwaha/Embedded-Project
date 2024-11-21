# **Readme.md**

## **Temperature Measurement using LM35 and ATmega32**

This project demonstrates how to measure temperature using the **LM35 temperature sensor** with an **ATmega32** microcontroller and display the measured temperature on an **LCD**. The LM35 sensor outputs an analog voltage which is directly proportional to the temperature, and the ADC (Analog-to-Digital Converter) of the ATmega32 is used to convert this analog voltage into a digital value. The result is then displayed on an LCD screen.

### **Features:**
- Temperature reading in Celsius from LM35
- LCD display of the temperature
- Conversion of ADC value to actual temperature in Celsius
- Code running on ATmega32 microcontroller
- Simple LCD interfacing using control and data pins

---

### **Table of Contents**
- [Project Overview](#project-overview)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Code Explanation](#code-explanation)
  - [LCD Functions](#lcd-functions)
  - [ADC Functions](#adc-functions)
  - [Main Program](#main-program)
- [Conclusion](#conclusion)

---

### **Project Overview**
This project uses the **LM35 temperature sensor** to measure the surrounding temperature. The LM35 outputs a voltage proportional to the temperature in **Celsius** (10mV per degree Celsius). The voltage is read through the **ADC0** pin of the **ATmega32 microcontroller**, which converts the analog voltage into a digital value. This digital value is then converted into the corresponding temperature and displayed on a **16x2 LCD**.

---

### **Hardware Requirements**
- **ATmega32 Microcontroller**
- **LM35 Temperature Sensor**
- **16x2 LCD Display**
- **Resistors** (for pull-up/pull-down as needed)
- **Connecting wires**
- **Breadboard** (for prototyping)
- **Power Supply** (5V)
- **Capacitor** (for stabilization on the AREF pin if necessary)

### **Pin Connections:**

| **Component**        | **ATmega32 Pin** | **Description**                  |
|----------------------|------------------|----------------------------------|
| LM35 Output          | ADC0 (PA0)       | LM35 analog voltage output      |
| LCD Data (D0-D7)     | PD0-PD7          | Data pins for LCD               |
| LCD Control (RS, RW, EN) | PB0-PB2        | Control pins for LCD            |
| LCD Power (VCC, GND) | VCC, GND         | Power supply to LCD             |

### **Software Requirements**
- **AVR GCC Compiler** for compiling C code.
- **AVRDUDE** for flashing the program to the ATmega32.
- **LCD Library** (if you're using a library, but this project directly handles the LCD control).

---

### **Code Explanation**

#### **1. LCD Functions**

These functions handle the interaction between the ATmega32 and the 16x2 LCD.

##### `lcdCommand(unsigned char cmnd)`
This function sends a command to the LCD:
- It sends a byte to the **LCD data port** (`PORTD`).
- Sets **RS** to 0 (command mode), **RW** to 0 (write mode), and pulses **EN** (enable pin) to latch the command.

##### `lcdData(unsigned char data)`
This function sends data to the LCD:
- It sends a byte to the **LCD data port**.
- Sets **RS** to 1 (data mode), **RW** to 0 (write mode), and pulses **EN** (enable pin) to latch the data.

##### `lcd_init()`
This function initializes the LCD in 8-bit mode. It performs the following tasks:
- Configures **LCD data port** (`PORTD`) and **LCD control port** (`PORTB`) as output.
- Sends initialization commands to the LCD: sets the display mode, clears the screen, and sets the cursor to shift to the right.

##### `lcd_gotoxy(unsigned char x, unsigned char y)`
This function moves the cursor to the specified position on the LCD:
- It uses the **firstCharAdr[]** array to find the address of the first character in the row (x, y).
- It sends the corresponding command to the LCD to move the cursor.

##### `lcd_print(char *str)`
This function prints a string of characters to the LCD:
- It loops through each character in the string and sends it to the LCD using `lcdData()`.

---

#### **2. ADC Functions**

These functions manage the conversion of the analog voltage from the LM35 to a digital value.

##### `adc_init()`
This function initializes the ADC on the ATmega32:
- It sets the **reference voltage** to **AVCC** with an external capacitor at the **AREF** pin.
- It enables the ADC and sets the **prescaler** to **64** to allow for proper ADC conversion timing (this is based on the clock speed).

##### `adc_read(uint8_t channel)`
This function reads the value from a specified ADC channel (in this case, **ADC0**):
- It configures the **ADMUX** register to select the ADC channel.
- It starts the ADC conversion by setting **ADSC** in the **ADCSRA** register.
- It waits for the conversion to complete by polling **ADSC** until it becomes 0.
- Finally, it returns the ADC result (10-bit value).

---

#### **3. Main Program**

The `main()` function runs the program that reads the temperature and displays it on the LCD.

```c
int main(void)
{
    char buffer[16];
    uint16_t adc_value;
    uint16_t temperature; // Store temperature

    lcd_init();  // Initialize LCD
    adc_init();  // Initialize ADC
    
    lcd_gotoxy(1, 1);  // Move cursor to the first row
    lcd_print("Temperature: ");  // Display label

    while (1)
    {
        adc_value = adc_read(0); // Read ADC value from ADC0 (LM35)
        temperature = (adc_value * 5000.0 / 1024.0); // Convert to temperature in Celsius

        // Format the temperature as a string (e.g., "25.2 °C")
        snprintf(buffer, sizeof(buffer), "%u.%u %cC", temperature / 10, temperature % 10, 0xDF); 
        lcd_gotoxy(1, 2);  // Move cursor to the second row
        lcd_print(buffer);  // Display the temperature

        _delay_ms(1000);  // Update every second
    }

    return 0;
}
```

##### Breakdown of the `main()` function:
1. **Initialization:**
   - The LCD and ADC are initialized using `lcd_init()` and `adc_init()` functions.
   - The first line of the LCD displays the label "Temperature:".

2. **Temperature Measurement:**
   - The ADC value is read from **ADC0** using `adc_read(0)`.
   - The ADC value is converted to a voltage (in millivolts) and then to temperature in **Celsius** by dividing by 10mV per degree Celsius.

3. **Displaying on LCD:**
   - The temperature is formatted as a string (e.g., "25.2 °C") and displayed on the second line of the LCD using `lcd_print()`.

4. **Delay:**
   - The program waits for 1 second before updating the temperature on the LCD.

---

### **Conclusion**

This project successfully reads the temperature from the **LM35** sensor using the **ATmega32** microcontroller's ADC and displays the result on a **16x2 LCD**. It demonstrates how to interface analog sensors with microcontrollers, use ADC for voltage-to-digital conversion, and display the data on an LCD.

---

### **License**

This project is open source and can be modified and distributed under the terms of the GPL v3 [LICENSE](LICENSE).