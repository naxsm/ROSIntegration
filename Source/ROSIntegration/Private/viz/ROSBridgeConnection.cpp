// Fill out your copyright notice in the Description page of Project Settings.


#include "ROSIntegrationGameInstance.h"


AROSBridgeConnection::AROSBridgeConnection()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AROSBridgeConnection::BeginPlay()
{
	if (!connected)
		Init();
	Super::BeginPlay();
}


void AROSBridgeConnection::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogROS, Warning, TEXT("AROSBridgeConnection::EndPlay0"));
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogROS, Warning, TEXT("AROSBridgeConnection::EndPlay1"));

	for (TObjectIterator<ATopic> It; It; ++It)
	{
		ATopic* Topic = *It;
		Topic->Unadvertise(); // to make sure all topics are unadvertised on ROS side
		Topic->Unsubscribe(); // to prevent messages arriving during shutdown from triggering subscription callbacks
		Topic->MarkAsDisconnected();
	}

	if (ROSIntegrationCore)
	{
		ROSIntegrationCore->ConditionalBeginDestroy();
		ROSIntegrationCore = nullptr;
	}
	
}

void AROSBridgeConnection::Init()
{
	if (!ROSIntegrationCore)
	{
		ROSIntegrationCore = NewObject<UROSIntegrationCore>(UROSIntegrationCore::StaticClass());
		connected = ROSIntegrationCore->Init(host, port, connType);
	}
}


