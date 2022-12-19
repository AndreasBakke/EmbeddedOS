/*
*********************************************************************************************************
*                                              uC/CSP
*                                        Chip Support Package
*
*                            (c) Copyright 2010; Micrium, Inc.; Weston, FL
*                         (c) Copyright 2003-2010; Micrium, Inc.; Weston, FL
*
*               All rights reserved. Protected by international copyright laws.
*
*               uC/CSP is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      FAMILY DEFINITIONS & API.
*
* Filename      : csp_dev.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
* Note(s)       : (1) The following products are supported by this family definition.
*                    
*                     +============================================================================+
*                     |   MCU      |   FLASH | SRAM | Ethernet |  USB           | CAN  | I2S | DAC |
*                     +======================|======|==========|================|==================|
*                     |  LPC1751   |   32KB  |  8KB |    N     |  Dev           |  1   |   N |  N  |
*                     |  LPC1752   |   64KB  | 16KB |    N     |  Dev           |  1   |   N |  N  |
*                     |  LPC1754   |  128KB  | 32KB |    N     |  Dev/Host/OTG  |  1   |   N |  Y  |
*                     |  LPC1756   |  256KB  | 32KB |    N     |  Dev/Host/OTG  |  2   |   Y |  Y  |
*                     |  LPC1758   |  512KB  | 64KB |    Y     |  Dev/Host/OTG  |  2   |   Y |  Y  |
*                     |  LPC1764   |  128KB  | 32KB |    N     |  Dev           |  2   |   N |  N  |
*                     |  LPC1765   |  256KB  | 64KB |    N     |  Dev/Host/OTG  |  2   |   Y |  Y  |
*                     |  LPC1766   |  256KB  | 64KB |    Y     |  Dev/Host/OTG  |  2   |   Y |  Y  |
*                     |  LPC1768   |  512KB  | 64KB |    Y     |  Dev/Host/OTG  |  2   |   Y |  Y  |
*                     +============================================================================+
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  CSP_GRP_MODULE_PRESENT
#define  CSP_GRP_MODULE_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/


#include <csp_types.h>

/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   CSP_GRP_MODULE
#define  CSP_GRP_EXT
#else
#define  CSP_GRP_EXT  extern
#endif


/*
*********************************************************************************************************
*                                        DEFAULT CONFIGURATION
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                           FAMILY DEFINITION
*********************************************************************************************************
*/

#define  CSP_GRP_LPC17xx
#define  CSP_GRP_NAME                           "LPC17xx"


/*
*********************************************************************************************************
*                                  LPC17xx INTERRUPT CONTROLLER DEVICE NUMBERS
*
* Note(s): (1) The LPC17xx uses the Nested Vectored Interrupt Controller (NVIC)that is an integral part of 
*              the ARM Cortex-M3 architecture.
* 
*              (a) Interrupts 0  to 15  are internals and common for all the ARM Cortex-M3 processors.
*              (b) Interurpts 16 to 256 are externals and varies among CPU's implementation.
* 
*
*          (2) Some interrupts lines are ORed together to a single channel of the vectored interrupt controller. 
*              To reduce the complexity of the interrupt handler for ORed interrupt lines a shared interupt handler
*              is implemented to allow the application to install a ISR for every interrupt line form the ORed channnel.
*              In the case of the of the LPC17xx a virtual interrupt controller is used for the USB shared interrupt handler.
*
*                         
*                                +-------------+                                       +-------------+
*                                |             |                        src16 -------->|             |
*                 src16 -------->|  Interrupt  |                        src17 -------->|  Interrupt  |
*                 src17 -------->|  Controller |                        src18 -------->|  Controller |
*                 src18 -------->|             |                                   .   |             |
*                 src19 -------->|    NVIC     |                                   .   |    NVIC     |
*                            .   |             |  ====                             .   |             |  
*                            .   |             |  ====                  srcn --------->|             |  
*                            .   |             |                                       |             |
*                 srcn --------->|             |                                       |             |
*                                |             |                                       +-------------+
*                   +-----+  --->|             |                    +------------+
*      usb_src0 --->|     |  |   +-------------|       usb_src0 --->|            |
*      usb_src1 --->|     |  |                         usb_src1 --->|    USB     |
*      usb_src2 --->|     |  |                         usb_src2 --->| Software   |
*          .        |  OR |--|                              .       | Interrupt  |
*          .        |     |                                 .       | controller |
*          .        |     |                                 .       |            |
*      usb_srcn --->|     |                            usb_srcn --->|            |
*                   +-----+                                         +------------+
*   
*           
*                For example two independent ISR can be installed for the USB Host and the OTG interrupt                
*
*                CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_USB,
*                               (CSP_DEV_NBR   )CSP_INT_USB_SRC_NBR_OTG,
*                               (CPU_FNCT_PTR  )OTG_Handler,
*                               (CPU_VOID     *)0);
*
*                CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_USB,
*                               (CSP_DEV_NBR   )CSP_INT_USB_SRC_NBR_HOST,
*                               (CPU_FNCT_PTR  )HOST_Handler,
*                               (CPU_VOID     *)0);
*
*                (a) The 'USB shared interrupt handler" is available for the following processors:
*                        LPC1754
*                        LPC1756
*                        LPC1758
*                        LPC1765
*                        LPC1766
*                        LPC1768
*********************************************************************************************************
*/

