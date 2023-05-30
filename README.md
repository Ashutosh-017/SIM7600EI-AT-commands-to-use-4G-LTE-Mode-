# SIM7600EI-AT-commands-to-use-4G-LTE-Mode-

# Send AT Commands to SIM7600EI module using ESP IDF

# AT Command Module Communication

This project demonstrates communication with SIM7600EI module  using AT commands via UART. The program initializes the module, checks SIM card status, registers with the network, checks signal quality, and configures network modes.

## Prerequisites

- ESP32  development board (or any other compatible board)
- UART-enabled module

## Getting Started
 
 1. You only need to copy main.c file code to your main.c file and build it and then flush it to microcontroller.

## Usage

1. Modify the code if necessary to adapt it to your specific module's AT command requirements. For example, you may need to change the baud rate, command strings, expected responses, or timeouts.

2. Make sure the UART pins (TXD_PIN and RXD_PIN) in the code match the physical connections on your development board.

3. Adjust the delays (vTaskDelay) between different AT commands if needed, depending on the module's response time.

4. Upload the code to your board and open the serial monitor to observe the program's output.

## Contributing

Contributions to this project are welcome. Feel free to open issues and submit pull requests to suggest improvements, add new features, or fix bugs.

