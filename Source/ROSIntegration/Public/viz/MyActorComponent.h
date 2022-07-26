// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerInput.h" 
#include "ROSBridgeConnection.h"
#include "ROSTopicSubscriber.h"

#include "MyActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROSINTEGRATION_API UMyActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyActorComponent();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ROS")
	AROSBridgeConnection* ROSBridgeConnection = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ROS")
	FString topicName = "/move_base_simple/goal";

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ROS")
	FString frameId = "map";

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	FTransform transform;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	FKey setGoalKey = EKeys::G;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:

	UPROPERTY()
	UROSTopicSubscriber* _subscriber;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool _leftMouseDownToggle = false;
	bool _leftMouseDown = false;
	FVector _hitBeg; 
	FVector _hitEnd;
	bool _begSet = false;
	bool _endSet = false;
	bool _setGoalMode = false;
	APlayerController* _controller;
	void OnLeftMousePress();
	void OnLeftMouseRelease();
	void OnSetGoalMode();
	bool PublishGoal();

};
