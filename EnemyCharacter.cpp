// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"

#include <Projectile.h>
#include <SceneExport.h>

#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

//================================================================//
// AEnemyCharacter - 构造函数
//================================================================//

AEnemyCharacter::AEnemyCharacter ()
{
	// 设置此角色每帧调用Tick()。如果不需要，可以关闭以提高性能。
	PrimaryActorTick.bCanEverTick = true;

	// 让角色朝向运动方向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	// 初始化默认值
	// 默认敌人视野范围
	SightRadiusYaw   = 150.0f;
	SightRadiusPitch = 150.0f;
	// 默认敌人视野距离
	SightDistance = 1000.0f;
	// 默认听觉距离
	HearDistance = 500.0f; // 假设一个默认值
	// 默认旋转速度
	RotationSpeed = 20.0f;

	MoveSpeed = 500.0f;

	MoveDistance = 100.0f;

	// 创建并设置视觉源组件
	SightSource = CreateDefaultSubobject<USphereComponent>(TEXT("SightSource"));
	if (SightSource)
	{
		SightSource->SetSphereRadius(20.0f);
		SightSource->SetupAttachment(RootComponent); // 将其附加到根组件
	}
}

//================================================================//
// AEnemyCharacter - 生命周期
//================================================================//

void AEnemyCharacter::BeginPlay ()
{
	Super::BeginPlay();

	// 获取玩家角色作为初始目标
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
	// 如果能看到或听到目标角色，则执行相应逻辑
	if (CanSeeActor(TargetCharacter) || CanHearActor(TargetCharacter))
	{
		// 转向目标
		RotationToActor(TargetCharacter, DeltaTime);
		// 追踪目标
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
// AEnemyCharacter - 感知能力
//================================================================//

bool AEnemyCharacter::CanSeeActor (const AActor* TargetActor) const
{
	// 检查输入是否有效
	if (!TargetActor || !SightSource)
	{
		return false;
	}

	// 1. 距离检查
	const FVector StartLocation                = SightSource->GetComponentLocation();
	const FVector EndLocation                  = TargetActor->GetActorLocation();
	const float DistanceWithTargetActorSquared = FVector::DistSquared(StartLocation, EndLocation);
	const float SightDistanceSquared           = FMath::Square(SightDistance);

	if (DistanceWithTargetActorSquared > SightDistanceSquared)
	{
		return false; // 目标太远，看不见
	}

	// 2. 视角范围检查
	const FRotator EnemyRotator       = this->GetActorRotation();
	const FRotator TargetActorRotator = (EndLocation - StartLocation).Rotation();
	const FRotator RotationGap        = (TargetActorRotator - EnemyRotator).GetNormalized();
	const float RotationGapYaw        = FMath::Abs(RotationGap.Yaw);
	const float RotationGapPitch      = FMath::Abs(RotationGap.Pitch);

	if (RotationGapYaw > SightRadiusYaw * 0.5f || RotationGapPitch > SightRadiusPitch * 0.5f)
	{
		return false; // 目标在视野范围之外
	}

	// 3. 障碍物检查 (Line Trace)
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
	// 如果没有命中任何物体，则表示视线没有被阻挡
	return !bHit;
}

bool AEnemyCharacter::CanHearActor (const AActor* TargetActor) const
{
	// 检查输入是否有效
	if (!TargetActor)
	{
		return false;
	}

	// 距离检查
	const float DistanceWithTargetActorSquared = FVector::DistSquared(GetActorLocation(),
	                                                                  TargetActor->GetActorLocation());
	const float HearDistanceSquared = FMath::Square(HearDistance);

	return DistanceWithTargetActorSquared <= HearDistanceSquared;
}

//================================================================//
// AEnemyCharacter - 动作
//================================================================//

void AEnemyCharacter::RotationToActor (const AActor* TargetActor, float DeltaTime)
{
	if (!TargetActor)
	{
		return;
	}

	const FVector StartLocation = this->GetActorLocation();
	const FVector EndLocation   = TargetActor->GetActorLocation();

	// 只关心Yaw轴的旋转
	const FRotator TargetRotation  = FRotator{0.0f, (EndLocation - StartLocation).Rotation().Yaw, 0.0f};
	const FRotator CurrentRotation = GetActorRotation();

	// 使用插值平滑地旋转到目标方向
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

	// 添加生成参数以避免碰撞问题
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
 * @brief 使角色追踪并朝向目标Actor移动。
 *
 * 此函数处理敌人追踪玩家的逻辑。它会检查与目标的距离，
 * 只有当目标在特定的距离范围内时（不太近也不太远），才会向目标移动。
 *
 * @param TargetActor 要追踪的目标Actor。
 * @param DeltaTime 帧间隔时间，用于缩放移动速度。
 */
void AEnemyCharacter::TrackActor (const AActor* TargetActor, float DeltaTime)
{
	// 1. 检查目标有效性
	if (!TargetActor)
	{
		return; // 如果没有目标，则不执行任何操作
	}

	const FVector SelfLocation          = GetActorLocation();
	const FVector TargetLocation        = TargetActor->GetActorLocation();
	const FVector DirectionToTarget     = TargetLocation - SelfLocation;
	const float DistanceToTargetSquared = DirectionToTarget.SizeSquared();

	// 2. 距离检查
	// 如果与目标的距离小于最小接近距离，则停止移动，避免靠得太近。
	if (DistanceToTargetSquared < FMath::Square(MoveDistance))
	{
		return;
	}
	// 如果与目标的距离大于视野距离，则停止追踪。
	if (DistanceToTargetSquared > FMath::Square(SightDistance))
	{
		return;
	}

	// 3. 添加移动输入
	// 计算朝向目标的旋转（仅Yaw轴），并获取该方向的单位向量。
	const FRotator RotationToTarget = FRotator(0.0f, DirectionToTarget.Rotation().Yaw, 0.0f);
	const FVector MoveDirection     = FRotationMatrix(RotationToTarget).GetUnitAxis(EAxis::X);

	AddMovementInput(MoveDirection,1.0f);
}