#define  CSP_INT_CTRL_NBR_MAIN                  (CSP_DEV_NBR)(0u)   /* NVIC    External Interrupt Controller.           */
#define  CSP_INT_CTRL_NBR_USB                   (CSP_DEV_NBR)(1u)   /* Virtual USB      Interrupt Controller.           */


/*
*********************************************************************************************************
*                                          INTERRUPT SOURCES
*
* Note(s) : (1) Interrupt controller sources #define's use the following standard:
*
*               #define  CSP_INT_SRC_NBR_<PER>_<EVENT>_<PER_NBR>
*
*               where 
*
*                   <PER>      Peripheral name (UART, ETHER, DMA, USB, etc)
*                   <EVENT>    Interrupt event (RX, TX, MATCH, ALERT, TO, etc)
*                              Empty for multiple events interrupts.
*                   <PER_NBR>  Peripheral number. (00, 01, 02, etc)
*
*              #define  CSP_INT_SRC_NBR_UART_RX_00    UART 00 receive interrupt.
*              #define  CSP_INT_SRC_NBR_UART_TX_01    UART 01 receive interrupt. 
*
*            (2) 'CSP_INT_SRC_NBR_MAX' defines maximum number of interrupts in the interrupt controller.
*********************************************************************************************************
*/

#define  CSP_INT_SRC_NBR_WDT_00                 (CSP_DEV_NBR)(0u)     /* Watchdog.                                        */
#define  CSP_INT_SRC_NBR_TMR_00                 (CSP_DEV_NBR)(1u)     /* Timer 0 (Match 0-3 Capture 0-3).                 */
#define  CSP_INT_SRC_NBR_TMR_01                 (CSP_DEV_NBR)(2u)     /* Timer 1 (Match 0-3 Capture 0-3).                 */
#define  CSP_INT_SRC_NBR_TMR_02                 (CSP_DEV_NBR)(3u)     /* Timer 2 (Match 0-3 Capture 0-3).                 */
#define  CSP_INT_SRC_NBR_TMR_03                 (CSP_DEV_NBR)(4u)     /* Timer 2 (Match 0-3 Capture 0-3).                 */
#define  CSP_INT_SRC_NBR_UART_00                (CSP_DEV_NBR)(5u)     /* UART 0  (RLS, THRE, RDA, CTI).                   */
#define  CSP_INT_SRC_NBR_UART_01                (CSP_DEV_NBR)(6u)     /* UART 1  (RLS, THRE, RDA, CTI, MSI).              */
#define  CSP_INT_SRC_NBR_UART_02                (CSP_DEV_NBR)(7u)     /* UART 1  (RLS, THRE, RDA, CTI, MSI).              */
#define  CSP_INT_SRC_NBR_UART_03                (CSP_DEV_NBR)(8u)     /* UART 1  (RLS, THRE, RDA, CTI, MSI).              */
#define  CSP_INT_SRC_NBR_PWM_01                 (CSP_DEV_NBR)(9u)     /* PWM 1   (Match 0-6 Capture 0-3).                 */
#define  CSP_INT_SRC_NBR_I2C_00                 (CSP_DEV_NBR)(10u)    /* I2C     (SI).                                    */
#define  CSP_INT_SRC_NBR_I2C_01                 (CSP_DEV_NBR)(11u)    /* I2C     (SI).                                    */
#define  CSP_INT_SRC_NBR_I2C_02                 (CSP_DEV_NBR)(12u)    /* I2C     (SI).                                    */
#define  CSP_INT_SRC_NBR_SPI_00                 (CSP_DEV_NBR)(13u)    /* SPI0    (SPIF, MODF).                            */
#define  CSP_INT_SRC_NBR_SSP_00                 (CSP_DEV_NBR)(14u)    /* SSP0    Tx/Rx FIFO Int. Rx Timeout/Overrun.      */
#define  CSP_INT_SRC_NBR_SSP_01                 (CSP_DEV_NBR)(15u)    /* SSP1    Tx/Rx FIFO Int. Rx Timeout/Overrun.      */
#define  CSP_INT_SRC_NBR_PLL_00                 (CSP_DEV_NBR)(16u)    /* PLL lock (PLOCK).                                */
#define  CSP_INT_SRC_NBR_RTC_00                 (CSP_DEV_NBR)(17u)    /* RTC     (RTCCIF, RTCALF).                        */
#define  CSP_INT_SRC_NBR_EINT_00                (CSP_DEV_NBR)(18u)    /* External interrupt 0 (EINT0).                    */
#define  CSP_INT_SRC_NBR_EINT_01                (CSP_DEV_NBR)(19u)    /* External interrupt 1 (EINT1).                    */
#define  CSP_INT_SRC_NBR_EINT_02                (CSP_DEV_NBR)(20u)    /* External interrupt 2 (EINT2).                    */
#define  CSP_INT_SRC_NBR_EINT_03                (CSP_DEV_NBR)(21u)    /* External interrupt 3 (EINT3).                    */
#define  CSP_INT_SRC_NBR_ADC_00                 (CSP_DEV_NBR)(22u)    /* A/D Converter 0 end of conversion.               */
#define  CSP_INT_SRC_NBR_BROWN_OUT_00           (CSP_DEV_NBR)(23u)    /* Brown out detect.                                */
#define  CSP_INT_SRC_NBR_USB_00                 (CSP_DEV_NBR)(24u)    /* USB Interrupts, DMA Interrupt.                   */
#define  CSP_INT_SRC_NBR_CAN_00                 (CSP_DEV_NBR)(25u)    /* CAN Interrupt.                                   */
#define  CSP_INT_SRC_NBR_DMA_00                 (CSP_DEV_NBR)(26u)    /* IntSatus of DMA channel 0/1.                     */
#define  CSP_INT_SRC_NBR_I2S_00                 (CSP_DEV_NBR)(27u)    /* SI (state change).                               */
#define  CSP_INT_SRC_NBR_ETHER_00               (CSP_DEV_NBR)(28u)    /* Ethernet Interrupt.                              */
#define  CSP_INT_SRC_NBR_RITINT_00              (CSP_DEV_NBR)(29u)    /* Repetitive interrupt timer.                      */
#define  CSP_INT_SRC_NBR_MOTOR_PWM_00           (CSP_DEV_NBR)(30u)    /* Motor Control PWM.                               */
#define  CSP_INT_SRC_NBR_QUAD_ENC_00            (CSP_DEV_NBR)(31u)    /* Quadrature Encoder.                              */
#define  CSP_INT_SRC_NBR_PLL_01                 (CSP_DEV_NBR)(32u)    /* PLL1 lock (PLOCK).                               */
#define  CSP_INT_SRC_NBR_USB_ACT_00             (CSP_DEV_NBR)(33u)    /* USB Activity.                                    */
#define  CSP_INT_SRC_NBR_CAN_ACT_00             (CSP_DEV_NBR)(34u)    /* CAN Activity.                                    */

