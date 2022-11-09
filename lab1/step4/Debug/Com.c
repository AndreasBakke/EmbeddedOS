#include "com/com/inc/ee_cominit.h"


/***************************************************************************
 *
 * COM application modes
 *
 **************************************************************************/
COMApplicationModeType EE_com_validmodes[EE_COM_N_MODE] = {LAB1_APPMODE};

/* Tasks */
EE_TID task_to_call__Task2 = Task2;


/* Transmission */
EE_COM_DEFINE_INTERNAL_MESSAGE(msgSendCount, 32, msgReceiveCount);

/* Receiving msgSendCount */
EE_COM_DEFINE_INTERNAL_UNQUEUED_MESSAGE(msgReceiveCount, 32, F_Always, NULL,
                                        EE_COM_T_OK, &task_to_call__Task2, EE_COM_NULL);


/* Messages (RAM) */
struct EE_com_msg_RAM_TYPE * EE_com_msg_RAM[EE_COM_N_MSG] = {
    EE_com_msg_RAM(msgSendCount),
    EE_com_msg_RAM(msgReceiveCount)
};

/* Messages (ROM) */
const struct EE_com_msg_ROM_TYPE * EE_com_msg_ROM[EE_COM_N_MSG] = {
    EE_com_msg_ROM(msgSendCount),
    EE_com_msg_ROM(msgReceiveCount)
};

/* Messages initial Value (ROM) */
const EE_UINT64 EE_com_msg_init_val[EE_COM_N_MSG] = {
    0,
    0
};

