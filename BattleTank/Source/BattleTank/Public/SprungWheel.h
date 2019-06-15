#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprungWheel.generated.h"


class USphereComponent;
class UPhysicsConstraintComponent;

UCLASS()
class BATTLETANK_API ASprungWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	ASprungWheel();

	virtual void Tick(float DeltaTime) override;

	void AddDrivingForce(float forceMagnitude);

protected:
	virtual void BeginPlay() override;

private:
	void SetupConstraint();

	void ApplyForce();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Axle = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	 USphereComponent* Wheel = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent* MassAxleConstraint = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent* AxleWheelConstraint = nullptr;

	float TotalForceMagnitudeThisFrame = 0.0f;
};
