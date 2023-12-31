// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	health = 1.0f; // 1 = 100
	hasTakenDamage = false;
	isDead = false;
	isSeen = false;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEnemy::TakeDamage(float damage)
{
	health -= damage;
	if (health <= 0.0f) {
		health = 0.0f;
		isDead = true;
	}
	else {
		hasTakenDamage = true;
	}
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




