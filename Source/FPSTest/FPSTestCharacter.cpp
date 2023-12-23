// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSTestCharacter.h"
#include "FPSTestProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/CharacterMovementComponent.h"


//////////////////////////////////////////////////////////////////////////
// AFPSTestCharacter

AFPSTestCharacter::AFPSTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;
	PrimaryActorTick.bCanEverTick = true;
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	isZoomedIn = false;
	isSprinting = false;
	FoVDefaultValue = 110.0f;
	MaxFovAddValue = 10;
}

void AFPSTestCharacter::BeginPlay()
{
	if (auto firstPersonCamera = GetFirstPersonCameraComponent()) {
		firstPersonCamera->SetFieldOfView(FoVDefaultValue);
	}
	// Call the base class  
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////// Input

void AFPSTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPSTestCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPSTestCharacter::StopSprint);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPSTestCharacter::Crouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPSTestCharacter::StopCrouch);

	PlayerInputComponent->BindAction("SecondaryAction", IE_Pressed, this, &AFPSTestCharacter::Zoom);
	PlayerInputComponent->BindAction("SecondaryAction", IE_Released, this, &AFPSTestCharacter::StopZoom);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AFPSTestCharacter::OnPrimaryAction);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AFPSTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AFPSTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AFPSTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AFPSTestCharacter::LookUpAtRate);
}

void AFPSTestCharacter::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	OnUseItem.Broadcast();
}

void AFPSTestCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AFPSTestCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AFPSTestCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSTestCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFPSTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFPSTestCharacter::Sprint()
{
	if (GetCharacterMovement()->bWantsToCrouch == true) {
		return;
	}
	if (isZoomedIn == true) {
		StopZoom();
	}
	isSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
}

void AFPSTestCharacter::StopSprint()
{
	CurrentFovAddValue = 0; // Reset the "sprint" fov
	isSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	if (auto firstPersonCamera = GetFirstPersonCameraComponent()) {
		firstPersonCamera->SetFieldOfView(FoVDefaultValue);
	}
}

void AFPSTestCharacter::Crouch()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	if (auto firstPersonCamera = GetFirstPersonCameraComponent()) {
		firstPersonCamera->SetFieldOfView(FoVDefaultValue);
	}
	GetCharacterMovement()->bWantsToCrouch = true;
	GetCharacterMovement()->Crouch(true);
}

void AFPSTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isSprinting) {
		if (auto firstPersonCamera = GetFirstPersonCameraComponent()) {
			if (CurrentFovAddValue == MaxFovAddValue) {
				firstPersonCamera->SetFieldOfView(FoVDefaultValue + MaxFovAddValue);
			}
			if (CurrentFovAddValue != MaxFovAddValue) {
				CurrentFovAddValue += 1;
				firstPersonCamera->SetFieldOfView(FoVDefaultValue + CurrentFovAddValue);
			}
		}
	}
}

void AFPSTestCharacter::StopCrouch()
{
	GetCharacterMovement()->bWantsToCrouch = false;
	GetCharacterMovement()->UnCrouch(true);
}

void AFPSTestCharacter::Zoom()
{
	if (isSprinting == true) {
		return;
	}

	if (auto firstPersonCamera = GetFirstPersonCameraComponent()) {
		firstPersonCamera->SetFieldOfView(FoVDefaultValue - 30.0f);
		isZoomedIn = true;
	}
}

void AFPSTestCharacter::StopZoom()
{
	if (auto firstPersonCamera = GetFirstPersonCameraComponent()) {
		firstPersonCamera->SetFieldOfView(FoVDefaultValue);
		isZoomedIn = false;
	}
}

bool AFPSTestCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFPSTestCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AFPSTestCharacter::EndTouch);

		return true;
	}
	
	return false;
}
