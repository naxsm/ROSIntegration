// Fill out your copyright notice in the Description page of Project Settings.

#include "viz/ROSSubPoseArrayActor.h"

#include "DrawDebugHelpers.h"


void AROSSubPoseArrayActor::DisplayMessage(const UWorld* world, TSharedPtr<FROSBaseMsg> baseMsg, float DeltaTime)
{
	auto msg = StaticCastSharedPtr<msg_t>(baseMsg);

	FTransform t = transform;
	if (zActor)
	{
		auto loc = t.GetLocation();
		loc.Z = zActor->GetActorLocation().Z;
		t.SetLocation(loc);
	}

	for (auto it = msg->poses.begin(); it != msg->poses.end(); ++it)
	{
		auto position = (*it).position;
		FVector lineStart = t.TransformPosition(FVector(position.x, position.y, position.z));
		auto orientation = (*it).orientation;
		FVector lineDir = t.TransformVector(FQuat(orientation.x, orientation.y, orientation.z, orientation.w).Vector());
		lineDir.Normalize();
		FVector lineEnd = lineStart + lineDir * arrowLength;
		DrawDebugDirectionalArrow(world, lineStart, lineEnd,
		//	arrowSize, color, !refreshEveryTick, -1, depthPriority, thickness);
			arrowSize, color, !refreshEveryTick, 1, depthPriority, thickness);
	}
}
