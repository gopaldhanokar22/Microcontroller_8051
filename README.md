# Microcontroller_8051
1. Title: Traffic Light System Using MCU

Objective: Design a traffic light system using Red, Yellow, and Green LEDs that mimic real-world traffic signals.

Functional Requirements:

The Red LED should turn ON for 5 seconds (Stop Signal).
After that, the Yellow LED should turn ON for 2 seconds (Get Ready Signal).
The Green LED should turn ON for 5 seconds (Go Signal).
The cycle should repeat continuously.

Hardware Components Required:
Microcontroller: AT89C52 (8051 family)
LEDs: Red, Yellow, Green
Resistors: 330Ω (for current limiting)
Crystal Oscillator: 11.0592 MHz
Capacitors: 22pF (for stabilizing the oscillator)
Power Supply: 5V DC
Connecting Wires
Breadboard/PCB

Hardware Connection:
Red LED → Connected to P1.0
Yellow LED → Connected to P1.1
Green LED → Connected to P1.2
Common Ground (GND) for all LEDs
Microcontroller powered by 5V DC supply

Software Used:
Keil uVision – For writing and compiling the C program
Proteus – For simulating the circuit
Flash Magic – For burning the code into the microcontroller

Working:
The Red LED turns ON for 5 seconds, signaling vehicles to STOP.
The Yellow LED turns ON for 2 seconds, indicating vehicles to GET READY.
The Green LED turns ON for 5 seconds, allowing vehicles to GO.
The cycle repeats continuously to simulate real-world traffic signals.

#include <reg52.h>  // Header file for 89C52

// Define LED connections
sbit RedLED = P1^0;
sbit YellowLED = P1^1;
sbit GreenLED = P1^2;

// Function to create delay in seconds
void delay_seconds(unsigned int sec) {
    unsigned int i, j;
    for(i = 0; i < sec; i++) {
        for(j = 0; j < 1000; j++) {
            // Approximate 1ms delay (adjust based on your crystal frequency)
            TMOD = 0x01;        // Timer0 mode 1
            TH0 = 0xFC;         // Initial values for 1ms delay with 11.0592MHz
            TL0 = 0x66;
            TR0 = 1;            // Start timer
            while(TF0 == 0);    // Wait for overflow
            TR0 = 0;            // Stop timer
            TF0 = 0;            // Clear overflow flag
        }
    }
}

void main() {
    while(1) {  // Infinite loop
        // Red LED ON for 5 seconds
        RedLED = 1;
        YellowLED = 0;
        GreenLED = 0;
        delay_seconds(5);
        
        // Yellow LED ON for 2 seconds
        RedLED = 0;
        YellowLED = 1;
        GreenLED = 0;
        delay_seconds(2);
        
        // Green LED ON for 5 seconds
        RedLED = 0;
        YellowLED = 0;
        GreenLED = 1;
        delay_seconds(5);
    }
}
___________________________________________________________________________________________________________________________________________________________________________
2. Title: Counter Control System Using MCU

Objective: Write an Embedded C program that performs the following functions:

Switch 1 → Resets the counter to zero.
Switch 2 → Increments the counter by one.
Switch 3 → Decrements the counter by one.
Display the counter value on display.

Hardware Components Required:
Microcontroller: AT89C52 (8051 family)
7-Segment Display: Common cathode
Push Buttons (Switches): 3 (for Reset, Increment, and Decrement)
Resistors: 10kΩ (for pull-up configuration)
Crystal Oscillator: 11.0592 MHz
Capacitors: 22pF (for oscillator stabilization)
Power Supply: 5V DC
Connecting Wires
Breadboard/PCB

Hardware Connection:
Switch Connections:
Reset Switch (S1) → Connected to P3.0 (Active LOW)
Increment Switch (S2) → Connected to P3.1 (Active LOW)
Decrement Switch (S3) → Connected to P3.2 (Active LOW)

7-Segment Display Connection (Common Cathode, Single Digit):
P2 → Connected to the 7-segment display segments

Power Supply:
VCC (5V) → Microcontroller and 7-Segment Display
GND → Common Ground for all components

Software Used:
Keil uVision – For writing and compiling the C program
Proteus – For simulating the circuit
Flash Magic – For burning the code into the microcontroller

Working:
Initially, the counter starts at zero and is displayed on the 7-segment display.
When the Reset button (S1) is pressed, the counter resets to zero.
When the Increment button (S2) is pressed, the counter increases by one, up to a maximum of 9.
When the Decrement button (S3) is pressed, the counter decreases by one, down to a minimum of 0.
The system continuously updates the 7-segment display with the current counter value.
A debounce delay is added to ensure accurate button detection.

#include <reg52.h>  // Header file for 89C52

// Define switch connections
sbit ResetSW = P3^0;
sbit IncrementSW = P3^1;
sbit DecrementSW = P3^2;

// 7-segment display patterns (common cathode)
unsigned char seg_pattern[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

int counter = 0; // Global counter variable

// Simple delay function in milliseconds
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++); // Adjust based on clock frequency
}

// Modified button press detection without sbit parameter
bit checkReset() { return (ResetSW == 0); }
bit checkIncrement() { return (IncrementSW == 0); }
bit checkDecrement() { return (DecrementSW == 0); }

// Function to display number on 7-segment
void display_number(int num) {
    // For single digit display (0-9)
    if(num < 0) num = 0; // Ensure non-negative
    if(num > 9) num = 9; // Limit to single digit
    
    P2 = seg_pattern[num]; // Display the number
}

