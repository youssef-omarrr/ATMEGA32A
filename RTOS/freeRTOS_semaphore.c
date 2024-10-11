
#include <avr/io.h>
#include "mHEADERS.h"

#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/FreeRTOSConfig.h"
#include "FreeRTOS/include/task.h"
#include "FreeRTOS/include/queue.h"
#include "FreeRTOS/include/semphr.h"

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

SemaphoreHandle_t semph_handle;
int x = 0;

int main() {
    initLED();
    initLCD4();

    initUART(9600, TRANSMITTER, ASYNCH);


    /* when you create a binary semaphore with xSemaphoreCreateBinary(),
     *  it starts in a "taken" state (unavailable), 
     * meaning the tasks can't take it until it has been given at least once.
     */

    // Create the binary semaphore
    semph_handle = xSemaphoreCreateBinary();

    // Give the semaphore so that the first task can take it
    xSemaphoreGive(semph_handle);

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
    ///////////////// --------- tasks ----------//////////////////

    //Starts the FreeRTOS scheduler, allowing tasks to run according to their priorities.
    vTaskStartScheduler();


    while (1) {

    }
    return 0;
}

///////////////// --------- tasks functions ----------///////////////////

void TaskFUNC1(void* para) {

    vTaskDelay(10);
    while (1) {
        /* See if we can obtain the semaphore. If the semaphore is not
           available wait 10 ticks to see if it becomes free. */
        if (xSemaphoreTake(semph_handle, (TickType_t) 10) == pdTRUE) {
            /* We were able to obtain the semaphore and can now access the
               shared resource. */

            UART_sen_str("TASK 1: ");
            UART_num(x++);
            UART_send('\r');

            /* We have finished accessing the shared resource. Release the
               semaphore. */
            xSemaphoreGive(semph_handle);
            taskYIELD(); // Allow other tasks to run
        } else
            UART_sen_str("[TASK1] BUSY...\r");


    }
}

void TaskFUNC2(void* para) {
    vTaskDelay(10);
    while (1) {

        if (xSemaphoreTake(semph_handle, (TickType_t) 10) == pdTRUE) {

            UART_sen_str("TASK 2: ");
            UART_num(x++);
            UART_send('\r');

            xSemaphoreGive(semph_handle);
            taskYIELD(); // Allow other tasks to run
        } else
            UART_sen_str("[TASK2] BUSY...\r");
    }
}

void TaskFUNC3(void* para) {
    vTaskDelay(10);
    while (1) {
        if (xSemaphoreTake(semph_handle, (TickType_t) 10) == pdTRUE) {

            UART_sen_str("TASK 3: ");
            UART_num(x++);
            UART_send('\r');

            xSemaphoreGive(semph_handle);
            taskYIELD(); // Allow other tasks to run
        } else
            UART_sen_str("[TASK3] BUSY...\r");

    }
}

