// Fill out your copyright notice in the Description page of Project Settings.

#include "viz/ROSSubOccupancyGridActor.h"


//#include "Math/Color.h"
#include "DrawDebugHelpers.h" 
#include "Misc/Base64.h"



void AROSSubOccupancyGridActor::DisplayMessage(const UWorld* world, TSharedPtr<FROSBaseMsg> baseMsg, float DeltaTime)
{
	static int minw = TNumericLimits<int>::Max();
	static int maxw = TNumericLimits<int>::Min();
	auto msg = StaticCastSharedPtr<msg_t>(baseMsg);
	ROSMessages::nav_msgs::MapMetaData info = msg->info;

	ROSMessages::geometry_msgs::Pose origin = info.origin;
	auto position = origin.position;
	FVector ogCenter(position.x, position.y, position.z);
	auto orientation = origin.orientation;
	FQuat rotation(orientation.x, orientation.y, orientation.z, orientation.w);
	float resolution = info.resolution;
	FVector extent(100 * resolution, 100 * resolution, boxThickness);

	//UE_LOG(LogTemp, Warning, TEXT("\t\t[[ROT: [%s]]]"), *rotation.ToString());

	uint32 width = info.width;
	uint32 height = info.height;
	//auto data = msg->data_ptr;
	//auto data = msg->my_ptr;
	//auto data = msg->data;
	/*
	TArray<uint8> decoded;
	FBase64::Decode(msg->str_data, decoded);
	auto data = decoded;
	*/
	// const float* points = reinterpret_cast<const float*>(data_arr.GetData());
	//const int8* data = reinterpret_cast<const int8*>(msg->data_ptr);

	auto data = msg->data;

	//UE_LOG(LogTemp, Warning, TEXT("arr len = %d / %d / [%d x %d]"), width*height, data, width, height);

	if (data)
	//if (data.Num() > 0)
		{
		FTransform t = transform;
		t.ConcatenateRotation(rotation);
		if (zActor)
		{
			auto loc = t.GetLocation();
			loc.Z = zActor->GetActorLocation().Z;
			t.SetLocation(loc);
		}
		for (uint32 j = 0; j < height; ++j)
			for (uint32 i = 0; i < width; ++i)
			{
				auto ix = i + j * width;
				if (data[ix] > 5)
				{
					FVector center = t.TransformPosition(ogCenter + FVector(i * resolution, j * resolution, 0));
					FColor color = FLinearColor::LerpUsingHSV(minColor, maxColor, static_cast<float>(data[ix]) / 100.0).Quantize();
					//DrawDebugSolidBox(world, center, extent, rotation, color, !refreshEveryTick, -1, depthPriority);
					DrawDebugPoint(world, center, boxThickness*resolution, color, !refreshEveryTick, 1, depthPriority);
				}
				if (data[ix] < minw)
				{
					minw = data[ix];
					UE_LOG(LogTemp, Warning, TEXT("\t**NEW MIN: %d"), minw);
				}
				if (data[ix] > maxw)
				{
					maxw = data[ix];
					UE_LOG(LogTemp, Warning, TEXT("\t**NEW MAX: %d"), maxw);
				}
			}
	}
	
}
