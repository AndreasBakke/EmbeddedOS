#include "ee.h"



/***************************************************************************
 *
 * Stack definition for Freescale MPC5674F, PPC e200z7
 *
 **************************************************************************/
    #define STACK_1_SIZE 1024 /* size = 1024 bytes */
    #define STACK_2_SIZE 1024 /* size = 1024 bytes */
    #define STACK_3_SIZE 4096 /* size = 4096 bytes */

    static EE_STACK_T EE_STACK_ATTRIB EE_stack_1[EE_STACK_WLEN(STACK_1_SIZE)];	/* Task 0 (Task1) */
    static EE_STACK_T EE_STACK_ATTRIB EE_stack_2[EE_STACK_WLEN(STACK_2_SIZE)];	/* Task 1 (Task2) */
    static EE_STACK_T EE_STACK_ATTRIB EE_stack_3[EE_STACK_WLEN(STACK_3_SIZE)];	/* irq stack */

    const EE_UREG EE_std_thread_tos[EE_MAX_TASK+1] = {
        0U,	 /* dummy*/
        1U,	 /* Task1*/
        2U 	 /* Task2*/
    };

    struct EE_TOS EE_e200z7_system_tos[EE_E200Z7_SYSTEM_TOS_SIZE] = {
        {0},	/* Task  (dummy) */
        {(EE_ADDR)(&EE_stack_1[EE_STACK_INITP(STACK_1_SIZE)])},	/* Task 0 (Task1) */
        {(EE_ADDR)(&EE_stack_2[EE_STACK_INITP(STACK_2_SIZE)])} 	/* Task 1 (Task2) */
    };

    EE_UREG EE_e200z7_active_tos = 0U; /* dummy */

    /* stack used only by IRQ handlers */
    struct EE_TOS EE_e200z7_IRQ_tos = {
        (EE_ADDR)(&EE_stack_3[EE_STACK_INITP(STACK_3_SIZE)])
    };



/***************************************************************************
 *
 * Kernel ( CPU 0 )
 *
 **************************************************************************/
    /* Definition of task's body */
    DeclareTask(Task1);
    DeclareTask(Task2);

    const EE_THREAD_PTR EE_hal_thread_body[EE_MAX_TASK] = {
        &EE_oo_thread_stub,		 /* thread Task1 */
        &EE_oo_thread_stub 		 /* thread Task2 */

    };

    EE_UINT32 EE_terminate_data[EE_MAX_TASK];

    /* ip of each thread body (ROM) */
    const EE_THREAD_PTR EE_terminate_real_th_body[EE_MAX_TASK] = {
        &FuncTask1,
        &FuncTask2
    };
    /* ready priority */
    const EE_TYPEPRIO EE_th_ready_prio[EE_MAX_TASK] = {
        0x1U,		 /* thread Task1 */
        0x1U 		 /* thread Task2 */
    };

    const EE_TYPEPRIO EE_th_dispatch_prio[EE_MAX_TASK] = {
        0x1U,		 /* thread Task1 */
        0x1U 		 /* thread Task2 */
    };

    /* thread status */
    EE_TYPESTATUS EE_th_status[EE_MAX_TASK] = {
        SUSPENDED,
        SUSPENDED
    };

    /* next thread */
    EE_TID EE_th_next[EE_MAX_TASK] = {
        EE_NIL,
        EE_NIL
    };

    /* The first stacked task */
    EE_TID EE_stkfirst = EE_NIL;

    /* system ceiling */
    EE_TYPEPRIO EE_sys_ceiling= 0x0000U;

    /* The priority queues: (16 priorities maximum!) */
    EE_TYPEPAIR EE_rq_queues_head[EE_RQ_QUEUES_HEAD_SIZE] =
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    EE_TYPEPAIR EE_rq_queues_tail[EE_RQ_QUEUES_TAIL_SIZE] =
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    EE_TYPE_RQ_MASK EE_rq_bitmask = 0U;

    /* remaining nact: init= maximum pending activations of a Task */
    EE_TYPENACT EE_th_rnact[EE_MAX_TASK] = {
        1U,		 /* thread Task1 */
        1U		 /* thread Task2 */
    };

    const EE_TYPENACT EE_th_rnact_max[EE_MAX_TASK] = {
        1U,		 /* thread Task1 */
        1U		 /* thread Task2 */
    };

    EE_TYPEPRIO EE_rq_link[EE_MAX_TASK] =
        { 0U, 0U};

    /* The pairs that are enqueued into the priority queues (2 is the total number of task activations) */
    EE_TYPEPAIR EE_rq_pairs_next[EE_RQ_PAIRS_NEXT_SIZE] =
        { 1, -1};

    /* no need to be initialized */
    EE_TID EE_rq_pairs_tid[EE_RQ_PAIRS_TID_SIZE];

    /* a list of unused pairs */
    EE_TYPEPAIR EE_rq_free = 0; /* pointer to a free pair */

    #ifndef __OO_NO_CHAINTASK__
        /* The next task to be activated after a ChainTask. initvalue=all EE_NIL */
        EE_TID EE_th_terminate_nextask[EE_MAX_TASK] = {
            EE_NIL,
            EE_NIL
        };
    #endif



