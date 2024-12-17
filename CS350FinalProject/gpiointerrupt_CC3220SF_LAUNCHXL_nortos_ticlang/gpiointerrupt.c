/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== gpiointerrupt.c ========
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/UART2.h>

/* Driver configuration */
#include "ti_drivers_config.h"


//#include <string.h>
//#include <ti/drivers/Board.h>

#define DISPLAY(x) UART2_write(uart, &output, x, NULL);

#define MAX_PATTERN_LENGTH 11

#define CONFIG_GPIO_RED_LED  9
#define CONFIG_GPIO_YELLOW_LED  10
//#define CONFIG_GPIO_GREEN_LED  11

// UART Global Variables
char output[64];
int bytesToSend;

// I2C Global Variables
static const struct {
    uint8_t address;
    uint8_t resultReg;
    char *id;
    } sensors[3] = {
    { 0x48, 0x0000, "11X" },
    { 0x49, 0x0000, "116" },
    { 0x41, 0x0001, "006" }
};

uint8_t txBuffer[1];
uint8_t rxBuffer[2];
I2C_Transaction i2cTransaction;

// Driver Handles - Global variables
volatile int timerFlag = 0;

typedef enum {
    STATE_OFF,
    STATE_ON
} HeatState;


// Driver Handles - Global variables
UART2_Handle uart;
I2C_Handle i2c;
Timer_Handle timer0;

// Timer intervals (in milliseconds)
const int BUTTON_CHECK_INTERVAL = 200000;
const int TEMP_CHECK_INTERVAL = 300000;
const int REPORT_INTERVAL = 500000;

// state, interrupt, and elapsed time variables
volatile HeatState currentState = STATE_OFF;
volatile int button0Flag = 0;
volatile int button1Flag = 0;
volatile uint32_t seconds = 0;

// Global variables to store the temperature, set-point, heater status, and timer count
volatile int16_t ambient_temp = 25;      // Example initial temperature in Celsius
volatile int16_t set_point = 22;         // Example initial set-point temperature in Celsius
volatile bool heater_on = false;     // Heater status (false = off, true = on)
volatile unsigned int seconds_since_reset = 0;  // Counter for seconds since reset

// method declarations
void initUART(void);
void initI2C(void);
int16_t readTemp(void); // check the ambient temperature
void sendStateToUART(void); // report to server
void setTimerPeriod(int); // encapsulation of set timer period operations
void timerCallback(Timer_Handle, int_fast16_t); // timer callback
void updateStateDisplay(void); // update the appropriate LED setting
void setTempStateON(void); // set the state
void setTempStateOFF(void); // set the state
void showTempStateON(void);  // turn LED ON
void showTempStateOFF(void); // turn LED OFF
void initTimer(void); // initialize timer
void gpioButtonFxn0(uint_least8_t); // button interrupt callback
void gpioButtonFxn1(uint_least8_t); // button interrupt callback
void checkSetPointButtons(void); // check to see if the buttons have been pressed
void compareAmbientToDesiredTemp(void); // read the ambient temp and compare to set_pointm then setTempState
void init_system(void); // system initialization command to keep the main thread clean and readable

// Driver Handles - Global variables
I2C_Handle i2c;

// initUART
void initUART(void)
{
    UART2_Params uartParams;
    size_t bytesRead;
    size_t bytesWritten = 0;
    uint32_t status = UART2_STATUS_SUCCESS;

    /* Create a UART where the default read and write mode is BLOCKING */
    UART2_Params_init(&uartParams);
    uartParams.baudRate = 115200;

    uart = UART2_open(CONFIG_UART2_0, &uartParams);

    if (uart == NULL)
    {
        /* UART2_open() failed */
        while (1) {}
    }
}

