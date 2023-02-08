// Fill out your copyright notice in the Description page of Project Settings.


#include "viz/ROSBridgeConnection.h"

AROSBridgeConnection::AROSBridgeConnection()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AROSBridgeConnection::BeginPlay()
{
	if (!ROSIntegrationCore)
		ROSIntegrationCore = NewObject<UROSIntegrationCore>(UROSIntegrationCore::StaticClass());
	bool conn = ROSIntegrationCore->Init(host, port, connType);
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


