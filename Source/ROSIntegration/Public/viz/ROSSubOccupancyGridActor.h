// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ROSBaseSubActor.h"
#include "nav_msgs/OccupancyGrid.h"

#include "ROSSubOccupancyGridActor.generated.h"

UCLASS()
class ROSINTEGRATION_API AROSSubOccupancyGridActor : public AROSBaseSubActor
{
	GENERATED_BODY()
	
	ROS_MSG_TYPE(nav_msgs::OccupancyGrid)

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	FColor minColor = FColor::Silver;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	FColor maxColor = FColor::Black;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	float boxThickness = 15;

public:

	virtual void DisplayMessage(const UWorld* world, TSharedPtr<FROSBaseMsg> baseMsg, float DeltaTime) override;

};
