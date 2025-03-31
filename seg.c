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