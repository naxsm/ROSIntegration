// Fill out your copyright notice in the Description page of Project Settings.


#include "viz/ROSTopicSubscriber.h"

#include "nav_msgs/Path.h"


void UROSTopicSubscriber::SubscribeCallback(TSharedPtr<FROSBaseMsg> msg)
{
	auto zz = reinterpret_cast<ROSMessages::nav_msgs::Path*>(msg.Get());
	//UE_LOG(LogTemp, Warning, TEXT("at [%d.%d] cb for %s [%s]"), zz->header.time._Sec, zz->header.time._NSec, *_topicName, *msg->_MessageType);

	_latestMsg = CopyMsg(msg);
	_gotNewData = true;
}

UROSTopicSubscriber::UROSTopicSubscriber()
	:_callback(std::bind(&UROSTopicSubscriber::SubscribeCallback, this, std::placeholders::_1)),
	_latestMsg(nullptr)
{}


void UROSTopicSubscriber::Init(AROSBridgeConnection* bridgeConnection, const FString& topicName, const FString& msgType,
	bool subscribe, bool advertise)
{
	_bridgeConnection = bridgeConnection;
	_topicName = topicName;
	if (bridgeConnection)
	{
		_topic = NewObject<ATopic>(ATopic::StaticClass());
		_topic->Init(_bridgeConnection->GetCore(), topicName, msgType, 100);
		if (subscribe)
			_topic->Subscribe(_callback);
		if (advertise)
			_topic->Advertise();
	}
}

UROSTopicSubscriber::~UROSTopicSubscriber()
{
	if (_topic)
		_topic->Unsubscribe();
}

TSharedPtr<FROSBaseMsg> UROSTopicSubscriber::GetLatestMsg()
{
	_gotNewData = false;
	return _latestMsg;
}

bool UROSTopicSubscriber::GotNewData() const
{
	return _gotNewData;
}

bool UROSTopicSubscriber::Publish(TSharedPtr<FROSBaseMsg> msg)
{
	return _topic->Publish(msg);
}
