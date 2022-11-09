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
    #define EE_MAX_RESOURCE 0U

    /* ALARM definition */
    #define EE_MAX_ALARM 0U

    /* SCHEDULING TABLE definition */
    #define EE_MAX_SCHEDULETABLE 0U

    /* COUNTER OBJECTS definition */
    #define EE_MAX_COUNTER_OBJECTS (EE_MAX_ALARM + EE_MAX_SCHEDULETABLE)

    /* COUNTER definition */
    #define EE_MAX_COUNTER 0U

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
#define __OO_STARTOS_OLD__


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
#define __OO_BCC1__
#define __OO_EXTENDED_STATUS__
#define __MONO__
#define __OO_NO_ALARMS__
#define __OO_NO_RESOURCES__
#define __OO_AUTOSTART_TASK__
#define __ALLOW_NESTED_IRQ__

#endif

    /* System stack size */
    #define EE_SYS_STACK_SIZE     4096



/***************************************************************************
 *
 * Vector size defines
 *
 **************************************************************************/
    #define EE_OO_AUTOSTART_TASK_MODE_OSDEFAULTAPPMODE_SIZE 1


#endif

