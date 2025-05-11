#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyCharacter.generated.h"

// Klasse enthält Attribute aus Game-Config
UCLASS(config = Game)
class UEUEBUNG_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// SpringArm ist überall sichtbar aber nicht editierbar
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	// Kamera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* ThirdPersonCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputMappingContext* InputMappingContext;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
};
