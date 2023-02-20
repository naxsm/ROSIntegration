#include "ROSBPMsg.h"



#define UROSBPMSG_CAST_STD_MSG(type, rosType) \
void UROSBPMsg::castTo ## rosType (type& value, bool& success)\
{\
if (msg->_MessageType != "std_msgs/" #rosType)\
	{\
		success = false;\
		return;\
	}\
	auto p = StaticCastSharedPtr<ROSMessages::std_msgs::rosType>(msg);\
	value = p->_Data;\
	success = true;\
}

UROSBPMSG_CAST_STD_MSG(bool, Bool)

UROSBPMSG_CAST_STD_MSG(float, Float32)

UROSBPMSG_CAST_STD_MSG(int32, Int32)

UROSBPMSG_CAST_STD_MSG(FString, String)

UROSBPMSG_CAST_STD_MSG(uint8, UInt8)
