// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/MeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class THIRDPERSON_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile ();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay () override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, meta=( AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> ProjectileSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

public:
	// Called every frame
	virtual void Tick (float DeltaTime) override;


	USphereComponent* GetProjectileSphereComponent () const;
	UProjectileMovementComponent* GetProjectileMovementComponent () const;
};
