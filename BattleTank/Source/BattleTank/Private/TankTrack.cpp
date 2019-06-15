#include "TankTrack.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"


UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UTankTrack::DriveTrack(float CurrentThrottle)
{	
	TArray<ASprungWheel*> Wheels = GetWheels();
	
	float ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
	float ForcePerWheel = ForceApplied / Wheels.Num();

	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}

void UTankTrack::SetThrottle(float Throttle)
{
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1.0f, 1.0f);
	DriveTrack(CurrentThrottle);
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<ASprungWheel*> ResultArray;
	TArray<USceneComponent*> Children;

	GetChildrenComponents(true, Children);

	for (USceneComponent* Child : Children)
	{
		USpawnPoint* SpawnPointChild = Cast<USpawnPoint>(Child);
		if (!SpawnPointChild) { continue; }

		AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();

		ASprungWheel* SprungWheel = Cast<ASprungWheel>(SpawnedChild);
		if (!SprungWheel) { continue; }

		ResultArray.Add(SprungWheel);
	}

	return ResultArray;
}
