// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS()
class FPSTEST_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	UFUNCTION(BlueprintCallable)
		void TakeDamage(float damage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		bool hasTakenDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		bool isDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		bool isSeen;
	// Called to bind functionality to input
	

};