#define  CSP_INT_SRC_NBR_MAX                    (CSP_DEV_NBR)(35u)    /* Total number of external sources                 */


/*
*********************************************************************************************************
*                                    LPC17xx SOFT USB INTERRUPT CONTROLLER SOURCES
*********************************************************************************************************
*/

#define  CSP_INT_USB_SRC_NBR_DEV                (CSP_DEV_NBR)0u
#define  CSP_INT_USB_SRC_NBR_HOST               (CSP_DEV_NBR)1u
#define  CSP_INT_USB_SRC_NBR_ATX                (CSP_DEV_NBR)2u
#define  CSP_INT_USB_SRC_NBR_OTG                (CSP_DEV_NBR)3u
#define  CSP_INT_USB_SRC_NBR_I2C                (CSP_DEV_NBR)4u

#define  CSP_INT_USB_SRC_MAX_NBR                (CSP_DEV_NBR)5u


/*
*********************************************************************************************************
*                                     PERIPHERALS CLOCKS DEFINES
*
* Note(s) : (1) The peripheral's clock #define's use the following standard.
*        
*                   #define  CSP_PM_PER_CLK_NBR_<PER>_<PER_NBR>
*
*                   where,
*
*                   <PER>      Peripheral clock name.
*                   <PER_NBR>  Peripheral clock number identifier.
*********************************************************************************************************
*/

