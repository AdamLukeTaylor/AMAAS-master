#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin_4.x-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug32
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1492088801/CartPole.o \
	${OBJECTDIR}/_ext/1492088801/CartPoleAgent.o \
	${OBJECTDIR}/_ext/1634949378/ActionSelection.o \
	${OBJECTDIR}/_ext/1634949378/Boltzmann.o \
	${OBJECTDIR}/_ext/1634949378/CollaborationProcess.o \
	${OBJECTDIR}/_ext/1634949378/DWLAgent.o \
	${OBJECTDIR}/_ext/1634949378/EGready.o \
	${OBJECTDIR}/_ext/1634949378/NeighbourReward.o \
	${OBJECTDIR}/_ext/1634949378/Policy.o \
	${OBJECTDIR}/_ext/1634949378/QTable.o \
	${OBJECTDIR}/_ext/1634949378/Reward.o \
	${OBJECTDIR}/_ext/1634949378/TaylorSeriesSelection.o \
	${OBJECTDIR}/_ext/1634949378/TransferMapping.o \
	${OBJECTDIR}/_ext/1634949378/WLearningProcess.o \
	${OBJECTDIR}/_ext/1634949378/WTable.o \
	${OBJECTDIR}/_ext/1492088801/MtCar.o \
	${OBJECTDIR}/_ext/1492088801/MtCarAgent.o \
	${OBJECTDIR}/_ext/1492088801/RewardCartPole.o \
	${OBJECTDIR}/_ext/1492088801/RewardMtCar.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=-m32

# CC Compiler Flags
CCFLAGS=-m32
CXXFLAGS=-m32

# Fortran Compiler Flags
FFLAGS=-m32

# Assembler Flags
ASFLAGS=--32

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/amaas-master.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/amaas-master.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/amaas-master ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/1492088801/CartPole.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/CartPole.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1492088801
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1492088801/CartPole.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/CartPole.cpp

${OBJECTDIR}/_ext/1492088801/CartPoleAgent.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/CartPoleAgent.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1492088801
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1492088801/CartPoleAgent.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/CartPoleAgent.cpp

${OBJECTDIR}/_ext/1634949378/ActionSelection.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/ActionSelection.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/ActionSelection.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/ActionSelection.cpp

${OBJECTDIR}/_ext/1634949378/Boltzmann.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/Boltzmann.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/Boltzmann.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/Boltzmann.cpp

${OBJECTDIR}/_ext/1634949378/CollaborationProcess.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/CollaborationProcess.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/CollaborationProcess.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/CollaborationProcess.cpp

${OBJECTDIR}/_ext/1634949378/DWLAgent.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/DWLAgent.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/DWLAgent.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/DWLAgent.cpp

${OBJECTDIR}/_ext/1634949378/EGready.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/EGready.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/EGready.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/EGready.cpp

${OBJECTDIR}/_ext/1634949378/NeighbourReward.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/NeighbourReward.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/NeighbourReward.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/NeighbourReward.cpp

${OBJECTDIR}/_ext/1634949378/Policy.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/Policy.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/Policy.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/Policy.cpp

${OBJECTDIR}/_ext/1634949378/QTable.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/QTable.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/QTable.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/QTable.cpp

${OBJECTDIR}/_ext/1634949378/Reward.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/Reward.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/Reward.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/Reward.cpp

${OBJECTDIR}/_ext/1634949378/TaylorSeriesSelection.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/TaylorSeriesSelection.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/TaylorSeriesSelection.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/TaylorSeriesSelection.cpp

${OBJECTDIR}/_ext/1634949378/TransferMapping.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/TransferMapping.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/TransferMapping.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/TransferMapping.cpp

${OBJECTDIR}/_ext/1634949378/WLearningProcess.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/WLearningProcess.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/WLearningProcess.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/WLearningProcess.cpp

${OBJECTDIR}/_ext/1634949378/WTable.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/WTable.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1634949378
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1634949378/WTable.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/src/WTable.cpp

${OBJECTDIR}/_ext/1492088801/MtCar.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1492088801
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1492088801/MtCar.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar.cpp

${OBJECTDIR}/_ext/1492088801/MtCarAgent.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCarAgent.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1492088801
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1492088801/MtCarAgent.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCarAgent.cpp

${OBJECTDIR}/_ext/1492088801/RewardCartPole.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/RewardCartPole.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1492088801
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1492088801/RewardCartPole.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/RewardCartPole.cpp

${OBJECTDIR}/_ext/1492088801/RewardMtCar.o: /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/RewardMtCar.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1492088801
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1492088801/RewardMtCar.o /cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/RewardMtCar.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/DWL/include -I/cygdrive/C/Users/Adam/Documents/NetBeansProjects/AMAAS-master/MtCar -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/src -I/cygdrive/C/Program\ Files\ \(x86\)/Visual\ Leak\ Detector/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/amaas-master.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
