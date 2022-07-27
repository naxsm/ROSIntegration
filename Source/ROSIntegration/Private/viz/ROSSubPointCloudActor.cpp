// Fill out your copyright notice in the Description page of Project Settings.

#include "viz/ROSSubPointCloudActor.h"

#include "DrawDebugHelpers.h" 
#include "Misc/Base64.h"


void AROSSubPointCloudActor::DisplayMessage(const UWorld* world, TSharedPtr<FROSBaseMsg> baseMsg, float DeltaTime)
{
	auto msg = StaticCastSharedPtr<msg_t>(baseMsg);
	size_t nPoints = msg->width * msg->height;
	check(msg->point_step == 12);
	//const float* points = reinterpret_cast<const float*>(msg->data_ptr);
	//const float* points = reinterpret_cast<const float*>(msg->my_ptr);


	/*
	TArray<uint8> decoded;
	FBase64::Decode(msg->str_data, decoded);
	auto data_arr = decoded;// .GetCharArray();
	const float* points = reinterpret_cast<const float*>(data_arr.GetData());
	*/
	float* points = reinterpret_cast<float*>(msg->data);

	//UE_LOG(LogTemp, Warning, TEXT("arr len = %d / %d / %d"), data_arr.Num(), nPoints, msg->str_data.Len());


	for (int i = 0; i < nPoints; ++i)
	{
		int i3 = i * 3;
		//DrawDebugPoint(world, transform.TransformPosition(FVector(points[i3], points[i3 + 1], points[i3 + 2])),
		//	pointSize, pointColor, !refreshEveryTick, -1, depthPriority);

		auto z = points[i3 + 2];
		FColor color = pointColor;
		if (rainbow)
		{
			if (refreshIntensities)
			{
				minIntensity = std::min(z, minIntensity);
				maxIntensity = std::max(z, maxIntensity);
			}
			else
			{
				if (minIntensity > z)
				{
					minIntensity = z;
				}
				else if (maxIntensity < z)
				{
					maxIntensity = z;
				}
			}
			color = FColor::MakeRedToGreenColorFromScalar((z - minIntensity) / (maxIntensity - minIntensity));
		}

		DrawDebugPoint(world, transform.TransformPosition(FVector(points[i3], points[i3 + 1], points[i3 + 2])),
			pointSize, color, false, 1, depthPriority);
	}
}
