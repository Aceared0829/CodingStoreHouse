// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"

#include <Projectile.h>
#include <SceneExport.h>

#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

//================================================================//
// AEnemyCharacter - ���캯��
//================================================================//

AEnemyCharacter::AEnemyCharacter ()
{
	// ���ô˽�ɫÿ֡����Tick()���������Ҫ�����Թر���������ܡ�
	PrimaryActorTick.bCanEverTick = true;

	// �ý�ɫ�����˶�����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	// ��ʼ��Ĭ��ֵ
	// Ĭ�ϵ�����Ұ��Χ
	SightRadiusYaw   = 150.0f;
	SightRadiusPitch = 150.0f;
	// Ĭ�ϵ�����Ұ����
	SightDistance = 1000.0f;
	// Ĭ����������
	HearDistance = 500.0f; // ����һ��Ĭ��ֵ
	// Ĭ����ת�ٶ�
	RotationSpeed = 20.0f;

	MoveSpeed = 500.0f;

	MoveDistance = 100.0f;

	// �����������Ӿ�Դ���
	SightSource = CreateDefaultSubobject<USphereComponent>(TEXT("SightSource"));
	if (SightSource)
	{
		SightSource->SetSphereRadius(20.0f);
		SightSource->SetupAttachment(RootComponent); // ���丽�ӵ������
	}
}

//================================================================//
// AEnemyCharacter - ��������
//================================================================//

void AEnemyCharacter::BeginPlay ()
{
	Super::BeginPlay();

	// ��ȡ��ҽ�ɫ��Ϊ��ʼĿ��
	TargetCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	
}

void AEnemyCharacter::EndPlay (const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void AEnemyCharacter::Tick (float DeltaTime)
{
	Super::Tick(DeltaTime);
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	// ����ܿ���������Ŀ���ɫ����ִ����Ӧ�߼�
	if (CanSeeActor(TargetCharacter) || CanHearActor(TargetCharacter))
	{
		// ת��Ŀ��
		RotationToActor(TargetCharacter, DeltaTime);
		// ׷��Ŀ��
		TrackActor(TargetCharacter, DeltaTime);
		if (!GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AEnemyCharacter::Fire, 0.5, true);
		}
	}
	else
	{
			GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}

}

//================================================================//
// AEnemyCharacter - ��֪����
//================================================================//

bool AEnemyCharacter::CanSeeActor (const AActor* TargetActor) const
{
	// ��������Ƿ���Ч
	if (!TargetActor || !SightSource)
	{
		return false;
	}

	// 1. ������
	const FVector StartLocation                = SightSource->GetComponentLocation();
	const FVector EndLocation                  = TargetActor->GetActorLocation();
	const float DistanceWithTargetActorSquared = FVector::DistSquared(StartLocation, EndLocation);
	const float SightDistanceSquared           = FMath::Square(SightDistance);

	if (DistanceWithTargetActorSquared > SightDistanceSquared)
	{
		return false; // Ŀ��̫Զ��������
	}

	// 2. �ӽǷ�Χ���
	const FRotator EnemyRotator       = this->GetActorRotation();
	const FRotator TargetActorRotator = (EndLocation - StartLocation).Rotation();
	const FRotator RotationGap        = (TargetActorRotator - EnemyRotator).GetNormalized();
	const float RotationGapYaw        = FMath::Abs(RotationGap.Yaw);
	const float RotationGapPitch      = FMath::Abs(RotationGap.Pitch);

	if (RotationGapYaw > SightRadiusYaw * 0.5f || RotationGapPitch > SightRadiusPitch * 0.5f)
	{
		return false; // Ŀ������Ұ��Χ֮��
	}

	// 3. �ϰ����� (Line Trace)
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(TargetActor);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
	                                                       HitResult,
	                                                       StartLocation,
	                                                       EndLocation,
	                                                       ECC_GameTraceChannel2,
	                                                       QueryParams
	                                                      );
	// ���û�������κ����壬���ʾ����û�б��赲
	return !bHit;
}

