OBJS = 	\
	interface/HasIDLVTMap.o \
	interface/HasResponseMessage.o \
	interface/HasRollbackTagList.o \
	interface/HasSendList.o \
	interface/Agent.o \
	interface/IAlp.o \
	interface/IdentifierHandler.o \
	interface/Interface.o \
	lp/AccessCostCalculator.o \
	lp/Clp.o \
   	lp/GvtCalculator.o \
   	lp/Lp.o \
	lp/LpId.o \
   	lp/MessageQueue.o \
   	lp/MpiInterface.o \
   	lp/ReceiveThread.o \
   	lp/RollbackList.o \
   	lp/RollbackTag.o \
	lp/RQPortScanStatus.o \
   	lp/SendThread.o \
	rangequery/RangePeriod.o \
	rangequery/RangePeriodListEntry.o \
	rangequery/RangeRoutingTable.o \
	rangequery/RangeTracker.o \
	rangequery/RangeUpdates.o \
   	routing/ForwardingTable.o \
   	routing/Router.o \
	routing/RouteTable.o \
	routing/RoutingInfo.o \
	messages/content/HasDestination.o \
	messages/content/HasGVT.o \
	messages/content/HasIdentifier.o \
	messages/content/HasMatternColour.o \
	messages/content/HasMatternCut.o \
	messages/content/HasMessageCount.o \
	messages/content/HasMessageMinimumTime.o \
	messages/content/HasNumberOfHops.o \
	messages/content/HasOrigin.o \
	messages/content/HasOriginalAgent.o \
	messages/content/HasRange.o \
	messages/content/HasRedMessageTime.o \
	messages/content/HasRollbackTag.o \
	messages/content/HasSenderAlp.o \
	messages/content/HasSSVID.o \
	messages/content/HasSSVIDValueMap.o \
	messages/content/HasStateVariableMap.o \
	messages/content/HasTimestamp.o \
	messages/content/HasNumberOfTraverseHops.o \
	messages/content/HasValue.o \
	messages/content/HasWriteStatus.o \
   	messages/AbstractMessage.o \
   	messages/AntiMessage.o \
   	messages/ControlMessage.o \
	messages/EndMessage.o \
   	messages/GvtControlMessage.o \
   	messages/GvtMessage.o \
   	messages/GvtRequestMessage.o \
   	messages/GvtValueMessage.o \
   	messages/LoadBalancingMessage.o \
   	messages/RangeQueryAntiMessage.o \
   	messages/RangeQueryMessage.o \
	messages/RangeUpdateMessage.o \
	messages/ResponseMessage.o \
   	messages/RollbackMessage.o \
	messages/SharedStateMessage.o \
 	messages/SimulationMessage.o \
   	messages/SingleReadAntiMessage.o \
   	messages/SingleReadMessage.o \
   	messages/SingleReadResponseMessage.o \
   	messages/StateMigrationMessage.o \
   	messages/WriteAntiMessage.o \
   	messages/WriteMessage.o \
   	messages/WriteResponseMessage.o \
	parse/Initialisor.o \
   	state/AbstractValue.o \
	state/ObjectMgr.o \
	state/SharedState.o \
   	state/SsvId.o \
	state/StateVariable.o \
	state/WritePeriod.o \
	types/Point.o \
	types/Range.o \
	types/Types.o \
	util/FilePrint.o \
	util/Log.o \
	util/Mutex.o \
	util/Semaphore.o \
	util/Thread.o

LIBSSLP_INC_DIR = ../include/ 
LIBSSLP_INC_DIR_INT = ../include/interface/ 
LIBSSLP_INC_DIR_LP = ../include/lp/ 
LIBSSLP_INC_DIR_MSGS = ../include/messages/ 
LIBSSLP_INC_DIR_MSGS_CONT = ../include/messages/content/ 
LIBSSLP_INC_DIR_PARSE = ../include/parse/
LIBSSLP_INC_DIR_RANGEQUERY = ../include/rangequery/
LIBSSLP_INC_DIR_ROUTING = ../include/routing/ 
LIBSSLP_INC_DIR_STATE = ../include/state/ 
LIBSSLP_INC_DIR_TYPES = ../include/types/ 
LIBSSLP_INC_DIR_UTL = ../include/util/ 
# For on the cluster
MPI_INCLUDE = /apps/openmpi/1.4.3/gcc_4.5.0-tm-eth-threads/include
# For on the laptop 
#MPI_INCLUDE = /home/bcraenen/lib/openmpi-1.4.2/include 
JNI_INCLUDE = /usr/lib/jvm/java-6-sun/include 
JNI_LINUX_INCLUDE = /usr/lib/jvm/java-6-sun/include/linux 

INC_PATH = -I${LIBSSLP_INC_DIR} -I${LIBSSLP_INC_DIR_INT} -I${LIBSSLP_INC_DIR_LP} -I${LIBSSLP_INC_DIR_MSGS} -I${LIBSSLP_INC_DIR_MSGS_CONT} -I${LIBSSLP_INC_DIR_PARSE} -I${LIBSSLP_INC_DIR_RANGEQUERY} -I${LIBSSLP_INC_DIR_ROUTING} -I${LIBSSLP_INC_DIR_STATE} -I${LIBSSLP_INC_DIR_TYPES} -I${LIBSSLP_INC_DIR_UTL} -I${MPI_INCLUDE} -I${JNI_INCLUDE} -I${JNI_LINUX_INCLUDE}

LIBNAME = libPDESMAS.so
LIBRARY = ${LIBNAME}
SO_FLAGS = -shared -Wl,-soname,${LIBNAME}

# For on the cluster
SO++ = mpiCC
# For on the laptop
#SO++ = /home/bcraenen/lib/openmpi-1.4.2/bin/mpiCC

# Build targets
%.o : %.cpp
	@echo
	@echo Compiling $< ...
	@echo
	${C++} -c ${DBFLAGS} ${C++FLAGS} ${INC_PATH} $< -o $@

default: ${LIBRARY}

${LIBRARY}: ${OBJS}
	@echo
	@echo Linking $@ ...
	@echo
	rm -f $@
	${SO++} ${LDFLAGS} ${LIBS} ${DBFLAGS} ${SO_FLAGS} -o $@ ${OBJS} ${LBOBJS}$
	@echo
	@echo -------All Done Created: $@ at `date`-------
	@echo

clean:
	rm -rf *.o core *~ .depend Templates.DB ${LIBRARY}
	rm -rf messages/*.o messages/core messages/*~
	rm -rf messages/content/*.o messages/content/core messages/content/*~
	rm -rf lp/*.o lp/core lp/*~
	rm -rf state/*.o state/core state/*~
	rm -rf parse/*.o parse/core parse/*~	
	rm -rf interface/*.o interface/core interface/*~
	rm -rf util/*.o util/core util/*~
	rm -rf types/*.o types/core types/*~
	rm -rf rangequery/*.o rangequery/core rangequery/*~
	rm -rf routing/*.o routing/core routing/*~