void main() {
    // Initialize switches as input
    ResetSW = 1;
    IncrementSW = 1;
    DecrementSW = 1;
    
    while(1) {
        // Check reset button with debounce
        if(checkReset()) {
            delay_ms(20);
            if(checkReset()) {
                while(checkReset());
                counter = 0;
            }
        }
        
        // Check increment button with debounce
        if(checkIncrement()) {
            delay_ms(20);
            if(checkIncrement()) {
                while(checkIncrement());
                if(counter < 9) counter++;
            }
        }
        
        // Check decrement button with debounce
        if(checkDecrement()) {
            delay_ms(20);
            if(checkDecrement()) {
                while(checkDecrement());
                if(counter > 0) counter--;
            }
        }
        
        // Display current counter value
        display_number(counter);
        
        // Small delay for display stability
        delay_ms(100);
    }
}

_________________________________________________________________________________________________________________________________________________________________
3. Title: Seven Segment Display - Counting 0 to 9 Using MCU

Objective: Write an Embedded C  program to interface a 7-segment display with MCU that:

Displays digits from 0 to 9 sequentially.
Each digit should be displayed with a 1-second delay.
The sequence should continuously repeat.

Hardware Components Required:
Microcontroller: AT89C52 (8051 family)
7-Segment Display: Common cathode
Resistors: 330Ω (for current limiting)
Crystal Oscillator: 11.0592 MHz
Capacitors: 22pF (for stabilizing the oscillator)
Power Supply: 5V DC
Connecting Wires
Breadboard/PCB

Hardware Connection:
7-Segment Display Connection (Common Cathode, Single Digit):
Segment Pins (a-g) → Connected to P2.0 - P2.6
Common Cathode Pin → Connected to GND

Microcontroller Power Supply:
VCC (5V) → Microcontroller and 7-segment display
GND → Common ground for all components

Software Used:
Keil uVision – For writing and compiling the C program
Proteus – For simulating the circuit
Flash Magic – For burning the code into the microcontroller

Working:
The microcontroller continuously updates the 7-segment display to show numbers from 0 to 9.
Each digit is displayed for 1 second before moving to the next.
After reaching 9, the counter resets back to 0 and the sequence repeats indefinitely.
The 7-segment display patterns are stored in an array, and the value of P2 is updated accordingly.
A delay function ensures that each number remains displayed for 1 second before updating to the next digit.


#include <reg52.h>  // Header file for 89C52

// 7-segment display patterns (common cathode)
unsigned char seg_pattern[] = {
    0x3F, // 0 (a,b,c,d,e,f)
    0x06, // 1 (b,c)
    0x5B, // 2 (a,b,g,e,d)
    0x4F, // 3 (a,b,g,c,d)
    0x66, // 4 (f,g,b,c)
    0x6D, // 5 (a,f,g,c,d)
    0x7D, // 6 (a,f,g,e,c,d)
    0x07, // 7 (a,b,c)
    0x7F, // 8 (a,b,c,d,e,f,g)
    0x6F  // 9 (a,b,c,d,f,g)
};

// Function to create 1 second delay
void delay_1s() {
    unsigned int i, j;
    for(i = 0; i < 1000; i++)      // 1000ms = 1s
        for(j = 0; j < 1275; j++); // Adjust based on crystal frequency
}

void main() {
    unsigned char count = 0;
    
    while(1) {  // Infinite loop
        // Display current count on 7-segment
        P2 = seg_pattern[count];
        
        // 1 second delay
        delay_1s();
        
        // Increment count (0-9)
        count++;
        if(count > 9) count = 0;  // Reset after 9
    }
}

___________________________________________________________________________________________________________________________________________________________________________

4. Title: LCD Interfacing with MCU

Objective: Write an Embedded C  program to interface a 16x2 LCD display with MCU that performs the following tasks:

Display the message "Hello World!" on the first row of the LCD.
Continuously update the second row with a counter value that increments every second.
Implement a clear screen feature using a push button that resets the counter and clears the LCD display.


Hardware Components Required:
Microcontroller: AT89C52 (8051 family)
16x2 LCD Display
Resistors: 10kΩ (for contrast adjustment)
Potentiometer: 10kΩ (for LCD brightness control)
Crystal Oscillator: 11.0592 MHz
Capacitors: 22pF (for stabilizing the oscillator)
Push Button: For clearing the display and resetting the counter
Pull-up Resistor: 10kΩ for the push button
Power Supply: 5V DC
Connecting Wires
Breadboard/PCB

Hardware Connection:
LCD Display to Microcontroller (8051)
RS (Register Select) → P3.0
RW (Read/Write) → P3.1
EN (Enable) → P3.2
Data Pins (D0 - D7) → P2.0 - P2.7
VCC (5V) → Power Supply
GND → Common ground
VEE (Contrast Pin) → Middle pin of 10kΩ potentiometer
Push Button for Resetting Counter and Clearing LCD
One side connected to P3.3
Other side connected to GND
Pull-up resistor (10kΩ) connected between P3.3 and VCC

Software Used:
Keil uVision – For writing and compiling the C program
Proteus – For simulating the circuit
Flash Magic – For burning the code into the microcontroller

Working:
LCD Initialization: The microcontroller initializes the 16x2 LCD in 8-bit mode.
Displaying "Hello World!": The first row of the LCD always displays "Hello World!".
Counter Display: The second row continuously updates with an incrementing counter value every 1 second.
Clear Button Feature:
If the clear button is pressed, the counter resets to 0, and the LCD is cleared.
The LCD then re-displays "Hello World!" in the first row and resumes counting from zero.
Loop Execution: The program runs continuously, updating the display and responding to the button press.


#include <reg52.h>  // Header file for 89C52
#include <stdio.h>  // For sprintf function

// LCD control pins
sbit RS = P3^0;    // Register Select
sbit RW = P3^1;    // Read/Write
sbit EN = P3^2;    // Enable
sbit CLEAR_BTN = P3^3; // Clear screen button

// LCD data port
#define LCD_PORT P2

