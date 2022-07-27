// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ROSBridgeConnection.h"
#include "ROSTopicSubscriber.h"

#include "ROSBaseSubActor.generated.h"


#define ROS_MSG_TYPE(T)\
	private:\
		typedef ROSMessages::##T msg_t;\
		const msg_t static_msg = msg_t();\
	public:\
		virtual const FROSBaseMsg* GetMsgType() override\
			{ return &static_msg; }


UCLASS()
class ROSINTEGRATION_API AROSBaseSubActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ROS")
	AROSBridgeConnection* ROSBridgeConnection = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ROS")
	FString topicName = "";

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	FTransform transform = FTransform(FRotator(), FVector(), FVector(100, -100, 100));

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	AActor* zActor = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	bool refreshEveryTick = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Viz")
	int depthPriority = 1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "debug")
	bool subAndDraw = true; // debug: don't do anything if false


protected:

	UPROPERTY()
	UROSTopicSubscriber* _subscriber;


public:	

	AROSBaseSubActor();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual const FROSBaseMsg* GetMsgType()
	{
		check(0 && "AROSBaseSubActor::GetMsgType must be overridden in every subclass; please use the ROS_MSG_TYPE macro");
		return nullptr;
	}

	virtual void DisplayMessage(const UWorld* world, TSharedPtr<FROSBaseMsg> baseMsg, float DeltaTime)
	{
		check(0 && "AROSBaseSubActor::DisplayMessage must be overridden in every subclass");
	}

};
