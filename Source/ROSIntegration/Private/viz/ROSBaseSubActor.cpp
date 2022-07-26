// Fill out your copyright notice in the Description page of Project Settings.


#include "viz/ROSBaseSubActor.h"

AROSBaseSubActor::AROSBaseSubActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = subAndDraw;

}

void AROSBaseSubActor::BeginPlay()
{
	Super::BeginPlay();
	if (subAndDraw)
	{
		_subscriber = NewObject<UROSTopicSubscriber>(UROSTopicSubscriber::StaticClass());
		_subscriber->Init(ROSBridgeConnection, topicName, GetMsgType()->_MessageType);
	}
}

void AROSBaseSubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//bool newData = _subscriber->GotNewData();
	//UE_LOG(LogTemp, Warning, TEXT("tick for %s, [%d,%d]"), *_subscriber->_topicName, refreshEveryTick, newData);
	//if (subAndDraw && (refreshEveryTick || newData))
	if (subAndDraw && (refreshEveryTick || _subscriber->GotNewData()))
		{
		auto msg = _subscriber->GetLatestMsg();
		bool valid = msg.IsValid();
		// UE_LOG(LogTemp, Warning, TEXT("\tmsg valid: %d"), valid);
		if (valid)
			DisplayMessage(GetWorld(), msg, DeltaTime);
	}
}