// Function prototypes
void LCD_Init();
void LCD_Command(unsigned char);
void LCD_Write(unsigned char);
void LCD_String(char*);
void LCD_Clear();
void delay_ms(unsigned int);
bit isButtonPressed();

unsigned int counter = 0;  // Global counter variable

void main() {
    char buffer[16];  // Buffer for counter display
    
    // Initialize LCD
    LCD_Init();
    
    // Display static message
    LCD_Command(0x80);  // Move cursor to first line
    LCD_String("Hello World!");
    
    while(1) {
        // Check clear button
        if(isButtonPressed()) {
            counter = 0;
            LCD_Clear();
            LCD_Command(0x80);  // Move cursor to first line
            LCD_String("Hello World!");
        }
        
        // Update counter display
        LCD_Command(0xC0);  // Move cursor to second line
        sprintf(buffer, "Count: %04d", counter);
        LCD_String(buffer);
        
        // Increment counter after 1 second
        delay_ms(1000);
        counter++;
    }
}

// Initialize LCD
void LCD_Init() {
    delay_ms(20);      // LCD power-up delay
    LCD_Command(0x38); // 8-bit mode, 2 lines, 5x7 dots
    LCD_Command(0x0C); // Display ON, cursor OFF
    LCD_Command(0x06); // Auto increment cursor
    LCD_Command(0x01); // Clear display
    delay_ms(2);       // Clear display delay
}

// Send command to LCD
void LCD_Command(unsigned char cmd) {
    LCD_PORT = cmd;
    RS = 0;  // Command mode
    RW = 0;  // Write mode
    EN = 1;  // High pulse
    delay_ms(1);
    EN = 0;
    delay_ms(1);
}

// Write data to LCD
void LCD_Write(unsigned char dat) {
    LCD_PORT = dat;
    RS = 1;  // Data mode
    RW = 0;  // Write mode
    EN = 1;  // High pulse
    delay_ms(1);
    EN = 0;
    delay_ms(1);
}

// Display string on LCD
void LCD_String(char *str) {
    while(*str) {
        LCD_Write(*str++);
    }
}

// Clear LCD screen
void LCD_Clear() {
    LCD_Command(0x01);  // Clear display command
    delay_ms(2);        // Clear display delay
}

// Button press detection with debounce
bit isButtonPressed() {
    if(CLEAR_BTN == 0) {  // Button pressed (active low)
        delay_ms(20);     // Debounce delay
        if(CLEAR_BTN == 0) {
            while(CLEAR_BTN == 0); // Wait for release
            return 1;
        }
    }
    return 0;
}

// Simple delay function in milliseconds
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++); // Adjust based on clock frequency
}


_____________________________________________________________________________________________________________________________________________________________________
5. Title: Basic Calculator Using MCU and Keypad with LCD Display

Objective:
Write an Embedded C  program to implement a basic calculator that performs the following arithmetic operations using a 4x4 keypad and a 16x2 LCD display:

Addition (+)
Subtraction (-)
Multiplication (*)
Division (/)
Functional Requirements:

The user should enter the first number.
The user should then press an operator key (+, -, *, /).
The user should enter the second number.
Pressing the ‘#’ key should display the result on the LCD.
Pressing the ‘*’ key should clear the display and reset the calculator.

Hardware Components Required
Microcontroller: 8051 (AT89C52 or similar)
7-Segment Display / LCD: 16x2 LCD for display
Keypad: 4x4 Matrix Keypad
Push Button: For reset or clear function
Resistors & Capacitors: As per circuit requirements
Crystal Oscillator: 11.0592 MHz for 8051
Power Supply: 5V DC
Connecting Wires & Breadboard / PCB
LEDs (if required for debugging)

Hardware Connection
For 7-Segment Display with MCU
Connect P2 of the microcontroller to the 7-segment display for segment control.
Ensure common cathode configuration.
Connect the GND of the display to the GND of MCU.
Use current-limiting resistors (220Ω) between each segment and MCU.

For LCD Interfacing
P2 (Data Pins D0-D7): Connect to LCD data pins.
P3.0 (RS): Connect to LCD RS.
P3.1 (RW): Connect to LCD RW.
P3.2 (EN): Connect to LCD Enable.
P3.3 (Clear Button): Connect push button to reset the display.

For Keypad Interfacing
Rows (R1-R4): Connect to P1.0-P1.3.
Columns (C1-C4): Connect to P1.4-P1.7.
Use pull-up resistors if necessary.

Software Used
Keil uVision – For Embedded C programming.
Proteus – For simulation.
Flash Magic / ISP Programmer – For flashing the program into the MCU.


Working
7-Segment Display Counting (0-9)
The microcontroller initializes the 7-segment display.
It continuously loops through numbers 0 to 9, displaying each for 1 second.
The process repeats indefinitely.
LCD Interfacing with MCU
The LCD initializes and displays "Hello World!" on the first row.
The counter updates every 1 second on the second row.
If the push button is pressed, the counter resets, and the display clears.
Basic Calculator with Keypad and LCD
The user enters the first number using the 4x4 keypad.
The user selects an operator (+, -, *, /).
The user enters the second number.
Pressing ‘=’ displays the result on the LCD.
Pressing ‘C’ clears the display and resets the calculator.


#include <reg52.h>
#include <stdio.h>

// LCD control pins
sbit RS = P3^0;
sbit RW = P3^1;
sbit EN = P3^2;

// Keypad connections
sbit ROW1 = P1^0;
sbit ROW2 = P1^1;
sbit ROW3 = P1^2;
sbit ROW4 = P1^3;
sbit COL1 = P1^4;
sbit COL2 = P1^5;
sbit COL3 = P1^6;
sbit COL4 = P1^7;

#define LCD_PORT P2

