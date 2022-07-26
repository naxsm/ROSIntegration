// Fill out your copyright notice in the Description page of Project Settings.


#include "viz/ROSSubPathActor.h"

#include "DrawDebugHelpers.h" 


void AROSSubPathActor::DisplayMessage(const UWorld* world, TSharedPtr<FROSBaseMsg> baseMsg, float DeltaTime)
{
	auto msg = StaticCastSharedPtr<msg_t>(baseMsg);
	if (msg->poses.Num() < 1)
		return;
	auto it = msg->poses.begin();
	//UE_LOG(LogTemp, Warning, TEXT("Got path w/ %d poses [%s]"), msg->poses.Num(), *_subscriber->_topicName);
	//UE_LOG(LogTemp, Warning, TEXT("\tp0 = (%d,%d,%d)"), (*it).pose.position.x, (*it).pose.position.y, (*it).pose.position.z);
	ROSMessages::geometry_msgs::Pose prevPose = (*it).pose;

	FTransform t = transform;
	if (zActor)
	{
		auto loc = t.GetLocation();
		loc.Z = zActor->GetActorLocation().Z;
		t.SetLocation(loc);
	}

	if (showPoints)
	{
		auto pos = prevPose.position;
		FVector firstPoint = t.TransformPosition(FVector(pos.x, pos.y, pos.z));
		DrawDebugPoint(world, firstPoint, pointSize, pointColor, false, 1, depthPriority);
	}
	for (++it; it != msg->poses.end(); ++it)
	{
		auto startPos = prevPose.position;
		FVector lineStart = t.TransformPosition(FVector(startPos.x, startPos.y, startPos.z));
		auto position = (*it).pose.position;
		FVector lineEnd = t.TransformPosition(FVector(position.x, position.y, position.z));
		//DrawDebugLine(world, lineStart, lineEnd, color, !refreshEveryTick, -1, depthPriority, thickness);
		if (showLines)
			DrawDebugLine(world, lineStart, lineEnd, lineColor, !refreshEveryTick, 1, depthPriority, lineThickness);
		if (showPoints)
			DrawDebugPoint(world, lineEnd, pointSize, pointColor, false, 1, depthPriority);
		prevPose = (*it).pose;
	}
}
