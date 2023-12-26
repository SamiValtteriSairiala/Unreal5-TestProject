// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "FPSTestCharacter.h"
#include "FPSTestProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	
}


void UTP_WeaponComponent::Fire()
{
	if(Character == nullptr || Character->GetController() == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Character is null!"));
		return;
	}
	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<AFPSTestProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
			}

			// Try and play a firing animation if specified
			if (FireAnimation != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(FireAnimation, 1.f);
				}
			}
			/*if (Weapon) {
				if (Weapon->clipAmmo > 0) {
					APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
					const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
					 MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
					const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

					Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					 Spawn the projectile at the muzzle
					World->SpawnActor<AFPSTestProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
					Weapon->clipAmmo -= 1;
					if (FireSound != nullptr)
					{
						UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
					}

					 Try and play a firing animation if specified
					if (FireAnimation != nullptr)
					{
						 Get the animation object for the arms mesh
						UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
						if (AnimInstance != nullptr)
						{
							AnimInstance->Montage_Play(FireAnimation, 1.f);
						}
					}
				}
				else if (Weapon->totalAmmo > 0) {
					ReloadWeapon();
				}
			}*/

		}
	}
	
	// Try and play the sound if specified
	
}

//void UTP_WeaponComponent::ReloadWeapon()
//{
//	if (Weapon) {
//		if (Weapon->clipAmmo != Weapon->maxClipAmmo) {
//			if (Weapon->totalAmmo - (Weapon->maxClipAmmo - Weapon->clipAmmo) >= 0) {
//				Weapon->totalAmmo -= (Weapon->maxClipAmmo - Weapon->clipAmmo);
//				Weapon->clipAmmo = Weapon->maxClipAmmo;
//			}
//			else {
//				Weapon->clipAmmo += Weapon->totalAmmo;
//				Weapon->totalAmmo = 0;
//			}
//		}
//	}
//}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(Character != nullptr)
	{
		// Unregister from the OnUseItem Event
		Character->OnUseItem.RemoveDynamic(this, &UTP_WeaponComponent::Fire);
	}
}

void UTP_WeaponComponent::AttachWeapon(AFPSTestCharacter* TargetCharacter)
{
	maxClipAmmo = 30;
	clipAmmo = 30;
	maxTotalAmmo = 120;
	totalAmmo = 90;
	reloadTime = 5.0f;
	Weapon = this;
	Character = TargetCharacter;
	Player = Character;
	Player->HasWeapon = true;
	//Character->Weapon = this;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Attached Weapon!"));
	if(Character != nullptr)
	{
		// Attach the weapon to the First Person Character
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GetOwner()->AttachToComponent(Character->GetMesh1P(),AttachmentRules, FName(TEXT("GripPoint")));

		// Register so that Fire is called every time the character tries to use the item being held
		Character->OnUseItem.AddDynamic(this, &UTP_WeaponComponent::Fire);
		/*Character->OnReload.AddDynamic(this, &UTP_WeaponComponent::ReloadWeapon);*/
	}
}

