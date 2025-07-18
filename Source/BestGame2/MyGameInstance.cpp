// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

void UMyGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Log, TEXT("UTSGameInstance::Init — Hello!"));

    LIC_GUARD_MAIN();

    ProtectedFunction();
}

void UMyGameInstance::Shutdown()
{
    UE_LOG(LogTemp, Log, TEXT("UTSGameInstance::Shutdown"));
    Super::Shutdown();
}

PROTECT_METHOD_IMPL(UMyGameInstance, void, ProtectedFunction, (), (),
{
    UE_LOG(LogTemp, Log, TEXT("UTSGameInstance::ProtectedFunction — Hello!"));
})

/*
void UMyGameInstance::ProtectedFunction()
{
    UE_LOG(LogTemp, Log, TEXT("UTSGameInstance::ProtectedFunction — Hello!"));
}
*/