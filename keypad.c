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