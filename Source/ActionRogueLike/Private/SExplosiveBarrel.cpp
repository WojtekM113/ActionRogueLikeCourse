// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = StaticMesh;
	StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::Impact);
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("Radial Force");
	RadialForceComponent->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplosiveBarrel::Impact(UPrimitiveComponent* Projectile, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();	
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

