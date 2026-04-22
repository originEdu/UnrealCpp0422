// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPC.h"
#include "EnhancedInputSubSystems.h"
#include "InputMappingContext.h"

void ABasicPC::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (IsLocalPlayerController())
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->ClearAllMappings();

			for (const auto& IMC : IMC_Basics)
			{
				Subsystem->AddMappingContext(IMC.LoadSynchronous(), 0);
			}
		}
	}
}

void ABasicPC::OnUnPossess()
{
	Super::OnUnPossess();

	if (IsLocalPlayerController())
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->ClearAllMappings();
		}
	}
}
