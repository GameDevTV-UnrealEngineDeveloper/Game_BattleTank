#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"


UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float AcceptanceRadius = 8000.0f;

private:
	virtual void BeginPlay() override;

	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnPossessedTankDeath();

};
