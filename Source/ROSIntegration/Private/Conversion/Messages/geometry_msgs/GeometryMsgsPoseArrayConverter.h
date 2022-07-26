#pragma once
#include <CoreMinimal.h>
#include <UObject/ObjectMacros.h>
#include <UObject/Object.h>
#include "Conversion/Messages/BaseMessageConverter.h"
#include "Conversion/Messages/std_msgs/StdMsgsHeaderConverter.h"
#include "Conversion/Messages/geometry_msgs/GeometryMsgsPoseConverter.h"
#include "geometry_msgs/PoseArray.h"

#include "GeometryMsgsPoseArrayConverter.generated.h"

UCLASS()
class ROSINTEGRATION_API UGeometryMsgsPoseArrayConverter : public UBaseMessageConverter
{
	GENERATED_UCLASS_BODY()

public:

	virtual bool ConvertIncomingMessage(const ROSBridgePublishMsg* message, TSharedPtr<FROSBaseMsg>& BaseMsg);

	virtual bool ConvertOutgoingMessage(TSharedPtr<FROSBaseMsg> BaseMsg, bson_t** message);

	static bool _bson_extract_child_pose_array(bson_t* b, FString key, ROSMessages::geometry_msgs::PoseArray* p, bool LogOnErrors = true)
	{
		bool KeyFound = false;
		KeyFound = UStdMsgsHeaderConverter::_bson_extract_child_header(b, TEXT("msg.header"), &p->header);
		if (!KeyFound) return false;

		p->poses = GetTArrayFromBSON<ROSMessages::geometry_msgs::Pose>(key + ".poses", b, KeyFound, [LogOnErrors](FString subKey, bson_t* subMsg, bool& subKeyFound)
			{
				ROSMessages::geometry_msgs::Pose ret;
				subKeyFound = UGeometryMsgsPoseConverter::_bson_extract_child_pose(subMsg, subKey, &ret, LogOnErrors);
				return ret;
			}, LogOnErrors);
		if (!KeyFound) return false;

		return true;
	}

	static void _bson_append_child_pose_array(bson_t* b, const char* key, const ROSMessages::geometry_msgs::PoseArray* p)
	{
		bson_t child;
		BSON_APPEND_DOCUMENT_BEGIN(b, key, &child);
		_bson_append_pose_array(&child, p);
		bson_append_document_end(b, &child);
	}

	static void _bson_append_pose_array(bson_t* b, const ROSMessages::geometry_msgs::PoseArray* p)
	{
		UStdMsgsHeaderConverter::_bson_append_child_header(b, "header", &(p->header));
		_bson_append_tarray<ROSMessages::geometry_msgs::Pose>(b, "poses", p->poses, [](bson_t* msg, const char* key, const ROSMessages::geometry_msgs::Pose& pose)
			{
				UGeometryMsgsPoseConverter::_bson_append_child_pose(msg, key, &pose);
			});
	}

};

