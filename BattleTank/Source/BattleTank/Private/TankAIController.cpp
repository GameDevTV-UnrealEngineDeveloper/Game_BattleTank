#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		ATank* PossessedTank = Cast<ATank>(InPawn);

		if(!PossessedTank) { return; }

		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	if (!ensure(GetPawn())) { return; }

	GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* ControlledTank = GetPawn();
	APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!(PlayerTank && ControlledTank)) { return; }

	MoveToActor(PlayerTank, AcceptanceRadius);

	UTankAimingComponent* AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire();
	}
}
