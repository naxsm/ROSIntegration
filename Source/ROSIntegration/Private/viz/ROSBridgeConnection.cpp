// Fill out your copyright notice in the Description page of Project Settings.


#include "viz/ROSBridgeConnection.h"

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
	Super::EndPlay(EndPlayReason);
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


