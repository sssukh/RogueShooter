#pragma once

#include "CoreMinimal.h"

#include "FlowControlLibrary.generated.h"


USTRUCT(BlueprintType)
struct FDoOnce
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool bDoOnce;
	
	FORCEINLINE FDoOnce();
	explicit FORCEINLINE FDoOnce(bool bStartClosed);
	
	FORCEINLINE void Reset() { bDoOnce = true; }

	FORCEINLINE bool Execute()
	{
		if (bDoOnce)
		{
			bDoOnce = false;
			return true;
		}
		return false;
	}
};

FORCEINLINE FDoOnce::FDoOnce() : bDoOnce(true)
{
}

FORCEINLINE FDoOnce::FDoOnce(const bool bStartClosed) : bDoOnce(!bStartClosed)
{
}


USTRUCT(BlueprintType)
struct FGate
{
	GENERATED_BODY()

public:
	FORCEINLINE FGate();

	explicit FORCEINLINE FGate(bool bStartClosed);
	
	FORCEINLINE void Open() { bGateOpen = true; }

	FORCEINLINE void Close() { bGateOpen = false; }

	FORCEINLINE void Toggle() { bGateOpen = !bGateOpen; }

	FORCEINLINE bool IsOpen() const { return bGateOpen; }

private:
	UPROPERTY(VisibleAnywhere)
	bool bGateOpen;
};

FORCEINLINE FGate::FGate() : bGateOpen(true)
{
}

FORCEINLINE FGate::FGate(const bool bStartClosed) : bGateOpen(!bStartClosed)
{
}

USTRUCT(BlueprintType)
struct FRetriggerableTimer
{
	GENERATED_BODY()

public:
	FORCEINLINE FRetriggerableTimer() {}

	FORCEINLINE FRetriggerableTimer(UObject* Object,FName FunctionName) {SetDelegate(Object,FunctionName);}
	
	FORCEINLINE void SetTimerWithDelegate(const UWorld* World, float Time, bool bLoop)
	{
		if(!World)
		{
			// 메시지 
			return;
		}
		World->GetTimerManager().ClearTimer(TimerHandle);
		World->GetTimerManager().SetTimer(TimerHandle, TimeDelegate, Time, bLoop);
	}

	FORCEINLINE void SetDelegate(UObject* Object,FName FunctionName)
	{
		TimeDelegate.BindUFunction(Object,FunctionName);
	}
private:
	UPROPERTY(VisibleAnywhere)
	FTimerHandle TimerHandle;

	FTimerDelegate TimeDelegate;
};