#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!GetPawn()) { return; }
	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimAtCrossHair();
}


void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		ATank* PlayerTank = Cast<ATank>(InPawn);

		if (!ensure(PlayerTank)) { return; }
		PlayerTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPlayerTankDeath);
	}
}

void ATankPlayerController::OnPlayerTankDeath()
{
	if (!ensure(GetPawn())) { return; }

	StartSpectatingOnly();
}

void ATankPlayerController::AimAtCrossHair()
{
	if (!GetPawn()) { return; }

	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(AimingComponent)) { return; }

	FVector Target;

	if (GetSightRayHitLocation(Target))
	{
		AimingComponent->AimAt(Target);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	int32 ViewportSizeX;
	int32 ViewportSizeY;

	GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2D ScreenPosition = FVector2D(ViewportSizeX * CrosshairPositionX, ViewportSizeY * CrosshairPositionY);

	FVector LookDirection;

	if (GetLookDirection(ScreenPosition, LookDirection))
	{
		return GetLookVectorHitLocation(LookDirection, HitLocation);
	}

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenPosition, FVector& LookDirection) const
{
	FVector CameraWorldLocation;

	return DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, CameraWorldLocation, LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;

	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	FVector EndLocation = StartLocation + (LookDirection * LineTraceRange);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Camera))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	else
	{
		HitLocation = FVector(0);
		return false;
	}
}
