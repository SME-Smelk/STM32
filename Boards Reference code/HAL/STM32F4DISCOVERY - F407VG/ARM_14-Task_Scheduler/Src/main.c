/**
  ******************************************************************************
  * @Project        : ARM_14-Task_Scheduler
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 03-01-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Example of a scratch OS for MCU, with tasks and scheduler.
  * @Lib			: -
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf: none
  * @note:
  *
  *		1-Enable faults processor
  *		2-Initialize the stack of the scheduler
  *		3-Initialize the stack of the taks
  *		4-Initialize all users leds
  *		5-Initialize the systick
  *		6-Switch to PSP
  *		7-Execute the task1. The OS will run.
  *
  *		In running state:
  *
  *		*SysTick_Handler (Each 1ms)
  *			1-update_global_tick_count();
  *			2-unblock_tasks
  *			3-pend the PenSV interrupt
  *				->__attribute__((naked)) void PendSV_Handler(void)
  *					Realize the context switch
  *						Task1,2,3,4.
  *
  *		Also:
  *
  *		*void task_delay(uint32_t tick_count)
  *			->void schedule(void)
  *				->__attribute__((naked)) void PendSV_Handler(void)
  *					Block task
  *					Realize the context switch
  *
  ******************************************************************************
**/

#include<stdio.h>
#include"main.h"
#include "led.h"

/*Task Handlers*/
void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

//tarea de espera
void task_delay(uint32_t tick_count);

