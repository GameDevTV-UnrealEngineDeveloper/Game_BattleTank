#include "SprungWheel.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


ASprungWheel::ASprungWheel()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	MassAxleConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassAxleConstraint"));
	SetRootComponent(MassAxleConstraint);

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Axle->SetupAttachment(MassAxleConstraint);

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Axle);

	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));
	AxleWheelConstraint->SetupAttachment(Axle);
}


void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();

	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);

	SetupConstraint();
}

void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		UE_LOG(LogTemp, Warning, TEXT("TotalForceMagnitudeThisFrame reset"));

		TotalForceMagnitudeThisFrame = 0.0f;
	}
}

void ASprungWheel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	ApplyForce();
}

void ASprungWheel::SetupConstraint()
{
	if (!GetAttachParentActor()) { return; }

	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());

	if (!BodyRoot) { return; }

	MassAxleConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Axle, NAME_None);
	AxleWheelConstraint->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);
}

void ASprungWheel::AddDrivingForce(float forceMagnitude)
{
	TotalForceMagnitudeThisFrame += forceMagnitude;
}

void ASprungWheel::ApplyForce()
{
	Wheel->AddForce(Axle->GetForwardVector() * TotalForceMagnitudeThisFrame);
}
