#include "TankBarrel.h"


void UTankBarrel::Elevate(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0f, +1.0f);
	
	float ElevationDelta = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	float RawElevation = RelativeRotation.Pitch + ElevationDelta;

	RawElevation = FMath::Clamp<float>(RawElevation, MinElevationDegrees, MaxElevationDegrees);

	SetRelativeRotation(FRotator(RawElevation, 0.0f, 0.0f));
}
