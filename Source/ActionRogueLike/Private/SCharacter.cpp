// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArmComponent);	

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add input mapping context to the local player
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				Subsystem->AddMappingContext(InputMapping.LoadSynchronous(),0);
			}
		}
	}
	
}

void ASCharacter::MoveForward(const FInputActionInstance& Instance)
{
	float FloatValue = Instance.GetValue().Get<float>();
	AddMovementInput(GetActorForwardVector(), FloatValue);
}

void ASCharacter::Turn(const FInputActionInstance& Instance)
{
	float FloatValue = Instance.GetValue().Get<float>();
	AddControllerYawInput(FloatValue);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered,this,&ASCharacter::MoveForward);
	EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered,this, &ASCharacter::Turn);

}

