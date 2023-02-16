#include "ROSBPMsg.h"

#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt8.h"


#define UROSBPMSG_CAST_STD_MSG(rosType) \
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

void UROSBPMsg::castToBool(bool& value, bool& success)
UROSBPMSG_CAST_STD_MSG(Bool)

void UROSBPMsg::castToFloat32(float& value, bool& success)
UROSBPMSG_CAST_STD_MSG(Float32)

void UROSBPMsg::castToInt32(int32& value, bool& success)
UROSBPMSG_CAST_STD_MSG(Int32)

void UROSBPMsg::castToString(FString& value, bool& success)
//UROSBPMSG_CAST_STD_MSG(String)
{
		if (msg->_MessageType != "std_msgs/String")
		{
			success = false;
			return;
		}
		auto p = StaticCastSharedPtr<ROSMessages::std_msgs::String>(msg);
		value = p->_Data;
		success = true;
}


void UROSBPMsg::castToUInt8(uint8& value, bool& success)
UROSBPMSG_CAST_STD_MSG(UInt8)
