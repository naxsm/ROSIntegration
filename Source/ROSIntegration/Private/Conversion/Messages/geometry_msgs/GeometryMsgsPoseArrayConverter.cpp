#include "GeometryMsgsPoseArrayConverter.h"

UGeometryMsgsPoseArrayConverter::UGeometryMsgsPoseArrayConverter(const FObjectInitializer& ObjectInitializer)
	//: Super(ObjectInitializer)
{
	_MessageType = "geometry_msgs/PoseArray";
}

bool UGeometryMsgsPoseArrayConverter::ConvertIncomingMessage(const ROSBridgePublishMsg* message, TSharedPtr<FROSBaseMsg>& BaseMsg)
{
	auto p = new ROSMessages::geometry_msgs::PoseArray();
	BaseMsg = TSharedPtr<FROSBaseMsg>(p);

	bool KeyFound = false;
	bson_t* b = message->full_msg_bson_;

	KeyFound = UStdMsgsHeaderConverter::_bson_extract_child_header(b, TEXT("msg.header"), &p->header); if (!KeyFound) return false;

	p->poses = GetTArrayFromBSON<ROSMessages::geometry_msgs::Pose>(FString("msg.poses"), b, KeyFound, [](FString subKey, bson_t* subMsg, bool& subKeyFound) {
		ROSMessages::geometry_msgs::Pose ret;
		subKeyFound = UGeometryMsgsPoseConverter::_bson_extract_child_pose(subMsg, subKey, &ret);
		return ret;
		});
	if (!KeyFound) return false;

	return true;
}

bool UGeometryMsgsPoseArrayConverter::ConvertOutgoingMessage(TSharedPtr<FROSBaseMsg> BaseMsg, bson_t** message)
{
	auto p = StaticCastSharedPtr<ROSMessages::geometry_msgs::PoseArray>(BaseMsg);

	*message = bson_new();

	UStdMsgsHeaderConverter::_bson_append_child_header(*message, "header", &(p->header));
	_bson_append_tarray<ROSMessages::geometry_msgs::Pose>(*message, "poses", p->poses, [](bson_t* msg, const char* key, const ROSMessages::geometry_msgs::Pose& pose)
		{
			UGeometryMsgsPoseConverter::_bson_append_child_pose(msg, key, &pose);
		});

	return true;
}

