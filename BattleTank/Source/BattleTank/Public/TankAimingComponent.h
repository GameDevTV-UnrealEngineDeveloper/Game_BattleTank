#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"


UENUM()
enum class EFiringState : uint8
{
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankTurret* TurretToSet, UTankBarrel* BarrelToSet);

	UFUNCTION(BlueprintCallable)
	void Fire();

	void AimAt(FVector Target);

	EFiringState GetFiringState() const;

	UFUNCTION(BlueprintCallable)
	int32 GetRoundsLeft() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringState FiringState = EFiringState::Reloading;

	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(BlueprintReadWrite, Category = "Firing")
	float LaunchSpeed = 4000.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Firing")
	float ReloadTimeInSeconds = 3.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Firing")
	int32 RoundsLeft = 3;

private:
	UTankAimingComponent();

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	double LastFireTime = 0.0;
	FVector AimDirection = FVector(0);

	void MoveBarrelTowards(FVector AimDirection);
	bool IsBarrelMoving();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

};
