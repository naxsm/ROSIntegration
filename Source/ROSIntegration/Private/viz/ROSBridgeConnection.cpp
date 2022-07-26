// Fill out your copyright notice in the Description page of Project Settings.


#include "viz/ROSBridgeConnection.h"

AROSBridgeConnection::AROSBridgeConnection()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AROSBridgeConnection::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("== beg == AROSBridgeConnection::BeginPlay()"));
	if (!ROSIntegrationCore)
		ROSIntegrationCore = NewObject<UROSIntegrationCore>(UROSIntegrationCore::StaticClass());
	bool conn = ROSIntegrationCore->Init(host, port, connType);
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("== end == AROSBridgeConnection::BeginPlay() %d"), conn);
}

void AROSBridgeConnection::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Warning, TEXT("== beg == AROSBridgeConnection::EndPlay()"));
	Super::EndPlay(EndPlayReason);
	if (ROSIntegrationCore)
		ROSIntegrationCore->ConditionalBeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("== end == AROSBridgeConnection::EndPlay()"));
}


