// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "FPSTestCharacter.h"
#include "BaseWeaponClass.h"
#include "TP_WeaponComponent.generated.h"


class AFPSTestCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSTEST_API UTP_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AFPSTestProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AFPSTestCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTP_WeaponComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ABaseWeaponClass* WeaponBaseClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AFPSTestCharacter* Player;

	//void ReloadWeapon();

	

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		

private:
	/** The Character holding this weapon*/
	AFPSTestCharacter* Character;
};
