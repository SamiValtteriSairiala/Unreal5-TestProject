// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponClass.generated.h"

UCLASS()
class FPSTEST_API ABaseWeaponClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeaponClass();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int maxTotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int maxClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int totalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int clipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float reloadTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