// Keypad mapping
const char keypad[4][4] = {
    {'7', '8', '9', '/'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'C', '0', '=', '+'}
};

// Calculator variables
float num1 = 0, num2 = 0, result = 0;
char op = 0;
unsigned char input_stage = 0; // 0:num1, 1:op, 2:num2
char buffer[16];

// Function prototypes
void LCD_Init(void);
void LCD_Command(unsigned char);
void LCD_Write(unsigned char);
void LCD_String(char*);
void LCD_Clear(void);
void delay_ms(unsigned int);
char Keypad_Scan(void);
void Calculator_Process(char key);
void Display_Result(void);

void main() {
    char key;
    
    LCD_Init();
    LCD_Clear();
    LCD_String("Calculator Ready");
    delay_ms(1000);
    LCD_Clear();
    
    while(1) {
        key = Keypad_Scan();
        if(key != 0) {
            Calculator_Process(key);
            delay_ms(200); // Debounce delay
        }
    }
}

void Calculator_Process(char key) {
    // Check for numeric input (0-9)
    if(key >= '0' && key <= '9') {
        if(input_stage == 0) {
            num1 = num1 * 10 + (key - '0');
            LCD_Clear();
            sprintf(buffer, "%.2f", num1);
            LCD_String(buffer);
        }
        else if(input_stage == 2) {
            num2 = num2 * 10 + (key - '0');
            LCD_Clear();
            sprintf(buffer, "%.2f %c %.2f", num1, op, num2);
            LCD_String(buffer);
        }
    }
    else {
        switch(key) {            
            case '+':
            case '-':
            case '*':
            case '/':
                if(input_stage == 0) {
                    op = key;
                    input_stage = 1;
                    LCD_Clear();
                    sprintf(buffer, "%.2f %c", num1, op);
                    LCD_String(buffer);
                }
                break;
                
            case '=': // Calculate result
                if(input_stage == 1) {
                    input_stage = 2;
                }
                else if(input_stage == 2) {
                    switch(op) {
                        case '+': result = num1 + num2; break;
                        case '-': result = num1 - num2; break;
                        case '*': result = num1 * num2; break;
                        case '/': 
                            if(num2 != 0) result = num1 / num2;
                            else {
                                LCD_Clear();
                                LCD_String("Error: Div by 0");
                                return;
                            }
                            break;
                    }
                    Display_Result();
                    input_stage = 0;
                }
                break;
                
            case 'C': // Clear
                num1 = num2 = result = 0;
                op = 0;
                input_stage = 0;
                LCD_Clear();
                break;
        }
    }
}

void Display_Result(void) {
    LCD_Clear();
    sprintf(buffer, "%.2f %c %.2f", num1, op, num2);
    LCD_String(buffer);
    LCD_Command(0xC0); // Move to second line
    sprintf(buffer, "= %.2f", result);
    LCD_String(buffer);
}

char Keypad_Scan(void) {
    char key = 0;
    
    // Scan Row 1
    COL1 = 0; COL2 = COL3 = COL4 = 1;
    if(!ROW1) { delay_ms(5); if(!ROW1) key = keypad[0][0]; }
    if(!ROW2) { delay_ms(5); if(!ROW2) key = keypad[1][0]; }
    if(!ROW3) { delay_ms(5); if(!ROW3) key = keypad[2][0]; }
    if(!ROW4) { delay_ms(5); if(!ROW4) key = keypad[3][0]; }
    COL1 = 1;
    
    // Scan Row 2
    COL2 = 0; COL1 = COL3 = COL4 = 1;
    if(!ROW1) { delay_ms(5); if(!ROW1) key = keypad[0][1]; }
    if(!ROW2) { delay_ms(5); if(!ROW2) key = keypad[1][1]; }
    if(!ROW3) { delay_ms(5); if(!ROW3) key = keypad[2][1]; }
    if(!ROW4) { delay_ms(5); if(!ROW4) key = keypad[3][1]; }
    COL2 = 1;
    
    // Scan Row 3
    COL3 = 0; COL1 = COL2 = COL4 = 1;
    if(!ROW1) { delay_ms(5); if(!ROW1) key = keypad[0][2]; }
    if(!ROW2) { delay_ms(5); if(!ROW2) key = keypad[1][2]; }
    if(!ROW3) { delay_ms(5); if(!ROW3) key = keypad[2][2]; }
    if(!ROW4) { delay_ms(5); if(!ROW4) key = keypad[3][2]; }
    COL3 = 1;
    
    // Scan Row 4
    COL4 = 0; COL1 = COL2 = COL3 = 1;
    if(!ROW1) { delay_ms(5); if(!ROW1) key = keypad[0][3]; }
    if(!ROW2) { delay_ms(5); if(!ROW2) key = keypad[1][3]; }
    if(!ROW3) { delay_ms(5); if(!ROW3) key = keypad[2][3]; }
    if(!ROW4) { delay_ms(5); if(!ROW4) key = keypad[3][3]; }
    COL4 = 1;
    
    return key;
}

void LCD_Init(void) {
    delay_ms(20);
    LCD_Command(0x38);
    LCD_Command(0x0C);
    LCD_Command(0x06);
    LCD_Command(0x01);
    delay_ms(2);
}

void LCD_Command(unsigned char cmd) {
    LCD_PORT = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    delay_ms(1);
    EN = 0;
    delay_ms(1);
}

void LCD_Write(unsigned char dat) {
    LCD_PORT = dat;
    RS = 1;
    RW = 0;
    EN = 1;
    delay_ms(1);
    EN = 0;
    delay_ms(1);
}

void LCD_String(char *str) {
    while(*str) {
        LCD_Write(*str++);
    }
}

void LCD_Clear(void) {
    LCD_Command(0x01);
    delay_ms(2);
}

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++);
}


