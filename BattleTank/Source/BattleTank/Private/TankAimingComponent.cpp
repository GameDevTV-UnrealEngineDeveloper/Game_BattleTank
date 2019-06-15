#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Components/StaticMeshComponent.h" 
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


UTankAimingComponent::UTankAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RoundsLeft <= 0)
	{
		FiringState = EFiringState::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}


void UTankAimingComponent::Initialise(UTankTurret* TurretToSet, UTankBarrel* BarrelToSet)
{
	Turret = TurretToSet;
	Barrel = BarrelToSet;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

void UTankAimingComponent::AimAt(FVector Target)
{
	if (!ensure(Barrel)) { return; }

	FVector LaunchVelocity(0.0f);
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(this, LaunchVelocity, StartLocation, Target, LaunchSpeed, false, 0.0f, 0.0f, ESuggestProjVelocityTraceOption::DoNotTrace);

	if (bHaveAimSolution)
	{
		AimDirection = LaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector TargetAimDirection)
{
	if (!ensure(Barrel && Turret)) { return; }

	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimRotator = TargetAimDirection.Rotation();
	FRotator DeltaRotator = AimRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);

	if (FMath::Abs(DeltaRotator.Yaw) < 180.0f)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
	{
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>
			(
				ProjectileBlueprint,
				Barrel->GetSocketLocation(FName("Projectile")),
				Barrel->GetSocketRotation(FName("Projectile"))
				);

		Projectile->Launch(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
	}
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }

	FVector BarrelForward = Barrel->GetForwardVector();

	return !BarrelForward.Equals(AimDirection, 0.01f);
}
