#ifndef RTDH_EECFG_H
#define RTDH_EECFG_H


#define RTDRUID_CONFIGURATOR_NUMBER 1278



/***************************************************************************
 *
 * Common defines ( CPU 0 )
 *
 **************************************************************************/

    /* TASK definition */
    #define EE_MAX_TASK 2
    #define Task1 0
    #define Task2 1

    /* MUTEX definition */
    #define EE_MAX_RESOURCE 1U
    #define RES_SCHEDULER 0U

    /* ALARM definition */
    #define EE_MAX_ALARM 2U
    #define AlarmTask1 0U
    #define AlarmTask2 1U

    /* SCHEDULING TABLE definition */
    #define EE_MAX_SCHEDULETABLE 0U

    /* COUNTER OBJECTS definition */
    #define EE_MAX_COUNTER_OBJECTS (EE_MAX_ALARM + EE_MAX_SCHEDULETABLE)

    /* COUNTER definition */
    #define EE_MAX_COUNTER 1U
    #define myCounter 0U

    /* APPMODE definition */
    #define EE_MAX_APPMODE 1U

    /* CPUs */
    #define EE_MAX_CPU 1
    #define EE_CURRENTCPU 0

    /* Number of isr 2 */
    #define EE_MAX_ISR2   0
    #define EE_MAX_ISR_ID 0

#ifndef __DISABLE_EEOPT_DEFINES__


/***************************************************************************
 *
 * User options
 *
 **************************************************************************/
#define __USE_LEDS__
#define __USE_BUTTONS__
#define __E200ZX_EXECUTE_FROM_RAM__
#define EE_ISR_DYNAMIC_TABLE
#define EE_LAUTERBACH_DEMO_SIM


/***************************************************************************
 *
 * Automatic options
 *
 **************************************************************************/
#define __RTD_LINUX__
#define __PPCE200ZX__
#define __MPC5674F__
#define __PPCE200Z7__
#define EE_GNU__
#define __OO_ORTI_LASTERROR__
#define __OO_ORTI_SERVICETRACE__
#define __OO_ORTI_PRIORITY__
#define __OO_ORTI_RES_LOCKER_TASK__
#define __OO_ORTI_RES_ISLOCKED__
#define __OO_ORTI_STACK__
#define __OO_ORTI_ALARMTIME__
#define __OO_ORTI_RUNNINGISR2__
#define __OO_ECC2__
#define __OO_EXTENDED_STATUS__
#define __OO_HAS_STARTUPHOOK__
#define __OO_HAS_ERRORHOOK__
#define __IRQ_STACK_NEEDED__
#define __MULTI__
#define __OO_HAS_USERESSCHEDULER__
#define __OO_NO_EVENTS__
#define __OO_AUTOSTART_TASK__
#define __OO_AUTOSTART_ALARM__
#define __ALLOW_NESTED_IRQ__

#endif

    /* System stack size */
    #define EE_SYS_STACK_SIZE     4096



/***************************************************************************
 *
 * Counter defines
 *
 **************************************************************************/
#define OSMAXALLOWEDVALUE_myCounter 65535U
#define OSTICKSPERBASE_myCounter    1U
#define OSMINCYCLE_myCounter        2U



/***************************************************************************
 *
 * Vector size defines
 *
 **************************************************************************/
    #define EE_ACTION_ROM_SIZE 2
    #define EE_ALARM_ROM_SIZE 2
    #define EE_COUNTER_OBJECTS_ROM_SIZE 2
    #define EE_COUNTER_ROM_SIZE 1
    #define EE_E200Z7_SYSTEM_TOS_SIZE 3
    #define EE_OO_AUTOSTART_ALARM_MODE_OSDEFAULTAPPMODE_SIZE 2
    #define EE_OO_AUTOSTART_TASK_MODE_OSDEFAULTAPPMODE_SIZE 2
    #define EE_RQ_PAIRS_NEXT_SIZE 2
    #define EE_RQ_PAIRS_TID_SIZE 2
    #define EE_RQ_QUEUES_HEAD_SIZE 16
    #define EE_RQ_QUEUES_TAIL_SIZE 16


#endif

