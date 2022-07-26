// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RI/Topic.h"
#include "ROSBridgeConnection.h"

#include "ROSTopicSubscriber.generated.h"


UCLASS()
class ROSINTEGRATION_API UROSTopicSubscriber : public UObject
{
	GENERATED_BODY()

public:
	UROSTopicSubscriber();
	~UROSTopicSubscriber();

	void Init(AROSBridgeConnection* bridgeConnection, const FString& topicName, const FString& msgType, bool subscribe = true, bool advertise = false);

	TSharedPtr<FROSBaseMsg> GetLatestMsg();

	bool GotNewData() const;
	bool Publish(TSharedPtr<FROSBaseMsg> msg);

	UPROPERTY()
	FString _topicName;

protected:
	virtual TSharedPtr<FROSBaseMsg> CopyMsg(TSharedPtr<FROSBaseMsg> msg)
	{
		return msg;
	}

private:
	UPROPERTY()
	AROSBridgeConnection* _bridgeConnection;

	UPROPERTY()
	UTopic* _topic;

	bool _gotNewData = false;

	void SubscribeCallback(TSharedPtr<FROSBaseMsg> msg);
	std::function<void(TSharedPtr<FROSBaseMsg> msg)> _callback;

	TSharedPtr<FROSBaseMsg> _latestMsg;

};