______________________________________________________________________________________________________________________________________________________________________

6. Title: DC Motor Control System Using MCU
Objective:
Write an Embedded C  program to interface a DC motor with MCU that performs the following tasks:

Use a 3-way switch system to control motor actions:
Switch 1 → Start the motor in Clockwise direction.
Switch 2 → Start the motor in Anti-clockwise direction.
Switch 3 → Stop the motor.

DC Motor Control System Using MCU
Hardware Components Required
Microcontroller: 8051 (AT89C52 or similar)
Motor Driver IC: L293D or L298N
DC Motor: 12V DC Motor
Power Supply: 12V DC (For Motor), 5V DC (For MCU)
3 Push Buttons (Switches): For controlling the motor
Resistors: 10KΩ pull-up resistors for switches
Connecting Wires & Breadboard/PCB

Hardware Connection
Motor Driver (L293D) to Microcontroller (8051)
IN1 (L293D Pin 2) → P1.0 (MOTOR_IN1)
IN2 (L293D Pin 7) → P1.1 (MOTOR_IN2)
Enable Pin (L293D Pin 1) → P1.2 (MOTOR_ENA)
Vcc (L293D Pin 16) → +5V
GND (L293D Pins 4, 5, 12, 13) → GND
Motor Power (L293D Pin 8) → +12V
Motor Output (L293D Pins 3, 6) → DC Motor Terminals

Switch Connections
Switch 1 (Clockwise) → P3.0 (SWITCH1)
Switch 2 (Anti-clockwise) → P3.1 (SWITCH2)
Switch 3 (Stop) → P3.2 (SWITCH3)
One side of all switches connected to GND
Other side connected to respective P3.x pins with a 10KΩ pull-up resistor

Software Used
Keil uVision – For Embedded C programming
Proteus – For simulation
Flash Magic / ISP Programmer – For flashing the program into the MCU


Working
The system initializes the motor control pins and sets up the switches as inputs.
When Switch 1 is pressed, the motor rotates clockwise.
When Switch 2 is pressed, the motor rotates anti-clockwise.
When Switch 3 is pressed, the motor stops immediately.
The program uses a while loop to ensure switch de-bouncing and waits for the button to be released before taking another action.

#include <reg52.h>  // Header file for 89C52

// Motor Driver Pins (L293D or L298N)
sbit MOTOR_IN1 = P1^0;  // Motor Input 1 (Clockwise)
sbit MOTOR_IN2 = P1^1;  // Motor Input 2 (Anti-clockwise)
sbit MOTOR_ENA = P1^2;  // Enable Pin for Motor Driver

// Switch Pins
sbit SWITCH1 = P3^0;    // Start Clockwise
sbit SWITCH2 = P3^1;    // Start Anti-clockwise
sbit SWITCH3 = P3^2;    // Stop Motor

// Function prototypes
void delay_ms(unsigned int);
void Motor_Stop();
void Motor_CW();
void Motor_CCW();

void main() {
    // Initialize motor control pins
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;
    MOTOR_ENA = 1;  // Enable motor driver
    
    // Initialize switch pins as input
    SWITCH1 = 1;
    SWITCH2 = 1;
    SWITCH3 = 1;
    
    while(1) {
        if(SWITCH1 == 0) {       // Clockwise Rotation
            Motor_CW();
            while(SWITCH1 == 0); // Wait for switch release
        }
        else if(SWITCH2 == 0) {  // Anti-clockwise Rotation
            Motor_CCW();
            while(SWITCH2 == 0); // Wait for switch release
        }
        else if(SWITCH3 == 0) {  // Stop Motor
            Motor_Stop();
            while(SWITCH3 == 0); // Wait for switch release
        }
    }
}

void Motor_CW() {
    MOTOR_IN1 = 1;
    MOTOR_IN2 = 0;
}

void Motor_CCW() {
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 1;
}

void Motor_Stop() {
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;
}

// Simple delay function in milliseconds
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++); // Adjust based on clock frequency
}

____________________________________________________________________________________________________________________________________________________________________________

7. Title: Servo Motor Control Using MCU

Objective:
Write an Embedded C program to interface a servo motor with the MCU that performs the following tasks using push buttons:

Button 1 → Move the servo motor to 0° position.
Button 2 → Move the servo motor to 90° position.
Button 3 → Move the servo motor to 180° position.
Display the servo angle on a connected LCD display

Hardware Components Required
Microcontroller: 8051 (AT89C52 or similar)
Servo Motor: SG90 or MG995
16x2 LCD Display (for angle display)
Push Buttons (3 units): For 0°, 90°, and 180° position control
Resistors: 10KΩ pull-up resistors for switches
Power Supply: 5V DC (for MCU, Servo, and LCD)
Connecting Wires & Breadboard/PCB

Hardware Connection
Servo Motor to Microcontroller (8051)
Servo Signal Pin → P1.0 (SERVO_PIN)
Servo VCC → 5V
Servo GND → GND

Button Connections
Button 1 (0° Position) → P3.0 (BTN_0)
Button 2 (90° Position) → P3.1 (BTN_90)
Button 3 (180° Position) → P3.2 (BTN_180)
One side of all buttons → GND
Other side → Respective P3.x pins with 10KΩ pull-up resistor

LCD Display (16x2) to Microcontroller (8051)
RS → P2.0
RW → P2.1
EN → P2.2
Data Lines (D0-D7) → P0 (LCD_DATA)
VCC → 5V, GND → GND

Software Used
Keil uVision – For Embedded C programming
Proteus – For simulation
Flash Magic / ISP Programmer – For flashing the program into the MCU

