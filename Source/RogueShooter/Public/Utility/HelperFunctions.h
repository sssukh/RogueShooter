#pragma once

#include "CoreMinimal.h"


inline bool ContainsFText(const TArray<FText>& Array, const FText& Item)
{
	FString ItemStr = Item.ToString();

	for (const FText& Elem : Array)
	{
		if (Elem.ToString() == ItemStr)
		{
			return true;
		}
	}
	return false;
}