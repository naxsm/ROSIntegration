#pragma once

#include <functional>
#include <memory>
#include <CoreMinimal.h>
#include <UObject/ObjectMacros.h>
#include <UObject/Object.h>
#include "ROSBaseMsg.h"
#include "ROSBPMsg.h"
#include "ROSIntegrationCore.h"
#include "viz/ROSBridgeConnection.h"

#include "Topic.generated.h"

/**
* @ingroup ROS Message Types
* Which Message type to work with.
*/
UENUM(BlueprintType, Category = "ROS")
enum class EMessageType : uint8
{
	String = 0,
	Float32 = 1,
};

UCLASS(Blueprintable, BlueprintType, Placeable, meta = (BlueprintSpawnableComponent))
class ROSINTEGRATION_API ATopic: public AActor
{
	GENERATED_UCLASS_BODY()

public:
	virtual void BeginPlay() override;

	bool Subscribe(std::function<void(TSharedPtr<FROSBaseMsg>)> func);

	bool Unsubscribe();

	bool Advertise();

	bool Unadvertise();

	bool Publish(TSharedPtr<FROSBaseMsg> msg);

	void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "ROS|Topic")
	void BaseInit(AROSBridgeConnection* conn, FString Topic, FString MessageType, int32 QueueSize = 10);

	void Init(UROSIntegrationCore* Ric, FString Topic, FString MessageType, int32 QueueSize = 10);

	virtual void PostInitProperties() override;

	void MarkAsDisconnected();
	bool Reconnect(UROSIntegrationCore* ROSIntegrationCore);
	
	bool IsAdvertising();

protected:

	virtual FString GetDetailedInfoInternal() const override;

	UFUNCTION(BlueprintImplementableEvent, Category = ROS)
	void OnConstruct();

	UFUNCTION(BlueprintImplementableEvent, Category = ROS)
	void OnStringMessage(const FString& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = ROS)
	void OnFloat32Message(const float& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = ROS)
	void OnROSMessage(const UROSBPMsg* message);

	UPROPERTY()
	UROSIntegrationCore* _ROSIntegrationCore = nullptr;

public:
	UPROPERTY(EditAnywhere, Category = "ROS")
	AROSBridgeConnection* _bridgeConnection;
	UPROPERTY(EditAnywhere, Category = "ROS")
	FString _topicName;
	UPROPERTY(EditAnywhere, Category = "ROS")
	FString _messageType = "std_msgs/String";
	UPROPERTY(EditAnywhere, Category = "ROS")
	int32 _queueSize = 1;

private:

	struct State
	{
		bool Connected;
		bool Advertised;
		bool Subscribed;
		bool Blueprint;
		EMessageType BlueprintMessageType;
	} _State;


	UFUNCTION(BlueprintCallable, Category = "ROS|Topic")
	void Init(const FString& TopicName, EMessageType MessageType, int32 QueueSize = 1);

	/**
	 * Subscribe to the given topic
	 */
	UFUNCTION(BlueprintCallable, Category = "ROS|Topic")
	bool Subscribe();

	UFUNCTION(BlueprintCallable, Category = "ROS|Topic")
	bool PublishStringMessage(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "ROS|Topic")
	bool PublishROSMessage(UROSBPMsg* message);

	// Helper to keep track of self-destruction for async functions
	TSharedPtr<ATopic, ESPMode::ThreadSafe> _SelfPtr;

	// PIMPL
	class Impl;
	Impl *_Implementation = nullptr;
};
