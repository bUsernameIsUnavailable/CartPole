// © 2023 Kaya Adrian.

#pragma once

#include "GameFramework/Pawn.h"

#include "CartpolePawn.generated.h"

class UPhysicsConstraintComponent;


UCLASS(Abstract)
class CARTPOLE_API ACartpolePawn : public APawn
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Cart;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Pole;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPhysicsConstraintComponent> CartConstraint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPhysicsConstraintComponent> PoleConstraint;
	
public:
	UE_NODISCARD_CTOR
	ACartpolePawn();
};