Working
The system initializes the LCD display and sets the servo motor to 90° as the default position.
When Button 1 is pressed, the servo motor moves to 0° position, and the LCD updates the angle.
When Button 2 is pressed, the servo motor moves to 90° position, and the LCD updates accordingly.
When Button 3 is pressed, the servo motor moves to 180° position, and the LCD reflects the change.
The system uses PWM (Pulse Width Modulation) to control the servo's angle by generating precise pulse widths.
Debounce delays are included to prevent multiple detections from a single button press.

#include <reg52.h>
#include <stdio.h>

// Servo control pin
sbit SERVO_PIN = P1^0;

// Button pins
sbit BTN_0 = P3^0;   // 0° position
sbit BTN_90 = P3^1;  // 90° position
sbit BTN_180 = P3^2; // 180° position

// LCD control pins
sbit RS = P2^0;
sbit RW = P2^1;
sbit EN = P2^2;
#define LCD_DATA P0

// Variables
unsigned char angle = 90; // Default position
char lcd_buffer[16];

// Function prototypes
void LCD_Init();
void LCD_Command(unsigned char);
void LCD_Write(unsigned char);
void LCD_String(char*);
void LCD_Clear();
void delay_us(unsigned int);
void delay_ms(unsigned int);
void Set_Servo_Angle(unsigned char);
void Update_LCD_Display();

void main() {
    // Initialize buttons as input
    BTN_0 = 1;
    BTN_90 = 1;
    BTN_180 = 1;
    
    // Initialize LCD
    LCD_Init();
    LCD_Clear();
    
    // Initialize servo to default position (90°)
    Set_Servo_Angle(90);
    Update_LCD_Display();
    
    while(1) {
        if(BTN_0 == 0) {       // Move to 0°
            delay_ms(20);       // Debounce delay
            if(BTN_0 == 0) {
                angle = 0;
                Set_Servo_Angle(angle);
                Update_LCD_Display();
                while(BTN_0 == 0); // Wait for button release
            }
        }
        else if(BTN_90 == 0) {  // Move to 90°
            delay_ms(20);
            if(BTN_90 == 0) {
                angle = 90;
                Set_Servo_Angle(angle);
                Update_LCD_Display();
                while(BTN_90 == 0);
            }
        }
        else if(BTN_180 == 0) { // Move to 180°
            delay_ms(20);
            if(BTN_180 == 0) {
                angle = 180;
                Set_Servo_Angle(angle);
                Update_LCD_Display();
                while(BTN_180 == 0);
            }
        }
    }
}

void Set_Servo_Angle(unsigned char angle) {
    unsigned int pulse_width;
    
    // Convert angle to pulse width (500-2500µs)
    pulse_width = 500 + (angle * 2000) / 180;
    
    // Generate PWM signal (20ms period)
    SERVO_PIN = 1;
    delay_us(pulse_width);
    SERVO_PIN = 0;
    delay_ms(20 - (pulse_width / 1000));
}

void Update_LCD_Display() {
    LCD_Command(0x80); // Move to first line
    LCD_String("Servo Position:");
    LCD_Command(0xC0); // Move to second line
    sprintf(lcd_buffer, "Angle: %3d", angle);
    LCD_String(lcd_buffer);
}

// LCD Functions
void LCD_Init() {
    delay_ms(20);
    LCD_Command(0x38); // 8-bit mode, 2 lines, 5x7 dots
    LCD_Command(0x0C); // Display ON, cursor OFF
    LCD_Command(0x06); // Auto increment cursor
    LCD_Command(0x01); // Clear display
    delay_ms(2);
}

void LCD_Command(unsigned char cmd) {
    LCD_DATA = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    delay_ms(1);
    EN = 0;
    delay_ms(1);
}

void LCD_Write(unsigned char dat) {
    LCD_DATA = dat;
    RS = 1;
    RW = 0;
    EN = 1;
    delay_ms(1);
    EN = 0;
    delay_ms(1);
}

void LCD_String(char *str) {
    while(*str) {
        LCD_Write(*str++);
    }
}

void LCD_Clear() {
    LCD_Command(0x01);
    delay_ms(2);
}

// Delay functions
void delay_us(unsigned int us) {
    while(us--) {
        /* Approximately 1µs delay at 12MHz clock */
        /* Adjust based on your crystal frequency */
    }
}

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++); // Adjust for 12MHz
}

___________________________________________________________________________________________________________________________________________________________________________

8. Title: Temperature Sensor (LM35) Interfacing with MCU

Objective:
Write an Embedded C program to interface an LM35 Temperature Sensor with the MCU that performs the following tasks:

Continuously read the temperature value from the LM35 sensor.
Display the temperature value on an LCD display or Serial Monitor.

Hardware Components Required
Microcontroller: 8051 (AT89C52 or similar)
Temperature Sensor: LM35
Analog-to-Digital Converter (ADC): ADC0831 (8-bit ADC)
16x2 LCD Display (for temperature display)
Resistors: 10KΩ pull-up for switches, 1KΩ for signal stabilization
Capacitors: 0.1µF for noise filtering
Power Supply: 5V DC (for MCU, LCD, ADC, and LM35)
Connecting Wires & Breadboard/PCB

Hardware Connection
LM35 Temperature Sensor to ADC0831
LM35 VCC → 5V
LM35 GND → GND
LM35 Output → ADC0831 CH0 (Analog Input Pin 2)

ADC0831 to 8051 Microcontroller
CS (Pin 1) → P1.0 (ADC_CS)
CLK (Pin 7) → P1.1 (ADC_CLK)
DO (Pin 6) → P1.2 (ADC_DO)
VCC (Pin 8) → 5V
GND (Pin 4) → GND

LCD Display (16x2) to Microcontroller
RS → P2.5
EN → P2.6
D4-D7 → P3.4 to P3.
RW → GND
VCC → 5V, GND → GND

