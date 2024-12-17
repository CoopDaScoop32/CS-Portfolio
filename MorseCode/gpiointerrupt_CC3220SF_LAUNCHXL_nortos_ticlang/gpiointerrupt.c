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

/* Driver Header files */
#include <ti/drivers/GPIO.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include <string.h>
#include <ti/drivers/UART2.h>


#include <ti/drivers/Timer.h>


typedef enum{
    STATE_OK,
    STATE_SOS
};

const int DOT_TIME = 500000;
const int DASH_TIME = 1500000;
const int CHAR_GAP = 150000;
const int WORD_GAP = 3500000;


volatile currentState = STATE_OK;
volatile int timerFlag = 0;
volatile int button0Flag = 0;
volatile int button1Flag = 0;

Timer_Handle timer0;

void toggleState(void);
void blinkMorse(void);
void blink_dot(void);
void blink_dash(void);
void setTimerPeriod(int);



void blinkMorse(){
    if(currentState == STATE_SOS){
        //S
        for(int i = 0; i<3; i++){
            blink_dot();
        }
        //O
        for(int i = 0; i<3; i++){
            blink_dash();
        }
        //S
        for(int i = 0; i<3; i++){
            blink_dot();
        }
        setTimerPeriod(WORD_GAP);

    }
    else if(currentState == STATE_OK){
        //O
        for(int i = 0; i<3; i++){
            blink_dash();
        }
        //K
        blink_dash();
        blink_dot();
        blink_dash();
        setTimerPeriod(WORD_GAP);

    }
}

void blink_dot(){
    GPIO_write(CONFIG_GPIO_LED_0, 1);
    setTimerPeriod(DOT_TIME);

    GPIO_write(CONFIG_GPIO_LED_0, 0);
    setTimerPeriod(CHAR_GAP);
}

void blink_dash(){
    GPIO_write(CONFIG_GPIO_LED_1, 1);
    setTimerPeriod(DASH_TIME);

    GPIO_write(CONFIG_GPIO_LED_1, 0);
    setTimerPeriod(CHAR_GAP);
}

void setTimerPeriod(int DURATION){
    Timer_stop(timer0);//Resets current timer?
    timerFlag=0;//Signifies timer has started
    Timer_setPeriod(timer0, Timer_PERIOD_US, DURATION);//configures the timer
    Timer_start(timer0);
    while(timerFlag==0){}

}

void timerCallback(Timer_Handle myHandle, int_fast16_t status)//called when timer finishes
{
    timerFlag = 1; //Timer has stopped
}


void toggleState(void){
    if(currentState == STATE_SOS){
        currentState = STATE_OK;
    }
    else{
        currentState = STATE_SOS;
    }
}

void initTimer(void)
{

    Timer_Params params;
    Timer_init();
    Timer_Params_init(&params);
    params.period = 500000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;
    timer0 = Timer_open(CONFIG_TIMER_0, &params);
    if (timer0 == NULL) {
    /* Failed to initialized timer */
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
    /* Toggle an LED */
    //GPIO_toggle(CONFIG_GPIO_LED_0);
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
    /* Toggle an LED */
    //GPIO_toggle(CONFIG_GPIO_LED_1);
    button1Flag = 1;
}

/*
 *  ======== mainThread ========
 */
void init_system(){
    /* Call driver init functions */
    GPIO_init();
    currentState = STATE_SOS;

    /* Configure the LED and button pins */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /* Install Button callback */
    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);

    /* Enable interrupts */
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);

    /*
     *  If more than one input pin is available for your device, interrupts
     *  will be enabled on CONFIG_GPIO_BUTTON1.
     */
    if (CONFIG_GPIO_BUTTON_0 != CONFIG_GPIO_BUTTON_1)
    {
        /* Configure BUTTON1 pin */
        GPIO_setConfig(CONFIG_GPIO_BUTTON_1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

        /* Install Button callback */
        GPIO_setCallback(CONFIG_GPIO_BUTTON_1, gpioButtonFxn1);
        GPIO_enableInt(CONFIG_GPIO_BUTTON_1);
    }
}

void *mainThread(void *arg0)
{
    init_system();
    initTimer();

    while(1){
        if(button0Flag==1||button1Flag==1){
            toggleState();
            button0Flag = 0;
            button1Flag = 0;
        }
        blinkMorse();
    }

    return (NULL);
}
