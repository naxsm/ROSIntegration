// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ROSBaseSubActor.h"
#include "geometry_msgs/PoseArray.h"

#include "ROSSubPoseArrayActor.generated.h"

UCLASS()
class ROSINTEGRATION_API AROSSubPoseArrayActor : public AROSBaseSubActor
{
	GENERATED_BODY()

	ROS_MSG_TYPE(geometry_msgs::PoseArray)

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	FColor color = FColor::Red;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	float arrowLength = 200;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	float arrowSize = 300;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	float thickness = 8;

public:
	
	virtual void DisplayMessage(const UWorld* world, TSharedPtr<FROSBaseMsg> baseMsg, float DeltaTime) override;

};