/*Scheduler functions*/
void init_systick_timer(uint32_t tick_hz);
__attribute__((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack);

/*Function that enable the task stack memory */
void init_tasks_stack(void);

/*Function for enable the processors faults*/
void enable_processor_faults(void);

/*Function for change SP a PSP*/
__attribute__((naked)) void switch_sp_to_psp(void);

/*For fet the PSP*/
uint32_t get_psp_value(void);

/*Variable for selects tasks */
uint8_t current_task = 1; //task1 is running

/* This variable gets updated from systick handler for every systick interrupt */
uint32_t g_tick_count = 0;

/* This is a task control block carries private information of each task */
typedef struct
{
	uint32_t psp_value;
	uint32_t block_count;
	uint8_t  current_state;
	void (*task_handler)(void);
}TCB_t;

/* Each task has its own TCB */
TCB_t user_tasks[MAX_TASKS];

int main(void)
{

	enable_processor_faults();

	init_scheduler_stack(SCHED_STACK_START);

	init_tasks_stack();

	led_init_all();

	init_systick_timer(TICK_HZ);

	switch_sp_to_psp();

	task1_handler();

	for(;;);

}


void idle_task(void)
{
	while(1);
}


void task1_handler(void)
{
	while(1)
	{
		led_on(LED_GREEN);
		task_delay(1000);
		led_off(LED_GREEN);
		task_delay(1000);
	}

}

void task2_handler(void)
{
	while(1)
	{
		led_on(LED_ORANGE);
		task_delay(500);
		led_off(LED_ORANGE);
		task_delay(500);
	}

}

void task3_handler(void)
{
	while(1)
	{
		led_on(LED_BLUE);
		task_delay(250);
		led_off(LED_BLUE);
		task_delay(250);
	}

}

void task4_handler(void)

{
	while(1)
	{
		led_on(LED_RED);
		task_delay(125);
		led_off(LED_RED);
		task_delay(125);
	}


}


void init_systick_timer(uint32_t tick_hz)
{
	uint32_t *pSRVR = (uint32_t*)0xE000E014;
	uint32_t *pSCSR = (uint32_t*)0xE000E010;

    /* calculation of reload value */
    uint32_t count_value = (SYSTICK_TIM_CLK/tick_hz)-1;

    //Clear the value of SVR
    *pSRVR &= ~(0x00FFFFFFFF);

    //load the value in to SVR
    *pSRVR |= count_value;

    //Enable the exception request
    *pSCSR |= ( 1 << 1); //Enables SysTick exception request:
    /*Indicates the clock source, processor clock source*/
    *pSCSR |= ( 1 << 2);

    //Enable the systick, the counter.
    *pSCSR |= ( 1 << 0);

}


__attribute__((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack)
{
	 /*Store in MSP the stack of scheduler*/
     __asm volatile("MSR MSP,%0": :  "r" (sched_top_of_stack)  :   );
     /**/
     __asm volatile("BX LR");

}

/* this function stores dummy stack contents for each task */
void init_tasks_stack(void)
{

	/*Set state ready for all tasks*/
	user_tasks[0].current_state = TASK_READY_STATE;
	user_tasks[1].current_state = TASK_READY_STATE;
	user_tasks[2].current_state = TASK_READY_STATE;
	user_tasks[3].current_state = TASK_READY_STATE;
	user_tasks[4].current_state = TASK_READY_STATE;

	/*Set the start stack address for each task*/
	user_tasks[0].psp_value = IDLE_STACK_START;
	user_tasks[1].psp_value = T1_STACK_START;
	user_tasks[2].psp_value = T2_STACK_START;
	user_tasks[3].psp_value = T3_STACK_START;
	user_tasks[4].psp_value = T4_STACK_START;

	/*Set the handler of tasks*/
	user_tasks[0].task_handler = idle_task;
	user_tasks[1].task_handler = task1_handler;
	user_tasks[2].task_handler = task2_handler;
	user_tasks[3].task_handler = task3_handler;
	user_tasks[4].task_handler = task4_handler;

	/*Use the Stack for each task for set and configure the previous parameters*/
	uint32_t *pPSP;

	for(int i = 0 ; i < MAX_TASKS ;i++)
	{
		/*Set pointer for write and initialize the stack for each task*/
		pPSP = (uint32_t*) user_tasks[i].psp_value;

		/*xprs 0x01000000*/
		pPSP--;
		*pPSP = DUMMY_XPSR;

		/* Set PC */
		pPSP--;
		*pPSP = (uint32_t) user_tasks[i].task_handler;

		/* Set LR */
		pPSP--;
		*pPSP = 0xFFFFFFFD;

		/* For general register Rx*/
		for(int j = 0 ; j < 13 ; j++)
		{
			pPSP--;
		    *pPSP = 0;

		}
		/*Set psp value*/
		user_tasks[i].psp_value = (uint32_t)pPSP;
	}
}

void enable_processor_faults(void)
{
	uint32_t *pSHCSR = (uint32_t*)0xE000ED24;

	/*mem manage*/
	*pSHCSR |= ( 1 << 16);
	/*bus fault*/
	*pSHCSR |= ( 1 << 17);
	/*usage fault*/
	*pSHCSR |= ( 1 << 18);
}


uint32_t get_psp_value(void)
{
	/*Read field psp_value of task struct */
	return user_tasks[current_task].psp_value;
}


void save_psp_value(uint32_t current_psp_value)
{
	/*Save the argument like a new psp_value*/
	user_tasks[current_task].psp_value = current_psp_value;
}


void update_next_task(void)
{
	int state = TASK_BLOCKED_STATE;

	/*Find the next task in READY to execute.*/
	for(int i= 0 ; i < (MAX_TASKS) ; i++)
	{
		current_task++;
	    current_task %= MAX_TASKS;
		state = user_tasks[current_task].current_state;
		if( (state == TASK_READY_STATE) && (current_task != 0) )
			break;
	}

	/*If tasks are not available. Go to idle task (current_task 0)*/
	if(state != TASK_READY_STATE)
		current_task = 0;
}




__attribute__((naked)) void switch_sp_to_psp(void)
{
    /*1. initialize the PSP with TASK1 stack start address*/

	/*get the value of psp of current_task*/
	/*preserve LR which connects back to main()*/
	__asm volatile ("PUSH {LR}");
	__asm volatile ("BL get_psp_value");
	/*initialize psp*/
	__asm volatile ("MSR PSP,R0");
	/*pops back LR value*/
	__asm volatile ("POP {LR}");

	/*2. change SP to PSP using CONTROL register*/
	__asm volatile ("MOV R0,#0X02");
	__asm volatile ("MSR CONTROL,R0");
	__asm volatile ("BX LR");
}


void schedule(void)
{
	/*It is called via task_delay to realize the change of state of the task in the stack frame*/
	/*pend the pendsv exception*/
	uint32_t *pICSR = (uint32_t*)0xE000ED04;
	*pICSR |= ( 1 << 28);

}


void task_delay(uint32_t tick_count)
{

	/* Delay not blocking*/

	/*disable interrupt*/
	INTERRUPT_DISABLE();

	if(current_task)
	{
	   /*Save the value of the future tick count in block_count to be
	    * discriminated by unblock_tasks in systick_handler
	    * One time for task.
	    * */
	   user_tasks[current_task].block_count = g_tick_count + tick_count;
	   user_tasks[current_task].current_state = TASK_BLOCKED_STATE;

	   /*Task blocked, to the next task...*/
	   /*Realize the context switching*/
	   schedule();
	}

	/*enable interrupt*/
	INTERRUPT_ENABLE();
}


__attribute__((naked)) void PendSV_Handler(void)
{
	/*Save the context of current task */
	/*1. Get current running task's PSP value*/
	__asm volatile("MRS R0,PSP");
	/*2. Using that PSP value store SF2( R4 to R11)*/
	__asm volatile("STMDB R0!,{R4-R11}");

	__asm volatile("PUSH {LR}");

	/*3. Save the current value of PSP*/
    __asm volatile("BL save_psp_value");


	/*Retrieve the context of next task */
	/*1. Decide next task to run*/
    __asm volatile("BL update_next_task");

	/*2. get its past PSP value*/
	__asm volatile ("BL get_psp_value");

	/*3. Using that PSP value retrieve SF2(R4 to R11)*/
	__asm volatile ("LDMIA R0!,{R4-R11}");

	/*4. update PSP and exit*/
	__asm volatile("MSR PSP,R0");

	__asm volatile("POP {LR}");

	__asm volatile("BX LR");
}



void update_global_tick_count(void)
{
	g_tick_count++;
}

void unblock_tasks(void)
{
	for(int i = 1 ; i < MAX_TASKS ; i++)
	{
		if(user_tasks[i].current_state != TASK_READY_STATE)
		{
			if(user_tasks[i].block_count == g_tick_count)
			{
				user_tasks[i].current_state = TASK_READY_STATE;
			}
		}

	}

}


void  SysTick_Handler(void)
{

	uint32_t *pICSR = (uint32_t*)0xE000ED04;

    update_global_tick_count();

    unblock_tasks();

    //pend the pendsv exception
    *pICSR |= ( 1 << 28);
}


/*2. implement the fault handlers*/

void HardFault_Handler(void)
{
	printf("Exception : Hardfault\n");
	while(1);
}


void MemManage_Handler(void)
{
	printf("Exception : MemManage\n");
	while(1);
}

void BusFault_Handler(void)
{
	printf("Exception : BusFault\n");
	while(1);
}

void UsageFault_Handler(void)
{
	printf("Exception : UsageFault\n");
	while(1);
}