Software Used
Keil uVision – For Embedded C programming
Proteus – For simulation
Flash Magic / ISP Programmer – For flashing the program into the MCU

Working
The LM35 temperature sensor outputs an analog voltage proportional to temperature (10mV/°C).
The ADC0831 converts this analog signal into an 8-bit digital value (0-255) corresponding to a 0-5V range.
The 8051 microcontroller reads this digital value via software SPI communication.

The temperature is calculated as:
Temperature

![image](https://github.com/user-attachments/assets/7b46c892-493c-4518-9ead-026864f661a0)

The calculated temperature value is displayed on the 16x2 LCD display.
The process repeats every 0.5 seconds to update the temperature reading continuously.

#include <reg52.h>
#include <stdio.h>
#include <intrins.h>

/*--- Hardware Connections ---*/
// ADC0831 Interface (Software SPI)
sbit ADC_CS  = P1^0;   // Chip Select (Active Low)
sbit ADC_CLK = P1^1;   // Clock
sbit ADC_DO  = P1^2;   // Data Out

// LCD Interface (4-bit mode to save pins)
sbit RS = P2^5;        // Register Select
sbit EN = P2^6;        // Enable
#define LCD_DATA P3    // Using P3.4-P3.7 (D4-D7)

/*--- Global Variables ---*/
float temperature = 0.0;
char lcd_buffer[16];

/*--- Function Prototypes ---*/
void System_Init(void);
float Read_Temperature(void);
unsigned char ADC_Read(void);
void Update_Display(void);
void LCD_Init_4bit(void);
void LCD_Write_4bit(unsigned char dat, bit mode);
void LCD_Command_4bit(unsigned char cmd);
void LCD_String_4bit(char *str);
void LCD_Clear(void);
void delay_ms(unsigned int ms);
void delay_us(unsigned int us);

/*--- Main Program ---*/
void main() {
    System_Init();
    
    while(1) {
        temperature = Read_Temperature();
        Update_Display();
        delay_ms(500); // Update every 0.5 seconds
    }
}

/*--- System Initialization ---*/
void System_Init(void) {
    // Initialize ADC pins
    ADC_CS = 1;
    ADC_CLK = 0;
    
    // Initialize LCD
    LCD_Init_4bit();
    LCD_Clear();
    LCD_String_4bit("Temperature:");
}

/*--- Temperature Measurement ---*/
float Read_Temperature(void) {
    unsigned char adc_value = ADC_Read();
    // LM35: 10mV/ C, ADC0831: 5V range, 8-bit (256 steps)
    return (adc_value * 500.0) / 256.0; // (adc_value * 5.0 * 100) / 256
}

/*--- ADC Read Function ---*/
unsigned char ADC_Read(void) {
    unsigned char i, value = 0;
    
    ADC_CS = 0;         // Start conversion
    _nop_(); _nop_();   // Small delay (tCSS)
    
    // Read 8 bits MSB first
    for(i = 0; i < 8; i++) {
        ADC_CLK = 1;
        _nop_();
        value <<= 1;
        if(ADC_DO) value |= 0x01;
        ADC_CLK = 0;
        _nop_();
    }
    
    ADC_CS = 1; // End conversion
    return value;
}

/*--- Display Update ---*/
void Update_Display(void) {
    LCD_Command_4bit(0xC0); // Move to second line
    sprintf(lcd_buffer, "%.1f C  ", temperature);
    LCD_String_4bit(lcd_buffer);
}

/*--- LCD Functions (4-bit mode) ---*/
void LCD_Init_4bit(void) {
    delay_ms(50);       // Power-on delay
    
    // Initialization sequence for 4-bit mode
    LCD_Command_4bit(0x33);
    LCD_Command_4bit(0x32);
    LCD_Command_4bit(0x28); // 4-bit, 2-line, 5x8 font
    LCD_Command_4bit(0x0C); // Display ON, cursor OFF
    LCD_Command_4bit(0x06); // Auto increment cursor
    LCD_Clear();
}

void LCD_Write_4bit(unsigned char dat, bit mode) {
    RS = mode;
    
    // High nibble first
    LCD_DATA = (LCD_DATA & 0x0F) | (dat & 0xF0);
    EN = 1; delay_us(1); EN = 0;
    
    // Low nibble
    LCD_DATA = (LCD_DATA & 0x0F) | ((dat << 4) & 0xF0);
    EN = 1; delay_us(1); EN = 0;
    
    delay_us(100); // Most commands need 40 s
}

void LCD_Command_4bit(unsigned char cmd) {
    LCD_Write_4bit(cmd, 0);
}

void LCD_String_4bit(char *str) {
    while(*str) {
        LCD_Write_4bit(*str++, 1);
    }
}

void LCD_Clear(void) {
    LCD_Command_4bit(0x01);
    delay_ms(2); // Clear display needs 1.52ms
}

/*--- Delay Functions ---*/
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++); // For 12MHz
}

void delay_us(unsigned int us) {
    while(us--) {
        _nop_(); _nop_(); _nop_(); _nop_();
    }
}


____________________________________________________________________________________________________________________________________________________________________________

9. Title: DHT11 Temperature and Humidity Sensor Interfacing with MCU

Objective:
Write an Embedded C program to interface a DHT11 Temperature and Humidity Sensor with the MCU that performs the following tasks:

Read temperature and humidity data from the DHT11 sensor.
Display the sensor readings on an LCD display.


Hardware Components Required
Microcontroller: 8051 (AT89C52 or similar)
Temperature & Humidity Sensor: DHT11
16x2 LCD Display (for displaying sensor readings)
10KΩ Pull-up Resistor (for DHT11 data pin)
Power Supply: 5V DC
Connecting Wires & Breadboard/PCB