#define  CSP_PM_PER_CLK_NBR_WDT_00              (CSP_DEV_NBR)(0u)    /* Watchdog Timer                                        */
#define  CSP_PM_PER_CLK_NBR_TMR_00              (CSP_DEV_NBR)(1u)    /* Timer 0                                               */
#define  CSP_PM_PER_CLK_NBR_TMR_01              (CSP_DEV_NBR)(2u)    /* Timer 1                                               */
#define  CSP_PM_PER_CLK_NBR_UART_00             (CSP_DEV_NBR)(3u)    /* UART 0                                                */
#define  CSP_PM_PER_CLK_NBR_UART_01             (CSP_DEV_NBR)(4u)    /* UART 1                                                */
#define  CSP_PM_PER_CLK_NBR_PWM_01              (CSP_DEV_NBR)(5u)    /* PWM1                                                  */
#define  CSP_PM_PER_CLK_NBR_I2C_00              (CSP_DEV_NBR)(7u)    /* I2C 0                                                 */
#define  CSP_PM_PER_CLK_NBR_SPI_00              (CSP_DEV_NBR)(8u)    /* SPI 0                                                 */
#define  CSP_PM_PER_CLK_NBR_RTC_00              (CSP_DEV_NBR)(9u)    /* RTC                                                   */
#define  CSP_PM_PER_CLK_NBR_SSP_01              (CSP_DEV_NBR)(10u)   /* SSP 1                                                 */
#define  CSP_PM_PER_CLK_NBR_AD_00               (CSP_DEV_NBR)(12u)   /* A/D converter                                         */
#define  CSP_PM_PER_CLK_NBR_CAN_01              (CSP_DEV_NBR)(13u)   /* CAN 1                                                 */
#define  CSP_PM_PER_CLK_NBR_CAN_02              (CSP_DEV_NBR)(14u)   /* CAN 2                                                 */
#define  CSP_PM_PER_CLK_NBR_GPIO_00             (CSP_DEV_NBR)(15u)   /* GPIO                                                  */
#define  CSP_PM_PER_CLK_NBR_ACF_00              (CSP_DEV_NBR)(15u)   /* CAN filtering.                                        */ 
#define  CSP_PM_PER_CLK_NBR_PRIT_00             (CSP_DEV_NBR)(16u)   /* Repetitive Interrupt Timer (Power)                    */
#define  CSP_PM_PER_CLK_NBR_QEI                 (CSP_DEV_NBR)(16u)   /* QEI                                                   */
#define  CSP_PM_PER_CLK_NBR_P_MOTOR_PWM_00      (CSP_DEV_NBR)(17u)   /* Motor Control PWM (Power)                             */
#define  CSP_PM_PER_CLK_NBR_GPIO_INT_00         (CSP_DEV_NBR)(17u)   /* GPIO INT                                              */
#define  CSP_PM_PER_CLK_NBR_P_QUAD_ENC_00       (CSP_DEV_NBR)(18u)   /* Quadrature Encoder Interface                          */
#define  CSP_PM_PER_CLK_NBR_PCB_00              (CSP_DEV_NBR)(18u)   /* Pin Connect Block                                     */
#define  CSP_PM_PER_CLK_NBR_I2C_01              (CSP_DEV_NBR)(19u)   /* I2C1                                                  */
#define  CSP_PM_PER_CLK_NBR_SSP_00              (CSP_DEV_NBR)(21u)   /* SSP0                                                  */
#define  CSP_PM_PER_CLK_NBR_TMR_02              (CSP_DEV_NBR)(22u)   /* TIMER 2                                               */
#define  CSP_PM_PER_CLK_NBR_TMR_03              (CSP_DEV_NBR)(23u)   /* TIMER 3                                               */
#define  CSP_PM_PER_CLK_NBR_UART_02             (CSP_DEV_NBR)(24u)   /* UART 2                                                */
#define  CSP_PM_PER_CLK_NBR_UART_03             (CSP_DEV_NBR)(25u)   /* UART 3                                                */
#define  CSP_PM_PER_CLK_NBR_I2C_02              (CSP_DEV_NBR)(26u)   /* I2C 2                                                 */
#define  CSP_PM_PER_CLK_NBR_I2S_00              (CSP_DEV_NBR)(27u)   /* I2S                                                   */
#define  CSP_PM_PER_CLK_NBR_DMA_00              (CSP_DEV_NBR)(29u)   /* GP DMA function                                       */
#define  CSP_PM_PER_CLK_NBR_RIT_00              (CSP_DEV_NBR)(29u)   /* RIT                                                   */
#define  CSP_PM_PER_CLK_NBR_ETHER_00            (CSP_DEV_NBR)(30u)   /* Ethernet Controller                                   */
#define  CSP_PM_PER_CLK_NBR_SYSCON_00           (CSP_DEV_NBR)(30u)   /* Sytem Control Block                                   */
#define  CSP_PM_PER_CLK_NBR_USB_00              (CSP_DEV_NBR)(31u)   /* USB                                                   */
#define  CSP_PM_PER_CLK_NBR_MOTOR_PWM_00        (CSP_DEV_NBR)(31u)   /* Motor Control PWM                                     */

