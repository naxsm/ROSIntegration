#pragma once

#include <CoreMinimal.h>

#include "ROSBaseMsg.h"
#include "ROSBPMsg.generated.h"


UCLASS()
class ROSINTEGRATION_API UROSBPMsg : public UObject
{
	GENERATED_BODY()
public:
	
	UROSBPMsg()
		: msg(nullptr)
	{}
	
	~UROSBPMsg()
	{}

	UROSBPMsg(TSharedPtr<FROSBaseMsg> m)
		:msg(m)
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

	//UPROPERTY()
	//FROSBaseMsg* msg;
	TSharedPtr<FROSBaseMsg> msg;
};