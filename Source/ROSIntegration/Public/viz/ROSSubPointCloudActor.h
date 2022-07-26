// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ROSBaseSubActor.h"
#include "sensor_msgs/PointCloud2.h"

#include "ROSSubPointCloudActor.generated.h"


UCLASS()
class ROSINTEGRATION_API AROSSubPointCloudActor : public AROSBaseSubActor
{
	GENERATED_BODY()

	ROS_MSG_TYPE(sensor_msgs::PointCloud2)

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	FColor pointColor = FColor::Red;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	bool rainbow = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	bool refreshIntensities = true;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	float pointSize = 3;

public:

	virtual void DisplayMessage(const UWorld* world, TSharedPtr<FROSBaseMsg> baseMsg, float DeltaTime) override;

private:
	float minIntensity = TNumericLimits<float>::Max();
	float maxIntensity = TNumericLimits<float>::Min();
};