// Make sure you call initUART() before calling this function.
void initI2C(void)
{
    int8_t i;
    int8_t found;
    I2C_Params  i2cParams;

    DISPLAY(snprintf(output, 64, "Initializing I2C Driver - "))

    // Init the driver
    I2C_init();

    // Configure the driver
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;

    // Open the driver
    i2c = I2C_open(CONFIG_I2C_0, &i2cParams);
    if (i2c == NULL)
    {
        DISPLAY(snprintf(output, 64, "Failed\n\r"))
        while (1);
    }

    DISPLAY(snprintf(output, 32, "Passed\n\r"))

    // Boards were shipped with different sensors.
    // Welcome to the world of embedded systems.
    // Try to determine which sensor we have.
    // Scan through the possible sensor addresses

    /* Common I2C transaction setup */
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;

    found = false;
    for (i=0; i<3; ++i)
    {
        i2cTransaction.targetAddress = sensors[i].address;
        txBuffer[0] = sensors[i].resultReg;

        DISPLAY(snprintf(output, 64, "Is this %s? ", sensors[i].id))
        if (I2C_transfer(i2c, &i2cTransaction))
        {
            DISPLAY(snprintf(output, 64, "Found\n\r"))
            found = true;
            break;
        }
        DISPLAY(snprintf(output, 64, "No\n\r"))
    }

    if(found)
    {
        DISPLAY(snprintf(output, 64, "Detected TMP%s I2C address: %x\n\r", sensors[i].id, i2cTransaction.targetAddress))
    }
    else
    {
        DISPLAY(snprintf(output, 64, "Temperature sensor not found, contact professor\n\r"))
    }
}


// read ambient temperature from sensor
int16_t readTemp(void)
{
    int j;
    int16_t temperature = 0;

    i2cTransaction.readCount = 2;
    if (I2C_transfer(i2c, &i2cTransaction))
    {
        /*
         * Extract degrees C from the received data;
         * see TMP sensor datasheet
         */
        temperature = (rxBuffer[0] << 8) | (rxBuffer[1]);
        temperature *= 0.0078125;

        /*
         * If the MSB is set '1', then we have a 2's complement
         * negative value which needs to be sign extended
         */
        if (rxBuffer[0] & 0x80)
        {
            temperature |= 0xF000;
        }
    }
    else
    {
        DISPLAY(snprintf(output, 64, "Error reading temperature sensor (%d)\n\r",i2cTransaction.status))
        DISPLAY(snprintf(output, 64, "Please power cycle your board by unplugging USB and plugging back in.\n\r"))
    }

    return temperature;
 }


// write the current state variables to the UART
void sendStateToUART()
{
    DISPLAY(snprintf(output, 64, "<%02d, %02d, %d, %04d>\n\r", ambient_temp, set_point, currentState, seconds));

}

// encapsulation of call to Timer_setPeriod and all ancillary operations
// Note that this is the same encapsulation of timer code that I provided in module five
void setTimerPeriod(int DURATION)
{

    Timer_stop(timer0);
    timerFlag=0;
    Timer_setPeriod(timer0,Timer_PERIOD_US,DURATION);
    Timer_start(timer0);
    while(timerFlag==0){}
}

/* timer callback
 *
 */
void timerCallback(Timer_Handle myHandle, int_fast16_t status)
{
    timerFlag=1;
}

// set temp state ON
void setTempStateON(void)
{
    currentState = STATE_ON;
    updateStateDisplay();
}

// set temp state OFF
void setTempStateOFF(void)
{
    currentState = STATE_OFF;
    updateStateDisplay();
}

// show red LED ON to inddicate heat is on
void showTempStateON() {
    GPIO_write(CONFIG_GPIO_RED_LED, 1);  // Turn on RED LED
}

// show red LED OFF to inddicate heat is off
void showTempStateOFF() {
    GPIO_write(CONFIG_GPIO_RED_LED, 0);  // Turn on RED LED
}

// update LED state
void updateStateDisplay(void)
{
    if (currentState == STATE_ON) {
        showTempStateON();
    } else {
        showTempStateOFF();
    }
}

void initTimer(void)
{
    Timer_Params params;

    Timer_init();
    Timer_Params_init(&params);
    params.period = 5000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;

    timer0 = Timer_open(CONFIG_TIMER_0, &params);

    if (timer0 == NULL)
    {
        /* Failed to initialize timer */
        while (1) {}
    }

    if (Timer_start(timer0) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }
}
/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_0.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn0(uint_least8_t index)
{
    button0Flag = 1;
}