/***************************************************************************
 *
 * Event handling
 *
 **************************************************************************/
    EE_TYPEEVENTMASK EE_th_event_active[EE_MAX_TASK] =
        { 0U, 0U};    /* thread event already active */

    EE_TYPEEVENTMASK EE_th_event_waitmask[EE_MAX_TASK] =
        { 0U, 0U};    /* thread wait mask */

    EE_TYPEBOOL EE_th_waswaiting[EE_MAX_TASK] =
        { 0U, 0U};

    const EE_TYPEPRIO EE_th_is_extended[EE_MAX_TASK] =
        { 1U, 1U};



/***************************************************************************
 *
 * Mutex
 *
 **************************************************************************/
    /*
     * This is the last mutex that the task has locked. This array
     *    contains one entry for each task. Initvalue= all -1. at runtime,
     *    it points to the first item in the EE_resource_stack data structure.
    */
    EE_UREG EE_th_resource_last[EE_MAX_TASK] =
        { (EE_UREG) -1, (EE_UREG) -1};

    /*
     * This array is used to store a list of resources locked by a
     *    task. there is one entry for each resource, initvalue = -1. the
     *    list of resources locked by a task is ended by -1.
    */
    EE_UREG EE_resource_stack[EE_MAX_RESOURCE] =
        { (EE_UREG) -1, (EE_UREG) -1};

    /*
     * Only in extended status or when using ORTI with resources; for
     *    each resource, a flag is allocated to see if the resource is locked or
     *    not.  Note that this information cannot be easily knew from the previous
     *    two data structures. initvalue=0
    */
    EE_TYPEBOOL EE_resource_locked[EE_MAX_RESOURCE] =
        { 0U, 0U};

    const EE_TYPEPRIO EE_resource_ceiling[EE_MAX_RESOURCE]= {
        0x1U,		/* resource RES_SCHEDULER */
        0x1U 		/* resource countResource */
    };

    EE_TYPEPRIO EE_resource_oldceiling[EE_MAX_RESOURCE];



/***************************************************************************
 *
 * Counters
 *
 **************************************************************************/
    const EE_oo_counter_ROM_type EE_counter_ROM[EE_COUNTER_ROM_SIZE] = {
        {65535U, 1U, 2U}         /* myCounter */
    };

    EE_oo_counter_RAM_type       EE_counter_RAM[EE_MAX_COUNTER] = {
        {0U, (EE_TYPECOUNTEROBJECT)-1}
    };


/***************************************************************************
 *
 * Alarms
 *
 **************************************************************************/
    const EE_oo_alarm_ROM_type EE_alarm_ROM[EE_ALARM_ROM_SIZE] = {
        {0U},
        {1U}
    };


/***************************************************************************
 *
 * Counter Objects
 *
 **************************************************************************/
    const EE_oo_counter_object_ROM_type   EE_oo_counter_object_ROM[EE_COUNTER_OBJECTS_ROM_SIZE] = {

        {myCounter, AlarmTask1, EE_ALARM },
        {myCounter, AlarmTask2, EE_ALARM }
    };

    EE_oo_counter_object_RAM_type EE_oo_counter_object_RAM[EE_COUNTER_OBJECTS_ROM_SIZE];


/***************************************************************************
 *
 * Alarms and Scheduling Tables actions
 *
 **************************************************************************/
    const EE_oo_action_ROM_type   EE_oo_action_ROM[EE_ACTION_ROM_SIZE] = {

        {EE_ACTION_EVENT   , Task1, TimerEvent1, (EE_VOID_CALLBACK)NULL, (EE_TYPECOUNTER)-1 },
        {EE_ACTION_EVENT   , Task2, TimerEvent2, (EE_VOID_CALLBACK)NULL, (EE_TYPECOUNTER)-1 }
    };



/***************************************************************************
 *
 * AppMode
 *
 **************************************************************************/
    EE_TYPEAPPMODE EE_ApplicationMode;


/***************************************************************************
 *
 * Auto Start (TASK)
 *
 **************************************************************************/
    static const EE_TID EE_oo_autostart_task_mode_OSDEFAULTAPPMODE[EE_OO_AUTOSTART_TASK_MODE_OSDEFAULTAPPMODE_SIZE] = 
        { Task1, Task2 };

    const struct EE_oo_autostart_task_type EE_oo_autostart_task_data[EE_MAX_APPMODE] = {
        { 2U, EE_oo_autostart_task_mode_OSDEFAULTAPPMODE}
    };


/***************************************************************************
 *
 * Auto Start (ALARM)
 *
 **************************************************************************/
    static const EE_TYPEALARM EE_oo_autostart_alarm_mode_OSDEFAULTAPPMODE[EE_OO_AUTOSTART_ALARM_MODE_OSDEFAULTAPPMODE_SIZE] =
        { AlarmTask1, AlarmTask2 };

    const struct EE_oo_autostart_alarm_type EE_oo_autostart_alarm_data[EE_MAX_APPMODE] = {
        { 2U, EE_oo_autostart_alarm_mode_OSDEFAULTAPPMODE}
    };


/***************************************************************************
 *
 * Init alarms parameters (ALARM)
 *
 **************************************************************************/

    const EE_TYPETICK EE_oo_autostart_alarm_increment[EE_MAX_ALARM] =
        {1000U, 1050U };

    const EE_TYPETICK EE_oo_autostart_alarm_cycle[EE_MAX_ALARM] =
        {4000U, 4000U };

