#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "uart.h" // Include UART driver for your platform
#include "timer.h" // Include timer library if required for delays

#define TIMEOUT_10S 10000  // Define timeouts in milliseconds
#define TIMEOUT_20S 20000

// Commands
const char* commands[] = {
    "GET SENSOR-A",
    "GET DEVICE-VERSION"
};

// Timeouts for each command
const uint16_t timeouts[] = {
    TIMEOUT_10S,
    TIMEOUT_20S
};

void send_command(const char* cmd, uint16_t timeout) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%s %ds\n", cmd, timeout / 1000);

    // Send command via UART
    UART_Write(buffer, strlen(buffer));

    // Start timer and wait for response within timeout
    uint16_t start = Timer_GetMillis();
    char response[100] = {0};
    uint8_t index = 0;
    
    while (Timer_GetMillis() - start < timeout) {
        if (UART_Available()) { // Check if data is available
            char c = UART_Read(); // Read data from UART
            if (c == '\n' || index >= sizeof(response) - 1) {
                break;  // End of response or buffer full
            }
            response[index++] = c;
        }
    }
    response[index] = '\0'; // Null-terminate the response string

    // Print command and response
    printf("Command: %s, Response: %s\n", cmd, response);
}

int main() {
    // Initialize UART and Timer
    UART_Init(9600);  // Initialize UART with baud rate 9600
    Timer_Init();     // Initialize timer if necessary

    // Loop through commands and send them with respective timeouts
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        send_command(commands[i], timeouts[i]);
    }

    return 0;
}
