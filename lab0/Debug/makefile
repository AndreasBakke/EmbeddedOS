### Setting Erika's variables:
# ERIKA_FILES= ../../..
# APPBASE = .
# 
# EEOPT =


RTDRUID_CONFIGURATOR_NUMBER:=1278



############################################################################
#
# User options
#
############################################################################
EEOPT += DEBUG
EEOPT += __USE_LEDS__
EEOPT += __USE_BUTTONS__
EEOPT += __E200ZX_EXECUTE_FROM_RAM__
EEOPT += EE_ISR_DYNAMIC_TABLE
EEOPT += EE_LAUTERBACH_DEMO_SIM
EEOPT += __OO_STARTOS_OLD__


############################################################################
#
# Automatic options
#
############################################################################
EEOPT += __RTD_LINUX__
EEOPT += __PPCE200ZX__
EEOPT += __MPC5674F__
EEOPT += __PPCE200Z7__
EEOPT += EE_GNU__
EEOPT += __OO_ORTI_LASTERROR__
EEOPT += __OO_ORTI_SERVICETRACE__
EEOPT += __OO_ORTI_PRIORITY__
EEOPT += __OO_ORTI_RES_LOCKER_TASK__
EEOPT += __OO_ORTI_RES_ISLOCKED__
EEOPT += __OO_ORTI_STACK__
EEOPT += __OO_ORTI_ALARMTIME__
EEOPT += __OO_ORTI_RUNNINGISR2__
EEOPT += __OO_BCC1__
EEOPT += __OO_EXTENDED_STATUS__
EEOPT += __MONO__
EEOPT += __OO_NO_ALARMS__
EEOPT += __OO_NO_RESOURCES__
EEOPT += __OO_AUTOSTART_TASK__
EEOPT += __ALLOW_NESTED_IRQ__
EEOPT += __DISABLE_EEOPT_DEFINES__


############################################################################
#
# Erika base directory
#
############################################################################
ifdef ERIKA_FILES
ifdef EEBASE
    $(warning EEBASE is set, but it has been overridden by ERIKA_FILES)
endif
EEBASE := ${ERIKA_FILES}

else # ERIKA_FILES

ifndef EEBASE
        EEBASE := /opt/case/evidence/eclipse/plugins/com.eu.evidence.ee_2.4.0.N20140917_2014/ee_files
else
        $(warning The usage of EEBASE is deprecated. Please use ERIKA_FILES)
endif
endif # ERIKA_FILES
# ERIKA_FILES has fulfilled its role. Make sure it's not used inside Erika makefiles
ERIKA_FILES :=
$(info Using erika files in $(EEBASE))


############################################################################
#
# Freescale MPC5674F, PPC E200Z7
#
############################################################################
-include $(EEBASE)/pkg/cfg/path_helper.mk
APPBASE := ..
OUTBASE := Debug

$(eval $(call check_and_set_linux_compiler_path,PPC_GNU_BASEDIR,))


############################################################################
#
# Flags
#
############################################################################
CFLAGS  += 
ASFLAGS += 
LDFLAGS += 
LDDEPS  += 
LIBS    += 



############################################################################
#
# Sources
#
############################################################################
APP_SRCS += eecfg.c code.c


############################################################################
#
# end
#
############################################################################
include $(EEBASE)/pkg/cfg/rules.mk
