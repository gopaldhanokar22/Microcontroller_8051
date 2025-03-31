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
