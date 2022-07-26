// Fill out your copyright notice in the Description page of Project Settings.


#include "viz/MyActorComponent.h"

#include "geometry_msgs/PoseStamped.h"

#include "DrawDebugHelpers.h" 



// Sets default values for this component's properties
UMyActorComponent::UMyActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	transform.SetScale3D(FVector(0.01, -0.01, 0.01));
}


// Called when the game starts
void UMyActorComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("== beg == UMyActorComponent::BeginPlay()"));

	auto owner = GetOwner();
	_controller = owner->GetWorld()->GetFirstPlayerController();
	_controller->DefaultMouseCursor = EMouseCursor::Crosshairs;
	_controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	_controller->bShowMouseCursor = false;

	FName action_name("LeftMousePress");
	FInputActionKeyMapping action(action_name, EKeys::LeftMouseButton, false, false, false, false);

	_controller->PlayerInput->AddActionMapping(action);
	_controller->InputComponent->
		BindAction(action_name, IE_Pressed, this, &UMyActorComponent::OnLeftMousePress);

	action_name = "LeftMouseRelease";
	action = FInputActionKeyMapping(action_name, EKeys::LeftMouseButton, false, false, false, false);
	_controller->PlayerInput->AddActionMapping(action);
	_controller->InputComponent->
		BindAction(action_name, IE_Released, this, &UMyActorComponent::OnLeftMouseRelease);

	action_name = "SetGoalMode";
	action = FInputActionKeyMapping(action_name, setGoalKey, false, false, false, false);
	_controller->PlayerInput->AddActionMapping(action);
	_controller->InputComponent->
		BindAction(action_name, IE_Pressed, this, &UMyActorComponent::OnSetGoalMode);

	_subscriber = NewObject<UROSTopicSubscriber>(UROSTopicSubscriber::StaticClass());
	_subscriber->Init(ROSBridgeConnection, topicName, "geometry_msgs/PoseStamped", false, true);

	/*
	TArray<USceneComponent*> chs;
	button->GetChildrenComponents(true, chs);
	
	if (button)
		UE_LOG(LogTemp, Warning, TEXT("button? %s"), *button->GetDetailedInfo());
		*/
	UE_LOG(LogTemp, Warning, TEXT("== end == UMyActorComponent::BeginPlay()"));
}


// Called every frame
void UMyActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// if (!_setGoalMode)
	//	return;

	//float mouseX, mouseY;
	//_controller->GetMousePosition(mouseX, mouseY);
	/*
	if (_leftMouseDown)
	{
		FHitResult hitResult;
		_controller->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hitResult);
		if (_leftMouseDownToggle)
			_hitBeg = hitResult.ImpactPoint;
		else
			_hitEnd = hitResult.ImpactPoint;
	}
	*/
	/*
	if (_leftMouseDown)
		if (_leftMouseDownToggle)
			_hitBeg = hitResult.ImpactPoint;
		else
			_hitEnd = hitResult.ImpactPoint;
	*/

	if (_begSet)
	{
		if (!_endSet)
		{
			FHitResult hitResult;
			_controller->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hitResult);
			_hitEnd = hitResult.ImpactPoint;
		}
		DrawDebugDirectionalArrow(GetWorld(), _hitBeg, _hitEnd,
			500, FColor::Blue, true, 1, 1, 16);
	}


	// if (_leftMouseDown)
	// 	UE_LOG(LogTemp, Warning, TEXT("... (%d,%d) (%f,%f) %s"), _leftMouseDownToggle, _leftMouseDown, mouseX, mouseY, *hitResult.ToString());
	/*
	DrawDebugPoint(world, transform.TransformPosition(FVector(points[i3], points[i3 + 1], points[i3 + 2])),
		pointSize, color, false, 1, depthPriority);
		*/

	// ...

	_controller->bShowMouseCursor = _setGoalMode;

}

void UMyActorComponent::OnLeftMousePress()
{
	_leftMouseDownToggle = !_leftMouseDownToggle;
	//_controller->bShowMouseCursor = _leftMouseDownToggle;
	_leftMouseDown = true;
	if (_setGoalMode)
	{
		FHitResult hitResult;
		_controller->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hitResult);
		if (!_begSet)
		{
			_hitBeg = hitResult.ImpactPoint;
			_begSet = true;
		}
		else if (!_endSet)
		{
			_hitEnd = hitResult.ImpactPoint;
			_endSet = true;
			_setGoalMode = false;
			PublishGoal();
		}
	}
}

void UMyActorComponent::OnLeftMouseRelease()
{
	_leftMouseDown = false;
}

void UMyActorComponent::OnSetGoalMode()
{
	_setGoalMode = !_setGoalMode;
	_leftMouseDownToggle = false;
	_begSet = false;
	_endSet = false;
}

bool UMyActorComponent::PublishGoal()
{
	TSharedPtr<ROSMessages::geometry_msgs::PoseStamped> msg = MakeShareable(new ROSMessages::geometry_msgs::PoseStamped);
	msg->header.seq = 0;
	msg->header.time = FROSTime::Now();
	msg->header.frame_id = frameId;
	msg->header._ros2 = true;
	auto hitBeg = transform.TransformPosition(_hitBeg);
	auto hitEnd = transform.TransformPosition(_hitEnd);
	hitBeg.Z = 0;
	hitEnd.Z = 0;
	msg->pose.position = hitBeg;
	msg->pose.orientation = FQuat::FindBetween(FVector(1, 0, 0), hitEnd - hitBeg);
	return _subscriber->Publish(msg);
}

