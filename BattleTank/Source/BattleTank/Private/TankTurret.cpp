#include "TankTurret.h"


void UTankTurret::Rotate(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0f, +1.0f);

	float RotationDelta = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	float Rotation = RelativeRotation.Yaw + RotationDelta;

	SetRelativeRotation(FRotator(0.0f, Rotation, 0.0f));
}
