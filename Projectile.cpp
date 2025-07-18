// Fill out your copyright notice in the Description page of Project Settings.

#include <Projectile.h>
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "Engine/Engine.h"
#include "Components/MeshComponent.h"

// Sets default values
AProjectile::AProjectile ()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ProjectileSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileSphereComponent"));
	check(ProjectileSphereComponent);
	ProjectileSphereComponent->SetSphereRadius(10.0f);
	SetRootComponent(ProjectileSphereComponent);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	check(ProjectileMesh);
	ProjectileMesh->SetupAttachment(ProjectileSphereComponent, ProjectileSphereComponent->GetAttachSocketName());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	check(ProjectileMovementComponent);
	ProjectileMovementComponent->InitialSpeed           = 3000.0f;
	ProjectileMovementComponent->MaxSpeed               = 3000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.5f;
	ProjectileMovementComponent->Bounciness             = 1;
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay ()
{
	Super::BeginPlay();
	this->SetLifeSpan(5.0f);
}

// Called every frame
void AProjectile::Tick (float DeltaTime)
{
	Super::Tick(DeltaTime);
}

USphereComponent* AProjectile::GetProjectileSphereComponent () const
{
	return ProjectileSphereComponent;
}

UProjectileMovementComponent* AProjectile::GetProjectileMovementComponent () const
{
	return ProjectileMovementComponent;
}
