// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseWeaponClass.h"
#include "BaseAmmo.h"
#include "FPSTestCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReload);

UCLASS(config=Game)
class AFPSTestCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AFPSTestCharacter();

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	UPROPERTY(EditAnywhere)
		bool isZoomedIn;

	UPROPERTY(EditAnywhere)
		float FoVDefaultValue;

	UPROPERTY(EditAnywhere)
		bool isSprinting;

	UPROPERTY(EditAnywhere)
		int CurrentFovAddValue;

	UPROPERTY(EditAnywhere)
		int MaxFovAddValue;


	UPROPERTY(EditAnywhere)
		bool HasWeapon;

	UPROPERTY(EditAnywhere)
		bool Ability2Active;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		FVector respawnLocation;


	//void ReloadWeapon();

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;

	/*UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnReload OnReload;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<ABaseWeaponClass> Weapon;


	// Has character used ability?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		bool hasUsedAbility1;
	// Has character used ability?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		bool hasUsedAbility2;
	// Has character used ability?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		bool hasUsedAbility3;
	// Ability Duration.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		float Ability1Duration;
	// Ability Duration.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		float Ability2Duration;
	// Ability Duration.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		float Ability3Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		float Ability1CooldownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		float Ability2CooldownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		float Ability3CooldownTime;

	FTimerHandle ability1TimerHandle;
	FTimerHandle ability2TimerHandle;
	FTimerHandle ability3TimerHandle;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		bool CanTakeDamageBool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float StopDamageFrame;

	FTimerHandle DamageFrameTimerHandle;

	UFUNCTION(BlueprintCallable)
		void TakeDamage(float damageAmount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int AssaultRifleAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int PistolAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int ShotgunAmmo;
protected:
	
	/** Fires a projectile. */
	void OnPrimaryAction();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);


	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void Sprint();

	void StopSprint();

	void Crouch();

	void Tick(float DeltaTime);

	void StopCrouch();

	void Zoom();

	void StopZoom();

	void Reload(EWeaponType weaponType);

	void UseAbility1();
	void UseAbility2();
	void UseAbility3();

	void ResetAbility1();
	void ResetAbility2();
	void ResetAbility3();

	void Ability1CooldownComplete();
	void Ability2CooldownComplete();
	void Ability3CooldownComplete();

	void ManualReload();

	int CalculateAmmo(int AmmoAmount);

	void CanTakeDamage();

	UFUNCTION(BlueprintCallable)
		void AddAmmo(EAmmoType ammoType, int AmmoAmount);

	UFUNCTION(BlueprintCallable)
		void Die();

	UFUNCTION(BlueprintCallable)
		void Respawn();

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

