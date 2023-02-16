#pragma once

#include <CoreMinimal.h>

//USTRUCT()
struct ROSINTEGRATION_API FROSBaseMsg {
	//GENERATED_BODY()

public:
	FROSBaseMsg() = default;
	~FROSBaseMsg() = default;

	FString _MessageType;
	bool _ros2;
};
