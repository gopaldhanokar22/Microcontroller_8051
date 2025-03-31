#include <reg51.h>
#define LCD P2  // Define Port 2 as LCD data port

sbit RS = P3^0;  // Register Select pin
sbit EN = P3^1;  // Enable pin
sbit IR1 = P1^0; // IR Sensor 1 (Entrance Detector)
sbit IR2 = P1^1; // IR Sensor 2 (Exit Detector)

unsigned int in_count = 0, out_count = 0, current_count = 0;

void delay(unsigned int time) {
    unsigned int i, j;
    for(i = 0; i < time; i++)
        for(j = 0; j < 1275; j++);
}

void lcd_cmd(unsigned char cmd) {
    LCD = cmd;
    RS = 0;
    EN = 1;
    delay(1);
    EN = 0;
}

void lcd_data(unsigned char value) {  // Renamed 'data' to 'value'
    LCD = value;
    RS = 1;
    EN = 1;
    delay(1);
    EN = 0;
}

void lcd_init() {
    lcd_cmd(0x38); // 2-line mode, 5x7 matrix
    lcd_cmd(0x0C); // Display ON, Cursor OFF
    lcd_cmd(0x06); // Auto increment cursor
    lcd_cmd(0x01); // Clear display
    lcd_cmd(0x80); // Move cursor to home position
}

void lcd_string(char *str) {
    while(*str) {
        lcd_data(*str);
        str++;
    }
}

void update_display() {
    lcd_cmd(0x80);
    lcd_string("IN: ");
    lcd_data(in_count + '0');
    lcd_string("  OUT: ");
    lcd_data(out_count + '0');
    
    lcd_cmd(0xC0);
    lcd_string("Current: ");
    lcd_data(current_count + '0');
}

void main() {
    P1 = 0xFF; // Set IR sensor pins as input
    lcd_init();
    update_display();
    
    while(1) {
        if (IR1 == 0) {
            delay(50);
            if (IR2 == 0) {
                in_count++;
                current_count = in_count - out_count;
                update_display();
                while(IR1 == 0 || IR2 == 0); // Wait for both sensors to return to normal state
            }
        }
        else if (IR2 == 0) {
            delay(50);
            if (IR1 == 0) {
                if (out_count < in_count) {
                    out_count++;
                    current_count = in_count - out_count;
                    update_display();
                    while(IR1 == 0 || IR2 == 0);
                }
            }
        }
    }
}