/*
 *  ======== gpioButtonFxn1 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_1.
 *  This may not be used for all boards.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn1(uint_least8_t index)
{
    button1Flag = 1;
}

void checkSetPointButtons()
{

    /*
       Here we have the followoing tasks to code:
       1. check both button flags (button0Flag, button1Flag)
       2. If button0Flag == 1 then DECREASE the set_point variable value by 1 (degree celsius)
          then, DISPLAY(snprintf(output, 64, "set_point decreased to: %02d\n\r", set_point));
       3. If button1Flag == 1 then INCREASE the set_point variable value by 1 (degree celsius)
          then, DISPLAY(snprintf(output, 64, "set_point increased to: %02d\n\r", set_point));
       4. set both button flags (button0Flag, button1Flag) back to 0

     */
    if(button0Flag == 1){
        set_point--;
        DISPLAY(snprintf(output, 64, "set_point decreased to: %02d\n\r", set_point));
    }
    else if(button1Flag == 1){
        set_point++;
        DISPLAY(snprintf(output, 64, "set_point increased to: %02d\n\r", set_point));
    }

    button0Flag = 0;
    button1Flag = 0;

}
// compare ambient_temp to set point and adjust state accordingly
void compareAmbientToDesiredTemp()
{
    /*
        Here we have the following tasks to code:
        1. call the provided readTemp() method and assign the return value to the ambient_temp variable
        2. compare the ambient_temp value to the set_point (desired temperature) value
        3. If the ambient_temp value is less than the set_point value, then use the provided atomic method to setTempStateON
        4. otherwise, use the provided method to setTempStateOFF
    */
    ambient_temp = readTemp();
    if(ambient_temp < set_point){
        setTempStateON();
    }
    else{
        setTempStateOFF();
    }

}
// Function to initialize the system.
// Moving these operations out of the main thread keeps the task scheduler more readable
void init_system() {

    // call driver init functions
    GPIO_init();

    // very important code block that will turn on your temperature sensor
 #ifdef CONFIG_GPIO_TMP_EN
     GPIO_setConfig(CONFIG_GPIO_TMP_EN, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
     /* Allow the sensor to power on */
     sleep(1);
 #endif

    // Note that the rest of this method is the same as in our GPIO morse code project

    // Set LED pins as output
     //GPIO_setConfig(CONFIG_GPIO_GREEN_LED, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
     GPIO_setConfig(CONFIG_GPIO_RED_LED, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
     GPIO_setConfig(CONFIG_GPIO_YELLOW_LED, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);


    // configure the user button pins
    GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_INT_FALLING | GPIO_CFG_INPUT);
    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);

    /*  If more than one input pin is available for your device, interrupts
    *  will be enabled on CONFIG_GPIO_BUTTON1.
    */
   if (CONFIG_GPIO_BUTTON_0 != CONFIG_GPIO_BUTTON_1) {
       /* Configure BUTTON1 pin */
       GPIO_setConfig(CONFIG_GPIO_BUTTON_1, GPIO_CFG_IN_INT_FALLING | GPIO_CFG_INPUT);

       /* Install Button callback */
       GPIO_setCallback(CONFIG_GPIO_BUTTON_1, gpioButtonFxn1);
       GPIO_enableInt(CONFIG_GPIO_BUTTON_1);
   }

   /* Call remaining driver init functions */
   initUART();
   initI2C();

}

/*
 *  ======== mainThread ========
    The main thread will drive our task scheduler.
    Therefore, keep it light and readable
 */
void *mainThread(void *arg0)
{
    init_system(); // Initialize system

    initTimer();

    setTempStateOFF(); // initial state of heat

    DISPLAY(snprintf(output, 64, "Starting Task Scheduler\n\r"));

    while (1)
    {

        /*
         This infinite loop contains our TASK SCHEDULER.

         We have the following tasks to code:
         1. every BUTTON_CHECK_INTERVAL (200 milliseconds) call a method to checkSetPointButtons
         2. every TEMP_CHECK_INTERVAL (300 milliseconds) call a method to compareAmbientToDesiredTemp
            NOTE: the method you call in step two should first call readTemp
            NOTE: the specification says to check the temperature every 500 milliseconds,
                  we achieve this here because sum(BUTTON_CHECK_INTERVAL, TEMP_CHECK_INTERVAL) == 500 milliseconds
         3. every REPORT_INTERVAL (500 milliseconds) call a method to sendStateToUART
         4. increment seconds by 1
            NOTE: it you sum BUTTON_CHECK_INTERVAL, TEMP_CHECK_INTERVAL, and REPORT_INTERVAL it takes 1 second
                  to get to the bottom of the loop and increment seconds

         NOTE: use setTimerPeriod to achieve each of your INTERVALS

         */

        // every 200 ms check the set_point control buttons

        // every 500 ms read temp and update LEDs

        // every second output the state to UART

        setTimerPeriod(BUTTON_CHECK_INTERVAL);
        checkSetPointButtons();

        setTimerPeriod(TEMP_CHECK_INTERVAL);
        compareAmbientToDesiredTemp();

        setTimerPeriod(REPORT_INTERVAL);
        sendStateToUART();



        seconds++;

    }

}