#define  CSP_PM_PER_CLK_NBR_MAX               (CSP_DEV_NBR)(32u)


/*
*********************************************************************************************************
*                                          DMA TRANSFER TYPES
*********************************************************************************************************
*/

#define  CSP_DMA_XFER_TYPE_MEM_TO_MEM         (CSP_OPT    )(0u)      /* Memory     to memory     transfer                    */
#define  CSP_DMA_XFER_TYPE_PER_TO_MEM         (CSP_OPT    )(1u)      /* Peripheral to memory     transfer.                   */
#define  CSP_DMA_XFER_TYPE_MEM_TO_PER         (CSP_OPT    )(2u)      /* Memory     to peripheral transfer.                   */
#define  CSP_DMA_XFER_TYPE_PER_TO_PER         (CSP_OPT    )(3u)      /* Peripheral to peripheral transfer.                   */


/*
*********************************************************************************************************
*                                             DMA PRIORITIES
*********************************************************************************************************
*/

#define  CSP_DMA_CH_PRIO_LOWEST               (CSP_OPT    )(0u)
#define  CSP_DMA_CH_PRIO_HIGHEST              (CSP_OPT    )(7u)
#define  CSP_DMA_CH_PRIO_ANY_LOW              (CSP_OPT    )(8u)
#define  CSP_DMA_CH_PRIO_ANY_HIGH             (CSP_OPT    )(9u)


/*
*********************************************************************************************************
*                                              DMA OPTIONS FLAGS
*********************************************************************************************************
*/

#define  CSP_DMA_OPT_FLAG_XFER_SRC_INC          DEF_BIT_00
#define  CSP_DMA_OPT_FLAG_XFER_DEST_INC         DEF_BIT_01


/*
*********************************************************************************************************
*                                      DMA PERIPHERAL REQUEST SIGNALS
*********************************************************************************************************
*/

