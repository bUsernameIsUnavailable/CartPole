// © 2023 Kaya Adrian.

#include "CartpolePawn.h"

#include "PhysicsEngine/PhysicsConstraintComponent.h"


ACartpolePawn::ACartpolePawn()
{
	Cart = CreateDefaultSubobject<UStaticMeshComponent>("Cart");
	SetRootComponent(Cart);
	Cart->SetSimulatePhysics(true);
	Cart->SetEnableGravity(true);
	
	Pole = CreateDefaultSubobject<UStaticMeshComponent>("Pole");
	Pole->SetupAttachment(Cart);
	Pole->SetSimulatePhysics(true);
	Pole->SetEnableGravity(true);
	
	CartConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>("CartConstraint");
	CartConstraint->SetupAttachment(Cart);
	CartConstraint->SetVisibility(false);
	CartConstraint->SetHiddenInGame(true);
	CartConstraint->SetDisableCollision(true);
	CartConstraint->SetAngularSwing1Limit(ACM_Locked, 0.0F);
	CartConstraint->SetAngularTwistLimit(ACM_Locked, 0.0F);
	
	PoleConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>("PoleConstraint");
	PoleConstraint->SetupAttachment(Pole);
	PoleConstraint->SetVisibility(false);
	PoleConstraint->SetHiddenInGame(true);
	PoleConstraint->SetDisableCollision(true);
	PoleConstraint->SetAngularSwing1Limit(ACM_Locked, 0.0F);
	PoleConstraint->SetAngularTwistLimit(ACM_Locked, 0.0F);
}
