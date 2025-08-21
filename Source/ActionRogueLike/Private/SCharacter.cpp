// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	bUseControllerRotationYaw = false;
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
	EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered,this, &ASCharacter::LookUp);
	EnhancedInputComponent->BindAction(LookRightAction, ETriggerEvent::Triggered,this, &ASCharacter::LookRight);
	EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered,this, &ASCharacter::MoveRight);
	EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASCharacter::Jump);
}

void ASCharacter::MoveForward(const FInputActionInstance& Instance)
{
	float FloatValue = Instance.GetValue().Get<float>();
	
	// We want to move character towards camera forward vector
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	
	AddMovementInput(ControlRotation.Vector(), FloatValue);
}

void ASCharacter::MoveRight(const FInputActionInstance& Instance)
{
	float FloatValue = Instance.GetValue().Get<float>();

	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	
	FVector RightVectorFromControlRotation = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVectorFromControlRotation, FloatValue);
}

void ASCharacter::LookUp(const FInputActionInstance& Instance)
{
	float FloatValue = Instance.GetValue().Get<float>();
	AddControllerPitchInput(FloatValue);
}

void ASCharacter::LookRight(const FInputActionInstance& Instance)
{
	float FloatValue = Instance.GetValue().Get<float>();
	AddControllerYawInput(FloatValue);
}

inline void ASCharacter::PrimaryAttack()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass,SpawnTM, SpawnParams);
}

 