bool AEnemyCharacter::CanHearActor (const AActor* TargetActor) const
{
	// ��������Ƿ���Ч
	if (!TargetActor)
	{
		return false;
	}

	// ������
	const float DistanceWithTargetActorSquared = FVector::DistSquared(GetActorLocation(),
	                                                                  TargetActor->GetActorLocation());
	const float HearDistanceSquared = FMath::Square(HearDistance);

	return DistanceWithTargetActorSquared <= HearDistanceSquared;
}

//================================================================//
// AEnemyCharacter - ����
//================================================================//

void AEnemyCharacter::RotationToActor (const AActor* TargetActor, float DeltaTime)
{
	if (!TargetActor)
	{
		return;
	}

	const FVector StartLocation = this->GetActorLocation();
	const FVector EndLocation   = TargetActor->GetActorLocation();

	// ֻ����Yaw�����ת
	const FRotator TargetRotation  = FRotator{0.0f, (EndLocation - StartLocation).Rotation().Yaw, 0.0f};
	const FRotator CurrentRotation = GetActorRotation();

	// ʹ�ò�ֵƽ������ת��Ŀ�귽��
	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

	this->SetActorRotation(NewRotation);
}

void AEnemyCharacter::Fire()
{
	if (!Projectile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile class is not set!"));
		return;
	}
	FRotator EnemyCharacterYawRotation = FRotator(0.0f, GetActorRotation().Yaw, 0.0f);
	FVector ForwardVector = EnemyCharacterYawRotation.Vector();
	float SpawnDistance = 40.0f;
	FVector SpawnLocation = GetActorLocation() + ForwardVector * SpawnDistance;

	// ������ɲ����Ա�����ײ����
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(
		Projectile,
		SpawnLocation,
		EnemyCharacterYawRotation,
		SpawnParams
	);

	if (!SpawnedProjectile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn projectile!"));
	}
}
/**
 * @brief ʹ��ɫ׷�ٲ�����Ŀ��Actor�ƶ���
 *
 * �˺����������׷����ҵ��߼�����������Ŀ��ľ��룬
 * ֻ�е�Ŀ�����ض��ľ��뷶Χ��ʱ����̫��Ҳ��̫Զ�����Ż���Ŀ���ƶ���
 *
 * @param TargetActor Ҫ׷�ٵ�Ŀ��Actor��
 * @param DeltaTime ֡���ʱ�䣬���������ƶ��ٶȡ�
 */
void AEnemyCharacter::TrackActor (const AActor* TargetActor, float DeltaTime)
{
	// 1. ���Ŀ����Ч��
	if (!TargetActor)
	{
		return; // ���û��Ŀ�꣬��ִ���κβ���
	}

	const FVector SelfLocation          = GetActorLocation();
	const FVector TargetLocation        = TargetActor->GetActorLocation();
	const FVector DirectionToTarget     = TargetLocation - SelfLocation;
	const float DistanceToTargetSquared = DirectionToTarget.SizeSquared();

	// 2. ������
	// �����Ŀ��ľ���С����С�ӽ����룬��ֹͣ�ƶ������⿿��̫����
	if (DistanceToTargetSquared < FMath::Square(MoveDistance))
	{
		return;
	}
	// �����Ŀ��ľ��������Ұ���룬��ֹͣ׷�١�
	if (DistanceToTargetSquared > FMath::Square(SightDistance))
	{
		return;
	}

	// 3. ����ƶ�����
	// ���㳯��Ŀ�����ת����Yaw�ᣩ������ȡ�÷���ĵ�λ������
	const FRotator RotationToTarget = FRotator(0.0f, DirectionToTarget.Rotation().Yaw, 0.0f);
	const FVector MoveDirection     = FRotationMatrix(RotationToTarget).GetUnitAxis(EAxis::X);

	AddMovementInput(MoveDirection,1.0f);
}
