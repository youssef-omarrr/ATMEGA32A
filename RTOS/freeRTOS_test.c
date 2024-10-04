
#include <avr/io.h>
#include "mHEADERS.h"

#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/FreeRTOSConfig.h"
#include "FreeRTOS/include/task.h"

// all FreeRTOS settings can be managed from "FreeRTOSConfig.h" file

/* steps to start:
 * 1) create a handle: TaskHandle_t TaskHandler1 = NULL;
 * 2) create a task: xTaskCreate(TaskFunction, "TaskName", stackSize, parameter, priority, &TaskHandle);
 *      you should have already created the function and/or the parameters you need in the task
 * 3) start scheduler: vTaskStartScheduler();
 * 
 * all this should be done in the main function before the while(1)
 * 4) at the end of the task function make sure to include: vTaskDelete(NULL); 
 * to delete the task after the loop is done
 */

/*These are function prototypes for TASKS that FreeRTOS will execute. 
Each takes a void* parameter, which allows passing any data type as input.*/
void TaskFUNC1(void* para);
void TaskFUNC2(void* para);
void TaskFUNC3(void* para);

int main() {
    initLED();
    initLCD4();
    
    char* state1 = "   TASK  A   ";
    char* state2 = "   TASK  B   ";
    
    initUART(9600, TRANSMITTER, ASYNCH);
    
    //handler to delete task (it acts as task pointer)
    //A task handle, used later to delete or control the task.
    TaskHandle_t TaskHandler1 = NULL;
    
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

    TaskHandle_t TaskHandler2 = NULL;
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
            (void *) state1, 
            
            //priority
            1, 
            
            //address to handler to delete task
            &TaskHandler2); 
    
    
    /*In FreeRTOS (and many other RTOSs), task functions always take a (void*) as their parameter for flexibility. 
     * This allows the same task function to receive ANY type of data when the task is created.
     */
    
    
    TaskHandle_t TaskHandler3 = NULL;
    xTaskCreate(TaskFUNC2, //task function
            "Task3",  //task name 
            100, //allowed stack size
            (void *) state2, //pointer to parameter (&(char*) state2 == state2)
            1, //priority
            &TaskHandler3); //address to handler to delete task
    
    
    //Starts the FreeRTOS scheduler, allowing tasks to run according to their priorities.
    vTaskStartScheduler();
    
    while (1) {
        
    }
    return 0;
}

void TaskFUNC1(void* para){
    while (1){
        LED_toggle(LED0);
        vTaskDelay(500);
    }
    vTaskDelete(NULL); //to delete task if we break from the loop
    /*if it is given NULL it deletes the task it is in, 
    else give it the handler of the task you want to delete */
}
void TaskFUNC2(void* para){
    while (1){
    if ((char*) para == "   TASK  A   "){
        UART_sen_str((char*) para);
        vTaskDelay(500);
    }
    else if ((char*) para == "   TASK  B   "){
        UART_sen_str((char*) para);
        vTaskDelay(500);
    }
    }
    vTaskDelete(NULL);
    
}
void TaskFUNC3(void* para){
    while (1){
        //cast para to be char pointer then DE-REFFRENCE (*) to get the data in this pointer
        char data = *((char *) para); 
        /* casting steps:
         * 1) you have para as void pointer (void* )
         * 2) make para a char pointer instead (or any data type you want) (type* )
         * 3) then dereference it using (*) to get data inside the pointer
         */
    }
    vTaskDelete(NULL);
}

/*1. Task Scheduling:
    FreeRTOS works by using a scheduler that manages how tasks are executed. 
    Tasks are individual functions that run concurrently, and the scheduler determines when and for how long each task runs.
    There are two main types of scheduling FreeRTOS uses:

    *Preemptive Scheduling: In preemptive mode, the scheduler switches tasks automatically based on priority. 
    Higher-priority tasks can interrupt lower-priority ones, ensuring time-critical tasks are executed as soon as possible.

    *Cooperative Scheduling: In cooperative mode, tasks have to voluntarily give up control of the CPU 
    (e.g., by calling vTaskDelay() or taskYIELD()), and the scheduler will not interrupt them unless they do so.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
2. Tasks
    A task in FreeRTOS is a function written by the programmer that runs concurrently with other tasks. 
    Each task has its own stack (memory space for variables) and can be assigned different priorities. 
    FreeRTOS manages the execution of tasks based on their priority and whether they are ready to run.

Example of task creation:
 * xTaskCreate(TaskFunction, "TaskName", stackSize, parameter, priority, &TaskHandle);

    TaskFunction: The function to be executed.
    stackSize:    The amount of stack memory the task can use.
    parameter:    The argument passed to the task.
    priority:     Determines the order of task execution. Higher priority tasks preempt lower priority ones.
    TaskHandle:   A handle used to reference the task (for deletion, suspension, etc.).

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
3. Task States
Tasks can be in several different states:

*Running: The task is currently being executed by the CPU.
*Ready:   The task is ready to run, but is waiting for the scheduler to give it CPU time.

*Blocked:   The task is waiting for an event to occur (e.g., waiting for a delay to expire, waiting for a semaphore, etc.).
*Suspended: The task has been suspended (manually by calling vTaskSuspend()) and will not be executed until resumed.
*Deleted:   The task has finished or has been deleted by calling vTaskDelete().

The scheduler manages transitions between these states.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
4. Task Priorities
Tasks in FreeRTOS are assigned a priority level (usually an integer). 
 *The higher the priority number, the more "important" the task is. 
 *The scheduler ensures that higher-priority tasks are executed before lower-priority ones.

*If two tasks have the same priority, FreeRTOS uses time-slicing, 
    where the CPU switches between tasks at regular intervals.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
5. Time Management
FreeRTOS allows tasks to delay themselves for a certain amount of time, freeing up CPU time for other tasks. 
This is typically done using vTaskDelay():

vTaskDelay(1000); // Delay for 1000 ticks (typically 1 tick = 1 ms)
The vTaskDelay() function puts the task into a blocked state for the specified number of ticks. 
After the delay has passed, the task moves to the ready state, and the scheduler will execute it when possible.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 6. Tick Interrupt
FreeRTOS uses a tick interrupt to manage time. The tick is a periodic interrupt generated by a hardware timer (timer1). 
Each time a tick occurs:
    *The scheduler is invoked to determine whether to switch tasks.
    *Delayed tasks are checked to see if their delay has expired.
    *The tick interrupt is configured by the system, 
     and its frequency determines the "resolution" of the time delays (typically 1ms per tick).
 */