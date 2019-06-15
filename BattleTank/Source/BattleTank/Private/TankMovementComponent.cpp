#include "TankMovementComponent.h"
#include "TankTrack.h"


void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector &MoveVelocity, bool bForceMaxSpeed)
{
	FVector TankForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector AIForwardIntention = MoveVelocity.GetSafeNormal();

	float RightThrow = FVector::CrossProduct(AIForwardIntention, TankForwardDirection).Z;
	IntendTurnRight(RightThrow);

	float ForwardThrow = FVector::DotProduct(TankForwardDirection, AIForwardIntention);
	IntendMoveForward(ForwardThrow);
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}
