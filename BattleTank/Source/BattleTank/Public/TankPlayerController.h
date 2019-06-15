#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"


UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingComponent);

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float CrosshairPositionX = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairPositionY = 0.3333f;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.0f;

	void AimAtCrossHair();

	bool GetSightRayHitLocation(FVector& HitLocation) const;

	bool GetLookDirection(FVector2D ScreenPosition, FVector& LookDirection) const;

	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnPlayerTankDeath();
};