Hardware Connection
DHT11 Sensor to 8051
VCC → 5V
GND → GND
DATA → P1.0 (DHT11_PIN) with a 10KΩ pull-up resistor to 5V

LCD Display (16x2) to Microcontroller
RS → P2.5
EN → P2.6
D4-D7 → P3.4 to P3.7
RW → GND
VCC → 5V, GND → GND

Software Used
Keil uVision – For Embedded C programming
Proteus – For simulation
Flash Magic / ISP Programmer – For flashing the program into the MCU


Working
The DHT11 sensor measures temperature and humidity and sends data in a digital format using a single-wire protocol.
The 8051 microcontroller initiates the communication by sending a low pulse (18ms) to the DHT11 sensor.
The DHT11 sensor responds with an acknowledgment and then transmits 5 bytes of data:

Humidity Integer Byte
Humidity Decimal Byte
Temperature Integer Byte
Temperature Decimal Byte
Checksum Byte (Sum of previous four bytes)

The MCU reads and verifies the checksum to ensure data integrity.
If the checksum is valid, the temperature and humidity values are extracted and displayed on the 16x2 LCD in the format:

```
H:xx% T:xx°C
```
The data is updated every 2 seconds to reflect real-time changes.


#include <reg52.h>
#include <stdio.h>
#include <intrins.h>

// DHT11 Connection
sbit DHT11_PIN = P1^0;

// LCD Interface (4-bit mode)
sbit RS = P2^5;
sbit EN = P2^6;
#define LCD_DATA P3

// Global Variables
unsigned char humidity_int, humidity_dec;
unsigned char temp_int, temp_dec;
unsigned char checksum;
char lcd_buffer[16];

// Function Prototypes
void LCD_Init_4bit(void);
void LCD_Command_4bit(unsigned char);
void LCD_Write_4bit(unsigned char, bit);
void LCD_String_4bit(char*);
void LCD_Clear(void);
void delay_ms(unsigned int);
void delay_us(unsigned int);
bit DHT11_Start(void);
unsigned char DHT11_Read_Byte(void);
bit DHT11_Read_Data(void);
void Update_Display(void);

void main() {
    // Initialize LCD
    LCD_Init_4bit();
    LCD_Clear();
    LCD_String_4bit("DHT11 Sensor");
    
    while(1) {
        if(DHT11_Read_Data()) {
            Update_Display();
        } else {
            LCD_Command_4bit(0xC0);
            LCD_String_4bit("Read Error!");
        }
        delay_ms(2000); // Wait 2 seconds between readings
    }
}

/* DHT11 Communication Functions */
bit DHT11_Start(void) {
    DHT11_PIN = 0;      // Pull low for 18ms
    delay_ms(18);
    DHT11_PIN = 1;      // Release bus
    delay_us(30);       // Wait 20-40us
    
    // Check DHT11 response
    if(!DHT11_PIN) {
        delay_us(80);   // Wait for low pulse (80us)
        if(DHT11_PIN) {
            delay_us(80); // Wait for high pulse (80us)
            return 1;   // DHT11 responded
        }
    }
    return 0;           // No response
}

unsigned char DHT11_Read_Byte(void) {
    unsigned char i, byte = 0;
    
    for(i=0; i<8; i++) {
        while(!DHT11_PIN);  // Wait for high
        delay_us(30);       // Wait 30us
        
        // If still high after 30us, it's a '1'
        if(DHT11_PIN) {
            byte |= (1 << (7-i));
            while(DHT11_PIN); // Wait for low
        }
    }
    return byte;
}

bit DHT11_Read_Data(void) {
    if(!DHT11_Start()) return 0;
    
    humidity_int = DHT11_Read_Byte();
    humidity_dec = DHT11_Read_Byte();
    temp_int = DHT11_Read_Byte();
    temp_dec = DHT11_Read_Byte();
    checksum = DHT11_Read_Byte();
    
    // Verify checksum
    if((humidity_int + humidity_dec + temp_int + temp_dec) != checksum)
        return 0;
    
    return 1;
}

/* Display Functions */
void Update_Display(void) {
    LCD_Command_4bit(0xC0); // Second line
    sprintf(lcd_buffer, "H:%d%% T:%dC", humidity_int, temp_int);
    LCD_String_4bit(lcd_buffer);
}

/* LCD Functions (4-bit mode) */
void LCD_Init_4bit(void) {
    delay_ms(50);
    LCD_Command_4bit(0x33);
    LCD_Command_4bit(0x32);
    LCD_Command_4bit(0x28); // 4-bit, 2-line, 5x8
    LCD_Command_4bit(0x0C); // Display ON, cursor OFF
    LCD_Command_4bit(0x06); // Auto increment
    LCD_Command_4bit(0x01); // Clear display
    delay_ms(2);
}

void LCD_Write_4bit(unsigned char dat, bit mode) {
    RS = mode;
    LCD_DATA = (LCD_DATA & 0x0F) | (dat & 0xF0);
    EN = 1; delay_us(1); EN = 0;
    LCD_DATA = (LCD_DATA & 0x0F) | ((dat << 4) & 0xF0);
    EN = 1; delay_us(1); EN = 0;
    delay_us(100);
}

void LCD_Command_4bit(unsigned char cmd) {
    LCD_Write_4bit(cmd, 0);
}

void LCD_String_4bit(char *str) {
    while(*str) LCD_Write_4bit(*str++, 1);
}

void LCD_Clear(void) {
    LCD_Command_4bit(0x01);
    delay_ms(2);
}

/* Delay Functions */
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<1275; j++); // For 12MHz
}

void delay_us(unsigned int us) {
    while(us--) {
        _nop_(); _nop_(); _nop_(); _nop_();
    }
}

_______________________________________________________________________________________________________________________________________________________________________
