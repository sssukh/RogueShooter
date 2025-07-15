#pragma once

#include "CoreMinimal.h"


ROGUESHOOTER_API DECLARE_LOG_CATEGORY_EXTERN(RS_LOG, Log, All); //추가

#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define LOG_S(Verbosity) UE_LOG(RS_LOG, Verbosity, TEXT("%s"), *LOG_CALLINFO)

#define LOG(Verbosity, Format, ...) UE_LOG(RS_LOG, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define RS_LOG(Verbosity, Format, ...) UE_LOG(RS_LOG, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define RS_LOG_WARNING(Format, ...) UE_LOG(RS_LOG, Warning, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define RS_LOG_ERROR(Format, ...) UE_LOG(RS_LOG, Error, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define	RS_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(Format, ##__VA_ARGS__))
#define	RS_SCREEN_T(Time, Format, ...) GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))


#define RS_LOG_SCREEN(Format, ...) \
{ \
GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(Format, ##__VA_ARGS__)); \
RS_LOG_WARNING(Format, ##__VA_ARGS__); \
}


#define RS_LOG_SCREEN_ERROR(Format, ...) \
{ \
GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(Format, ##__VA_ARGS__)); \
RS_LOG_ERROR(Format, ##__VA_ARGS__); \
}


#define RS_CHECK(Expr, ...) {if(!(Expr)) {LOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}
// #define RS_IF_CHECK(Expr, ErrorMsg, ...) {if(!(Expr)) {RS_LOG_SCREEN_ERROR(ErrorMsg); return __VA_ARGS__;}}


#define RS_IF_CHECK_RETURN(Expr, ErrorMsg, ReturnValue, ...) \
{ \
if (!(Expr)) \
{ \
RS_LOG_SCREEN_ERROR(ErrorMsg, ##__VA_ARGS__); \
return ReturnValue; \
} \
}


#define RS_IF_CHECK_VOID(Expr, ErrorMsg, ...) \
{ \
if (!(Expr)) \
{ \
RS_LOG_SCREEN_ERROR(ErrorMsg, ##__VA_ARGS__); \
return; \
} \
}
