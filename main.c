#include <stdio.h>
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"
#include "stdlib.h"
#include "esp_err.h"

#define BUF_SIZE (512)

#define TXD_PIN (GPIO_NUM_21) // Tx
#define RXD_PIN (GPIO_NUM_20) // Rx


bool success;

void uart_init(void) 

{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,          // UART driver setting 
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

}

bool parse_data(uint8_t* data, size_t len, int timeout_ms , const char* word)
{
    char resp[BUF_SIZE] = {0};   // Allocate a buffer to hold the received data

    TickType_t start_time = xTaskGetTickCount(); // get the start time of the loop
    
    //printf("\nparsing data start\n");

    while ((xTaskGetTickCount() - start_time) < (timeout_ms / portTICK_PERIOD_MS))
    {
        int bytes_read = uart_read_bytes(UART_NUM_0, data, len, pdMS_TO_TICKS(500));

        // printf( "\nBytes read : %d\n",bytes_read);

        if (bytes_read > 0)
        {
            // Append the received data to the buffer
            strncat(resp, (const char*)data, bytes_read);

            // Check if the word is present in the buffer
            if (strstr(resp, word) != NULL)
            {
                return true;
            }
        }
    }
    
    return false;
}



bool sendATCommand(char* command, char* expectedResponse, int timeoutMs) // Sending AT Command
{
    uint8_t buffer[BUF_SIZE]; // buffer to store response

    memset(buffer , 0 ,BUF_SIZE);

    uart_write_bytes(UART_NUM_0, command, strlen(command));

    //printf("\nWrite done\n");

   bool responseReceived = parse_data(buffer, BUF_SIZE , timeoutMs, expectedResponse);

  

    if (responseReceived) 
    {
        printf("\nExpected response received");

        printf("\nResponse: %s\n", buffer); // printing response

        return true;

    } else 
    {
        printf("\nExpected response not received");

        printf("\nResponse: %s\n", buffer);

        return false;
    }
    return true;
}

void module_init(void)
{
    success = sendATCommand("AT\r\n", "RDY", 5000);

    if (success)
    {
        printf("Module Initiation done\n");
    } 

    else
    {
        printf("Module Initiation fail !\n");
    }    
}

void module_func(void)
{
    success = sendATCommand("AT+CFUN?\r\n", "+CFUN: 1", 5000);

    if (success)
    {
        printf("Module is in full functionality, online mode\n");
    } 

    else
    {
        printf("Module is in minimum functionality\n");
    }    
}

void checking_SIM(void)
{
    success = sendATCommand("AT+CPIN?\r\n", "+CPIN: READY", 5000);

    if (success)
    {
        printf("SIM is present in module\n");
    } 

    else
    {
      printf("SIM is not present in module!\n");
    }    
}

void check_SIM_reg(void)
{
    success = sendATCommand("AT+CREG?\r\n", "+CREG: 0,1", 5000);

    if (success)
    {
        printf("Module Registration done\n");
    } 

    else
    {
        printf("Module Registration fail !\n");
    }

}

void Query_signal_quality(void)
{
    success = sendATCommand("AT+CSQ\r\n", "OK", 5000);

    if (success)
    {
        printf("Signal quality\n");
    }

    else
    {
        printf("Fail to check signal quality!\n");
    }

}

void check_network_mode(void)
{
    success = sendATCommand("AT+CNMP?\r\n" , "OK" , 5000);

    if (success)
    {
        printf("Module is work in this mode \n");
    } 

    else
    {
       printf("Fail to check mode!\n");
    }

}

void change_module_network_mode(void)
{
    success = sendATCommand("AT+CNMP=38\r\n", "OK", 5000);

    if (success)
    {
        printf("Module mode change to LTE\n");
    } 

    else
    {
       printf("Fail to change Module mode to LTE!\n");
    }
}

void check_network_system_mode(void)
{
    success = sendATCommand("AT+CNSMOD?\r\n", "OK", 5000);

    if (success)
    {
        printf("Current network system mode\n");
    } 

    else
    {
       printf("Fail to check Current network system mode!\n");
    }    
}


void app_main(void)

{
    uart_init();
    vTaskDelay(18000 / portTICK_PERIOD_MS);
    //Initiate module 
    module_init();
    vTaskDelay(2000/portTICK_PERIOD_MS);
    // check SIM is ready or not 
    checking_SIM();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    // Check SIM register with network
    check_SIM_reg();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    // Check SIM signal quality
    Query_signal_quality();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    // Check network mode of module
    check_network_mode();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    // change network mode of module 
    change_module_network_mode();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    // Check module network mode 
    check_network_mode();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
       // Check SIM register with network
    check_SIM_reg();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
        // Checking signal quality
    Query_signal_quality();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    // Check module network mode
    check_network_mode();
    vTaskDelay(5000 / portTICK_PERIOD_MS);  
}

