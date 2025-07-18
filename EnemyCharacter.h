// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class AProjectile;
class USphereComponent;

/**
 * @class AEnemyCharacter
 * @brief 代表游戏中的敌人角色。
 *
 * 该类处理敌人的基本行为，如感知（视觉和听觉）以及对玩家角色的反应。
 */
UCLASS()
class THIRDPERSON_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//================================================================//
	// Public - 方法
	//================================================================//

	/**
	 * @brief 构造函数，设置角色默认属性。
	 */
	AEnemyCharacter();

	/**
	 * @brief 每帧调用，处理角色的主要逻辑。
	 * @param DeltaTime 距离上一帧的时间。
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 检查是否能看到目标Actor。
	 * @param TargetActor 要检查的目标Actor。
	 * @return 如果能看到目标则返回true，否则返回false。
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Perception")
	bool CanSeeActor(const AActor* TargetActor) const;

	/**
	 * @brief 检查是否能听到目标Actor。
	 * @param TargetActor 要检查的目标Actor。
	 * @return 如果能听到目标则返回true，否则返回false。
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Perception")
	bool CanHearActor(const AActor* TargetActor) const;

	/**
	 * @brief 使角色朝向目标Actor旋转。
	 * @param TargetActor 旋转的目标Actor。
	 * @param DeltaTime 帧间隔时间，用于平滑旋转。
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Action")
	void RotationToActor(const AActor* TargetActor, float DeltaTime);

	FTimerHandle FireTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "Ability|Action")
	void Fire();

protected:
	//================================================================//
	// Protected - 方法
	//================================================================//

	/**
	 * @brief 使角色追踪并朝向目标Actor移动。
	 * @param TargetActor 要追踪的目标Actor。
	 * @param DeltaTime 帧间隔时间，用于缩放移动。
	 */
	void TrackActor(const AActor* TargetActor, float DeltaTime);

	/**
	 * @brief 游戏开始时调用，用于初始化。
	 */
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	//================================================================//
	// Public - 组件
	//================================================================//

	/**
	 * @brief 视觉感知的源组件，通常是一个球体组件，定义了视线的起点。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Perception")
	TObjectPtr<USphereComponent> SightSource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Fire")
	TSubclassOf<AProjectile> Projectile;

	//================================================================//
	// Public - 属性
	//================================================================//

	/**
	 * @brief 视觉的水平半夹角（单位：度）。决定了敌人水平方向的视野范围。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Perception|Sight")
	float SightRadiusYaw;

	/**
	 * @brief 视觉的垂直半夹角（单位：度）。决定了敌人垂直方向的视野范围。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Perception|Sight")
	float SightRadiusPitch;

	/**
	 * @brief 视觉的最大距离。超过此距离的物体将不会被看到。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Perception|Sight")
	float SightDistance;

	/**
	 * @brief 听觉的最大距离。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Perception|Hear")
	float HearDistance;

	/**
	 * @brief 角色转向目标时的旋转速度。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Action")
	float RotationSpeed;

	/**
	 * @brief 角色追踪目标时的移动速度。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Action")
	float MoveSpeed;

	/**
	 * @brief 追踪目标时，与目标保持的最小距离。当距离小于此值时，角色将停止移动。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Action")
	float MoveDistance;

protected:
	//================================================================//
	// Protected - 属性
	//================================================================//

	/**
	 * @brief 敌人当前追踪的目标角色。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	TObjectPtr<ACharacter> TargetCharacter;
};
