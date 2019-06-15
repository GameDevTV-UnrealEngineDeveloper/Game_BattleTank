#include "SpawnPoint.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


USpawnPoint::USpawnPoint()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnClass, GetComponentTransform());

	if (!SpawnedActor) { return; }

	SpawnedActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

	UGameplayStatics::FinishSpawningActor(SpawnedActor, GetComponentTransform());
}

void USpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
