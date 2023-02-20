#pragma once

#include <CoreMinimal.h>

#include "ROSBaseMsg.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt8.h"

#include "ROSBPMsg.generated.h"



UCLASS(Blueprintable, BlueprintType)
class ROSINTEGRATION_API UROSBPMsg : public UObject
{
	GENERATED_BODY()
public:
	
	UROSBPMsg()
		:msg(nullptr)
	{}
	
	UROSBPMsg(TSharedPtr<FROSBaseMsg> m)
		:msg(m)
	{}

	~UROSBPMsg()
	{}

	UFUNCTION(BlueprintCallable, Category = ROS)
	void castToBool(bool& value, bool& success);

	UFUNCTION(BlueprintCallable, Category = ROS)
	void castToFloat32(float& value, bool& success);

	UFUNCTION(BlueprintCallable, Category = ROS)
	void castToInt32(int32& value, bool& success);

	UFUNCTION(BlueprintCallable, Category = ROS)
	void castToString(FString& value, bool& success);

	UFUNCTION(BlueprintCallable, Category = ROS)
	void castToUInt8(uint8& value, bool& success);

	uint8 zzz[1024 * 256]; // TODO FIXME: remove this, it's only there to show a mem leak
	TSharedPtr<FROSBaseMsg> msg;
};