#define  CSP_DMA_XFER_PER_REQ_SSP_00_TX         (CSP_DEV_NBR)(0u)
#define  CSP_DMA_XFER_PER_REQ_SSP_00_RX         (CSP_DEV_NBR)(1u)
#define  CSP_DMA_XFER_PER_REQ_SSP_01_TX         (CSP_DEV_NBR)(2u)
#define  CSP_DMA_XFER_PER_REQ_SSP_01_RX         (CSP_DEV_NBR)(3u)
#define  CSP_DMA_XFER_PER_REQ_ADC               (CSP_DEV_NBR)(4u)
#define  CSP_DMA_XFER_PER_REQ_I2S_00            (CSP_DEV_NBR)(5u)
#define  CSP_DMA_XFER_PER_REQ_I2S_01            (CSP_DEV_NBR)(6u)
#define  CSP_DMA_XFER_PER_REQ_DAC               (CSP_DEV_NBR)(7u)
#define  CSP_DMA_XFER_PER_REQ_UART_00_TX        (CSP_DEV_NBR)(8u)
#define  CSP_DMA_XFER_PER_REQ_UART_00_RX        (CSP_DEV_NBR)(9u)
#define  CSP_DMA_XFER_PER_REQ_UART_01_TX        (CSP_DEV_NBR)(10u)
#define  CSP_DMA_XFER_PER_REQ_UART_01_RX        (CSP_DEV_NBR)(11u)
#define  CSP_DMA_XFER_PER_REQ_UART_02_TX        (CSP_DEV_NBR)(12u)
#define  CSP_DMA_XFER_PER_REQ_UART_02_RX        (CSP_DEV_NBR)(13u)
#define  CSP_DMA_XFER_PER_REQ_UART_03_TX        (CSP_DEV_NBR)(14u)
#define  CSP_DMA_XFER_PER_REQ_UART_03_RX        (CSP_DEV_NBR)(15u)
#define  CSP_DMA_XFER_PER_REQ_TMR_00_MATCH_0    (CSP_DEV_NBR)(16u)
#define  CSP_DMA_XFER_PER_REQ_TMR_00_MATCH_1    (CSP_DEV_NBR)(17u)
#define  CSP_DMA_XFER_PER_REQ_TMR_01_MATCH_0    (CSP_DEV_NBR)(18u)
#define  CSP_DMA_XFER_PER_REQ_TMR_01_MATCH_1    (CSP_DEV_NBR)(19u)
#define  CSP_DMA_XFER_PER_REQ_TMR_02_MATCH_0    (CSP_DEV_NBR)(20u)
#define  CSP_DMA_XFER_PER_REQ_TMR_02_MATCH_1    (CSP_DEV_NBR)(21u)
#define  CSP_DMA_XFER_PER_REQ_TMR_03_MATCH_0    (CSP_DEV_NBR)(22u)
#define  CSP_DMA_XFER_PER_REQ_TMR_03_MATCH_1    (CSP_DEV_NBR)(23u)


/*
*********************************************************************************************************
*                                           TIMER MATCH DEFINES
*********************************************************************************************************
*/

#define  CSP_TMR_MATCH_NBR_00                 (CSP_DEV_NBR)(0u)
#define  CSP_TMR_MATCH_NBR_01                 (CSP_DEV_NBR)(1u)
#define  CSP_TMR_MATCH_NBR_02                 (CSP_DEV_NBR)(2u)
#define  CSP_TMR_MATCH_NBR_03                 (CSP_DEV_NBR)(3u)

#define  CSP_TMR_EXT_MATCH_OPT_NONE           (CSP_OPT    )(0u)
#define  CSP_TMR_EXT_MATCH_OPT_CLR            (CSP_OPT    )(1u)
#define  CSP_TMR_EXT_MATCH_OPT_SET            (CSP_OPT    )(2u)
#define  CSP_TMR_EXT_MATCH_OPT_TOGGLE         (CSP_OPT    )(3u)


/*
*********************************************************************************************************
*                                              SPI DEFINES
*********************************************************************************************************
*/

#define  CSP_SPI_NBR_MAX                      (CSP_DEV_NBR)(1u)


/*
*********************************************************************************************************
*                                              I2C DEFINES
*********************************************************************************************************
*/

#define  CSP_I2C_NBR_MAX                      (CSP_DEV_NBR)(3u)


/*
*********************************************************************************************************
*                                              SPI DEFINES
*********************************************************************************************************
*/

