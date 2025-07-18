// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class AProjectile;
class USphereComponent;

/**
 * @class AEnemyCharacter
 * @brief ������Ϸ�еĵ��˽�ɫ��
 *
 * ���ദ����˵Ļ�����Ϊ�����֪���Ӿ����������Լ�����ҽ�ɫ�ķ�Ӧ��
 */
UCLASS()
class THIRDPERSON_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//================================================================//
	// Public - ����
	//================================================================//

	/**
	 * @brief ���캯�������ý�ɫĬ�����ԡ�
	 */
	AEnemyCharacter();

	/**
	 * @brief ÿ֡���ã������ɫ����Ҫ�߼���
	 * @param DeltaTime ������һ֡��ʱ�䡣
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief ����Ƿ��ܿ���Ŀ��Actor��
	 * @param TargetActor Ҫ����Ŀ��Actor��
	 * @return ����ܿ���Ŀ���򷵻�true�����򷵻�false��
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Perception")
	bool CanSeeActor(const AActor* TargetActor) const;

	/**
	 * @brief ����Ƿ�������Ŀ��Actor��
	 * @param TargetActor Ҫ����Ŀ��Actor��
	 * @return ���������Ŀ���򷵻�true�����򷵻�false��
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Perception")
	bool CanHearActor(const AActor* TargetActor) const;

	/**
	 * @brief ʹ��ɫ����Ŀ��Actor��ת��
	 * @param TargetActor ��ת��Ŀ��Actor��
	 * @param DeltaTime ֡���ʱ�䣬����ƽ����ת��
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Action")
	void RotationToActor(const AActor* TargetActor, float DeltaTime);

	FTimerHandle FireTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "Ability|Action")
	void Fire();

protected:
	//================================================================//
	// Protected - ����
	//================================================================//

	/**
	 * @brief ʹ��ɫ׷�ٲ�����Ŀ��Actor�ƶ���
	 * @param TargetActor Ҫ׷�ٵ�Ŀ��Actor��
	 * @param DeltaTime ֡���ʱ�䣬���������ƶ���
	 */
	void TrackActor(const AActor* TargetActor, float DeltaTime);

	/**
	 * @brief ��Ϸ��ʼʱ���ã����ڳ�ʼ����
	 */
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	//================================================================//
	// Public - ���
	//================================================================//

	/**
	 * @brief �Ӿ���֪��Դ�����ͨ����һ��������������������ߵ���㡣
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Perception")
	TObjectPtr<USphereComponent> SightSource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Fire")
	TSubclassOf<AProjectile> Projectile;

	//================================================================//
	// Public - ����
	//================================================================//

	/**
	 * @brief �Ӿ���ˮƽ��нǣ���λ���ȣ��������˵���ˮƽ�������Ұ��Χ��
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Perception|Sight")
	float SightRadiusYaw;

	/**
	 * @brief �Ӿ��Ĵ�ֱ��нǣ���λ���ȣ��������˵��˴�ֱ�������Ұ��Χ��
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Perception|Sight")
	float SightRadiusPitch;

	/**
	 * @brief �Ӿ��������롣�����˾�������彫���ᱻ������
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Perception|Sight")
	float SightDistance;

	/**
	 * @brief �����������롣
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Perception|Hear")
	float HearDistance;

	/**
	 * @brief ��ɫת��Ŀ��ʱ����ת�ٶȡ�
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Action")
	float RotationSpeed;

	/**
	 * @brief ��ɫ׷��Ŀ��ʱ���ƶ��ٶȡ�
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Action")
	float MoveSpeed;

	/**
	 * @brief ׷��Ŀ��ʱ����Ŀ�걣�ֵ���С���롣������С�ڴ�ֵʱ����ɫ��ֹͣ�ƶ���
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Action")
	float MoveDistance;

protected:
	//================================================================//
	// Protected - ����
	//================================================================//

	/**
	 * @brief ���˵�ǰ׷�ٵ�Ŀ���ɫ��
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	TObjectPtr<ACharacter> TargetCharacter;
};
