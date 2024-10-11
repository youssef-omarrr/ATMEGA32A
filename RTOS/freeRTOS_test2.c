
#include <avr/io.h>
#include "mHEADERS.h"

#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/FreeRTOSConfig.h"
#include "FreeRTOS/include/task.h"
#include "FreeRTOS/include/queue.h"
#include "mLCD.h"

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

char* a = "T1: ";
char* b = "| T2: ";
char* c = "| T3: ";

typedef struct {
    int data;
    char* task;
}xdata;

//ISR (ADC_vect){
//      
//    xdata mydata;
//    mydata.data = ADC_read();
//    mydata.task = "ADC from ISR";
//    xQueueSendToFront(QHandle,(void*) &mydata, 2);
//    
//}


int main() {
    initLED();
    initLCD4();
    
    initADC(CH0, AREF, ADC_freq_128);
    initUART(9600, TRANSMITTER, ASYNCH);
    
    //queue
    QHandle = xQueueCreate(5, sizeof(xdata));
    
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
            2, 
            
            //address to handler to delete task
            &TaskHandler2); 
    
    
    /*In FreeRTOS (and many other RTOSs), task functions always take a (void*) as their parameter for flexibility. 
     * This allows the same task function to receive ANY type of data when the task is created.
     */
    
    
    xTaskCreate(
            //task function
            TaskFUNC3,
            
            //task name 
            "Task3",  
            
            //allowed stack size
            100, 
            
            //address of parameter casted as (void*)
            //if it is already a pointer we don't need to send it address
            //if state1 wasn't a pointer: (void*) &state1
            NULL, 
            
            //priority
            1, 
            
            //address to handler to delete task
            &TaskHandler3);
    
    
    //Starts the FreeRTOS scheduler, allowing tasks to run according to their priorities.
    vTaskStartScheduler();
    
    
    
//    ADC_int_en();
//    sei();
    
    while (1) {
        
    }
    return 0;
}

void TaskFUNC1(void* para){
    
    xdata mydata;
   
    while(1){
        vTaskDelay(1);
        ADC_select_ch(CH0);
        ADC_startConv(); 
        
        ADC_Flag;
        
        mydata.data = ADC_read();
        mydata.task = "TASK1";
        
        xQueueSendToFront(QHandle,(void*) &mydata, 2);
        taskYIELD();
        
    }
    
    
    
    
    /* Obtain the handle of a task from its name.
      if i want to get the handle name from the task's name */
    
//    TaskHandle_t xHandle;
//    xHandle = xTaskGetHandle("Task1");
    
    
    /* Check the handle is not NULL. */
//    configASSERT( xHandle );
//    
//    
//    TaskStatus_t stat1, stat2, stat3;
    
    
    /*
        // Use the handle to obtain further information about the task. 
        vTaskGetInfo( // The handle of the task being queried. 
                      xHandle,

                      // The TaskStatus_t structure to complete with information
                         on xTask. 
                      &xTaskDetails,

                      // Include the stack high water mark value in the
                         TaskStatus_t structure. 
                      pdTRUE,

                      // Include the task state in the TaskStatus_t structure. 
                      eInvalid );
       */
    
    /*The stack high water mark in FreeRTOS refers to the minimum amount of stack space 
     * that has remained unused during the lifetime of a task. 
     * It provides an indication of how much stack space is "left over" for a task at its most stack-intensive point.
     */
    
    
    
//    while (1) {
//        
//        //task one status (running/ blocked/ suspended)
//        vTaskGetTaskInfo(TaskHandler1, &stat1, pdTRUE, eInvalid);
//
//        //task 2
//        vTaskGetTaskInfo(TaskHandler2, &stat2, pdTRUE, eInvalid);
//
//        //task 3
//        vTaskGetTaskInfo(TaskHandler3, &stat3, pdTRUE, eInvalid);
//    
//    
//        vTaskDelay(2);
//        UART_sen_str(a);
//        UART_num(stat1.eCurrentState);
//        
//        UART_sen_str(b);
//        UART_num(stat2.eCurrentState);
//        
//        UART_sen_str(c);
//        UART_num(stat3.eCurrentState);
//        UART_send('\r');
//        
//    }
    
   
}
void TaskFUNC2(void* para){
    xdata received_data;
     
    while (1){  
//        ADC_startConv();  //for isr
        LCD4_CLEAR();
        xQueueReceive(QHandle, (void*) &received_data, 2);
        
        UART_sen_str(received_data.task);
        UART_send(' ');
        UART_num(received_data.data);
        UART_send('\r');
        
        LCD4_data_str(received_data.task);
        LCD4_data_num(received_data.data);
        
        vTaskDelay(5);
    }
    vTaskDelete(NULL);
    
}
void TaskFUNC3(void* para){
    
    xdata mydata;
   
    while(1){
        vTaskDelay(1);
        ADC_select_ch(CH1);
        ADC_startConv();  
        ADC_Flag;
        
        mydata.data = ADC_read();
        mydata.task = "TASK3";
        
        xQueueSendToFront(QHandle,(void*) &mydata, 2);
        taskYIELD();
        
    }
    
    
    
//    while (1){
////        UART_send('\r');
////        UART_sen_str("task 3!!");
////        UART_send('\r');
//        
//        LCD4_data_str("y");
//        vTaskSuspend(TaskHandler3);
//    }
}
