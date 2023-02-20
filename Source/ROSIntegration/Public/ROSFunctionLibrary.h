// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ROSBPMsg.h"
#include "ROSFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ROSINTEGRATION_API UROSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//UROSFunctionLibrary()
	//: rosMsg(NewObject<UROSBPMsg>())
	//{}

	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast), Category = ROS)
	static UROSBPMsg* Conv_boolToROSBPMsg(bool value)
	{
		UROSBPMsg* rosMsg = NewObject<UROSBPMsg>();
		rosMsg->msg = MakeShareable(new ROSMessages::std_msgs::Bool(value));
		return rosMsg;
	}

	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast), Category = ROS)
	static UROSBPMsg* Conv_integerToROSBPMsg(int32 value)
	{
		UROSBPMsg* rosMsg = NewObject<UROSBPMsg>();
		rosMsg->msg = MakeShareable(new ROSMessages::std_msgs::Int32(value));
		return rosMsg;
	}

};
