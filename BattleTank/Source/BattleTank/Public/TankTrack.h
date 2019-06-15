#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"


class ASprungWheel;

UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetThrottle(float Throttle);

	UPROPERTY(BlueprintReadWrite)
	float TrackMaxDrivingForce = 400000.0f;

private:
	UTankTrack();

	void DriveTrack(float CurrentThrottle);

	TArray<class ASprungWheel*> GetWheels() const;

};