#define  CSP_SSP_NBR_00                         (CSP_DEV_NBR)( 0u)
#define  CSP_SSP_NBR_01                         (CSP_DEV_NBR)( 1u)
#define  CSP_SSP_NBR_02                         (CSP_DEV_NBR)( 2u)
#define  CSP_SSP_NBR_03                         (CSP_DEV_NBR)( 3u)
#define  CSP_SSP_NBR_04                         (CSP_DEV_NBR)( 4u)
#define  CSP_SSP_NBR_05                         (CSP_DEV_NBR)( 5u)
#define  CSP_SSP_NBR_06                         (CSP_DEV_NBR)( 6u)
#define  CSP_SSP_NBR_07                         (CSP_DEV_NBR)( 7u)
#define  CSP_SSP_NBR_08                         (CSP_DEV_NBR)( 8u)
#define  CSP_SSP_NBR_09                         (CSP_DEV_NBR)( 9u)
#define  CSP_SSP_NBR_10                         (CSP_DEV_NBR)(10u)
#define  CSP_SSP_NBR_11                         (CSP_DEV_NBR)(11u)
#define  CSP_SSP_NBR_12                         (CSP_DEV_NBR)(12u)
#define  CSP_SSP_NBR_13                         (CSP_DEV_NBR)(13u)
#define  CSP_SSP_NBR_14                         (CSP_DEV_NBR)(14u)
#define  CSP_SSP_NBR_15                         (CSP_DEV_NBR)(15u)

#define  CSP_SSP_NBR_MAX                        (CSP_DEV_NBR)(2u)
#define  CSP_SSP_FRAME_FMT_SPI                  (CSP_OPT    )(0u) 
#define  CSP_SSP_FRAME_FMT_TI                   (CSP_OPT    )(1u) 
#define  CSP_SSP_FRAME_FMT_MICROWIRE            (CSP_OPT    )(2u) 


/*
*********************************************************************************************************
*                                            DATA TYPES
*********************************************************************************************************
*/

typedef  CPU_INT32U  CSP_GPIO_MSK;                              /* GPIO port size.                                      */
typedef  CPU_INT32U  CSP_TMR_VAL;                               /* Timer value size.                                    */


/*
*********************************************************************************************************
*                                      INTERRUPT VECTOR DATA TYPE
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      DMA CALL BACK FUNCTION
*********************************************************************************************************
*/


typedef  void  (*CSP_DMA_CALLBACK_PTR)(CSP_DEV_NBR  ch_nbr,                                                            
                                       CPU_INT16U   xfer_size_rem, 
                                       void        *p_arg,                           
                                       CPU_BOOLEAN  status);


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                               MCU's FAMILY DEPENDENT API (EXTENDED API)
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         DMA FUNCTIONS
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_DMA_InitExt            (void);

CPU_BOOLEAN  CSP_DMA_CH_AllocExt        (CSP_OPT                prio,
                                         CSP_DEV_NBR           *p_ch_nbr);

CPU_BOOLEAN  CSP_DMA_CH_FreeExt         (CSP_DEV_NBR            ch_nbr);
                                
CPU_BOOLEAN  CSP_DMA_CH_CfgExt          (CSP_DEV_NBR            ch_nbr,
                                         CSP_OPT                xfer_type,                                       
                                         CSP_OPT                src_burst_size,
                                         CSP_OPT                src_width,
                                         CSP_OPT                dest_burst_size,
                                         CSP_OPT                dest_width,
                                         CSP_DEV_NBR            src_req,
                                         CSP_DEV_NBR            dest_req);

CPU_BOOLEAN  CSP_DMA_XferStartExt       (CSP_DEV_NBR            ch_nbr,
                                         void                   *p_src,
                                         void                   *p_dest,
                                         CPU_SIZE_T             xfer_size,
                                         CSP_OPT_FLAGS          opt);

#if 0
CPU_BOOLEAN  CSP_DMA_XferSyncStart      (CSP_DEV_NBR            ch_nbr,
                                         void                  *p_src,
                                         void                  *p_dest,
                                         CPU_INT16U             timeout,
                                         CPU_INT16U            *p_xfer_size_rem,
                                         CSP_OPT_FLAGS          opt);
#endif

CPU_BOOLEAN  CSP_DMA_XferAsyncStartExt  (CSP_DEV_NBR            ch_nbr,
                                         void                  *p_src,
                                         void                  *p_dest,
                                         CPU_SIZE_T             xfer_size,
                                         CSP_DMA_CALLBACK_PTR   callback,
                                         void                  *p_arg,
                                         CSP_OPT_FLAGS          opt);

CPU_INT32U   CSP_DMA_XferSizeMaxGetExt  (CSP_DEV_NBR            ch_nbr);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of CSP_GRP module include.                      */

