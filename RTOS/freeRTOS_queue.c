
#include <avr/io.h>
#include "mHEADERS.h"

#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/FreeRTOSConfig.h"
#include "FreeRTOS/include/task.h"
#include "FreeRTOS/include/queue.h"

// all FreeRTOS settings can be managed from "FreeRTOSConfig.h" file

/* steps to start:
 * 1) create a handle: TaskHandle_t TaskHandler1 = NULL;
 * 2) create a task: xTaskCreate(TaskFunction, "TaskName", stackSize, (void*) &parameter, priority, &TaskHandle);
 *      you should have already created the function and/or the parameters you need in the task
 * 3) start scheduler: vTaskStartScheduler();
 * 
 * all this should be done in the main function before the while(1)
 * 4) at the end of the task function make sure to include: vTaskDelete(NULL); 
 * to delete the task after the loop is done
 */

void TaskFUNC1(void* para);
void TaskFUNC2(void* para);
void TaskFUNC3(void* para);

//handler to delete task (it acts as task pointer)
//A task handle, used later to delete or control the task.

TaskHandle_t TaskHandler1 = NULL;
TaskHandle_t TaskHandler2 = NULL;
TaskHandle_t TaskHandler3 = NULL;

xQueueHandle QHandle = NULL;

//we can send multiple data to the queue by sending them as a struct
typedef struct{
    int data;
    const char* task;
}xdata;

//sending ADC reading by interrupt
ISR (ADC_vect){
    //create struct to hold data
    xdata mydata;
    
    //init data
    mydata.data = ADC_read();
    mydata.task = "ADC from ISR: ";
    
    // Variable to determine if a context switch is required
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // Send data packet using the ISR-safe function
    xQueueSendToFrontFromISR(QHandle, (void*) &mydata, &xHigherPriorityTaskWoken);
    
    // Perform a context switch if a higher-priority task was woken
    if (xHigherPriorityTaskWoken) {
        taskYIELD(); // Yield to a higher-priority task
    }
}


int main() {
    initLED();
    initLCD4();
    
    initADC(CH0, AREF, ADC_freq_128);
    initUART(9600, TRANSMITTER, ASYNCH);
    
    ///////////////// --------- queue ----------///////////////////
    
    //xQueueCreate(QUEUE_LENGTH, ITEM_SIZE)
    QHandle = xQueueCreate(5, sizeof(xdata));
    
    /*---TO SEND:
     * xQueueSendToFront (xQueue, (void*) &value,  (TickType_t) 10);  // Wait 10 ticks if the queue is full
     * 
     *--- TO SEND FROM ISR:
     * // Variable to hold whether a context switch is needed
     * BaseType_t xHigherPriorityTaskWoken = pdFALSE;
     * 
     * xQueueSendToFrontFromISR(QHandle,(void*) &mydata, &xHigherPriorityTaskWoken);
     * 
     * // Perform a context switch if needed
     * if (xHigherPriorityTaskWoken)
            taskYIELD(); // Yield to a higher-priority task
     * 
     * //A context switch is the process of saving the state of a currently running task and 
     * restoring the state of another task, allowing multiple tasks to share the CPU.
     * 
     *---TO RECEIVE:
     * xQueueReceive(xQueue, (void*) &receivedValue, (TickType_t) 10);  // Wait 10 ticks if the queue is empty
     * 
     * the timeout is the maximum period that a task will wait for an operation
     * (such as sending or receiving data from a queue) to succeed.
     */
   
    ///////////////// --------- queue ----------///////////////////
    
    
    ///////////////// --------- tasks ----------///////////////////
    //create a TASK
    xTaskCreate(//task function
                TaskFUNC1,
            
                //task name 
                "Task1",  
            
                //allowed stack size
                100,
            
                //POINTER to parameter 
                NULL,
            
                //priority (where higher numbers indicate higher priority).
                1, 
            
                //ADDRESS to handler to delete/control task
                &TaskHandler1); 

    xTaskCreate(
            //task function
            TaskFUNC2,
            
            //task name 
            "Task2",  
            
            //allowed stack size
            100, 
            
            //address of parameter casted as (void*)
            //if it is already a pointer we don't need to send it address
            //if state1 wasn't a pointer: (void*) &state1
            NULL, 
            
            //priority
            1, 
            
            //address to handler to delete task
            &TaskHandler2); 
    
    
    xTaskCreate(
            //task function
            TaskFUNC3,
            
            //task name 
            "Task3",  
            
            //allowed stack size
            100, 
            
            //address of parameter casted as (void*)
            NULL, 
            
            //priority
            1, 
            
            //address to handler to delete task
            &TaskHandler3);
    
    ///////////////// --------- tasks ----------///////////////////
    
    //Starts the FreeRTOS scheduler, allowing tasks to run according to their priorities.
    vTaskStartScheduler();
    
    //IF WE WANT TO USE THE ISR 
//    ADC_int_en();
//    sei();
    
    while (1) {
        
    }
    return 0;
}


///////////////// --------- tasks functions ----------///////////////////
void TaskFUNC1(void* para){
    //function that reads the ADC "channel 0" and sends data to the queue
    xdata mydata;
   
    while(1){
        // Delay to allow other tasks to run
        vTaskDelay(1);
        
        // Select ADC channel and start conversion
        ADC_select_ch(CH0);
        ADC_startConv(); 
        
        // Wait for ADC conversion to complete
        ADC_Flag;
        
        mydata.data = ADC_read();
        mydata.task = "TASK1: ";
        
        // Send data to the queue with a timeout
        if (xQueueSendToFront(QHandle, (void*) &mydata, (TickType_t) 2) != pdPASS) {
            // Handle the case where the queue is full
        }
        
        // Yield to allow other tasks to run
        taskYIELD();
        
    }
   
}

void TaskFUNC2(void* para){
    //a function that reads the ADC FROM ISR or from func1 and receives data from the queue
    
    //struct to handle received data
    xdata received_data;
     
    while (1){  
        
//        //IF WE WANT TO USE THE ISR TO MAKE THE CONVERSIONS NOT THE FUNCTIONS
//          
//        // Trigger ADC conversion (handled by ISR)
//          ADC_select_ch(CH1);
//          ADC_startConv(); 
         
         
        
        LCD4_CLEAR();
        
        // Wait to receive data from the queue, timeout set to 2 ticks
        if (xQueueReceive(QHandle, (void*) &received_data, (TickType_t)2) == pdPASS) {
            
            // Send data to UART
            UART_sen_str(received_data.task);  // Send task description
            UART_num(received_data.data);      // Send ADC data as number
            UART_send('\r');                   // Carriage return
            
            // Display data on LCD
            LCD4_data_str(received_data.task);  // Display task description on LCD
            LCD4_data_num(received_data.data);  // Display ADC data on LCD
        }
        
        vTaskDelay(5);
    }
    vTaskDelete(NULL);
}

void TaskFUNC3(void* para){
 
    //function that reads the ADC "channel 1" and sends data to the queue
    xdata mydata;
   
    while(1){
        // Delay to allow other tasks to run
        vTaskDelay(1);
        
        // Select ADC channel and start conversion
        ADC_select_ch(CH1);
        ADC_startConv(); 
        
        // Wait for ADC conversion to complete
        ADC_Flag;
        
        mydata.data = ADC_read();
        mydata.task = "TASK3: ";
        
        // Send data to the queue with a timeout
        if (xQueueSendToFront(QHandle, (void*) &mydata, (TickType_t) 2) != pdPASS) {
            // Handle the case where the queue is full
        }
        
        // Yield to allow other tasks to run
        taskYIELD();
    }
}
