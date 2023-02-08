#pragma once

#include <CoreMinimal.h>

#include "ROSBaseMsg.h"
#include "ROSBPMsg.generated.h"

UCLASS()
class ROSINTEGRATION_API UROSBPMsg : public UObject
{
	GENERATED_BODY()
public:
	
	UROSBPMsg()
		: msg(nullptr)
	{}
	
	~UROSBPMsg()
	{}

	UROSBPMsg(TSharedPtr<FROSBaseMsg> m)
		:msg(m)
	{}

	TSharedPtr<FROSBaseMsg> msg;
};