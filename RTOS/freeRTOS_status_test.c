
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

char* a = "T1: ";
char* b = "| T2: ";
char* c = "| T3: ";

int main() {
    initLED();
    initLCD4();

    initUART(9600, TRANSMITTER, ASYNCH);

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
            TaskFUNC1,

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


    /*In FreeRTOS (and many other RTOSs), task functions always take a (void*) as their parameter for flexibility. 
     * This allows the same task function to receive ANY type of data when the task is created.
     */


    xTaskCreate(
            //task function
            TaskFUNC1,

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
    ///////////////// --------- tasks ----------///////////////////

    //Starts the FreeRTOS scheduler, allowing tasks to run according to their priorities.
    vTaskStartScheduler();


    while (1) {

    }
    return 0;
}


//////////// ------ vTaskGetInfo ------////////////
/*
    // Use the handle to obtain further information about the task. 
    vTaskGetInfo( // The handle of the task being queried. 
                  xHandle,

                  // A pointer to a TaskStatus_t structure where the information will be stored. 
                  &status,

                  // Indicates that the high water mark value should be included in the structure.
                  pdTRUE,

                  // Include the task STATE in the TaskStatus_t structure. 
                   Specifies that the state should not be filtered 
                   the actual state enumeration should be provided.
                  eInvalid );
 */

/*  The stack high water mark in FreeRTOS refers to the least amount of stack space that has been free
 *  since the task started running

 * It provides an indication of how much stack space is "left over" for a task at its most stack-intensive point.
 * This can be helpful for debugging and optimizing stack usage.
 */

//////////// ------ vTaskGetInfo ------////////////


///////////////// --------- tasks functions ----------///////////////////

/*this functions shows the status of the tasks where:
 * 0: running   // The task is currently running (only one task can be in this state at a time).
 * 1: ready     // The task is ready to run but is not currently running (it is in the ready list).
 * 2: blocked   // The task is currently blocked (waiting for an event or time to pass).
 * 3: suspended // The task is in a suspended state (not available for scheduling, typically vTaskSuspend() was called).
 * 4: deleted   // The task has been deleted (not available for scheduling, typically vTaskDelete() was called).
 * 5: eInvalid  // The task state is invalid (the task does not exist or the handle is incorrect).   
 */

void TaskFUNC1(void* para) {

    //init status to hold the status information of three different tasks 
    TaskStatus_t stat1, stat2, stat3;

    while (1) {

        //task one status (running/ blocked/ suspended)
        vTaskGetTaskInfo(TaskHandler1, &stat1, pdTRUE, eInvalid);

        //task 2
        vTaskGetTaskInfo(TaskHandler2, &stat2, pdTRUE, eInvalid);

        //task 3
        vTaskGetTaskInfo(TaskHandler3, &stat3, pdTRUE, eInvalid);


        vTaskDelay(2);
        UART_sen_str(a);
        UART_num(stat1.eCurrentState);

        UART_sen_str(b);
        UART_num(stat2.eCurrentState);

        UART_sen_str(c);
        UART_num(stat3.eCurrentState);
        UART_send('\r');

    }


    //////////-----another way to get task handles-----////////////

    //    /* Obtain the handle of a task from its name.
    //      if i want to get the handle name from the task's name */
    //    TaskHandle_t xHandle;
    //    xHandle = xTaskGetHandle("Task1");
    //
    //    /* Check the handle is not NULL. */
    //    configASSERT( xHandle );

    //////////-----another way to get task handles-----////////////
}

void TaskFUNC2(void* para) {

    while (1) {

    }
    vTaskDelete(NULL);

}

void TaskFUNC3(void* para) {

    while (1) {

    }
    vTaskDelete(NULL);
}

