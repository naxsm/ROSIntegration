// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ROSBaseSubActor.h"
#include "nav_msgs/Path.h"

#include "ROSSubPathActor.generated.h"


UCLASS()
class ROSINTEGRATION_API AROSSubPathActor : public AROSBaseSubActor
{
	GENERATED_BODY()
	
	ROS_MSG_TYPE(nav_msgs::Path)

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	bool showLines = true;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	FColor lineColor = FColor::Red;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	float lineThickness = 5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	FColor pointColor = FColor::White;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	bool showPoints = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	float pointSize = 15;


public:

	virtual void DisplayMessage(const UWorld* world, TSharedPtr<FROSBaseMsg> baseMsg, float DeltaTime) override;

};
