// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <LicSecure.hpp>
#include "MyGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class BESTGAME2_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init()      override;
	virtual void Shutdown()  override;

	PROTECT_METHOD_DECL(void, ProtectedFunction, ())
};
