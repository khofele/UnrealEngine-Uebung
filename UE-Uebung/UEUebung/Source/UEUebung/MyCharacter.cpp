#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// erzeugt SpringArm f�r Kamera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));

	// SpringArm an Root des Characters h�ngen --> Root fungiert als Parentobjekt
	SpringArm->SetupAttachment(this->RootComponent);

	// Default "Arml�nge"
	SpringArm->TargetArmLength = 300.0f;

	// Kamera erzeugen und an SpringArm anh�ngen
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));

	// Kamera an Socket anh�ngen, damit Ausrichtung richtig ist
	ThirdPersonCamera->SetupAttachment(this->SpringArm, USpringArmComponent::SocketName);

	// Charakter wird nicht vom Character-Controller rotiert
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

	// Controller des Pawns w�hlen und dessen Rotationsdaten an SpringArm geben --> dadurch mit Maus SpringArm steuern/rotieren
	SpringArm->bUsePawnControlRotation = true;
	// es soll nur der SpringArm rotiert werden und nicht die Kamera, die am SpringArm h�ngt
	ThirdPersonCamera->bUsePawnControlRotation = false;

	// Charakter soll, sobald er losl�uft (d.h. WASD gedr�ckt wird), in die entsprechende Bewegungsrichtung laufen
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// jeder Character hat eine Movement-Component

	// wie stark Character beim Sprung nach oben geschleudert wird
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	// Wie gut man sich in der Luft bewegen kann (Wert zwischen 0 und 1)
	GetCharacterMovement()->AirControl = 0.2f;

	// Wie lang man die Sprungtaste halten kann, um l�nger/h�her zu springen
	this->JumpMaxHoldTime = 0.2f;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// check pr�ft den Speicherbereich, ob der angegebenene Component verf�gbar ist
	//check(PlayerInputComponent);

	// holt sich den Controller und die Input-Subsystems und f�gt den Input Mapping Context hinzu
	if (APlayerController* PlayerController = CastChecked<APlayerController>(Controller)) {

		// Controller auf enhanced Inputsystem casten
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Aktionen an Methoden binden
		Input->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);

		Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

		Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
	}
}

// FInputActionValue enth�lt die X- und Y-Koordinaten der Input-Action
void AMyCharacter::Move(const FInputActionValue& Value)
{
	// holt die Eingabewerte der Aktion IA_Move, hier: von WASD, d.h. einen 2D-Vektor mit X- und Y-Werten
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {

		// Tastatur steuert den Controller --> Controller nimmt Tastatureingabe entgegen
		// Controller hat sein eigenes Koordinatensystem
		// deshalb kann man sich von Controller die Rotation des Characters holen, da dadurch die Bewegungsrichtung vorgegeben wird

		// holt sich den Yaw-Winkel der Kamera (links/rechts), um die Richtung des Spielers, in die er nach vorne geht, zu bestimmen
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

		// berechnet die Vorw�rtsrichtung relativ zur Kamera
		// FRotationMatrix() erstellt eine Rotationsmatrix aus der gegebenen Yaw-Rotation
		// GetUnitAxis(EAxis::X) holt sich den X-Vektor, also die Vorw�rtsrichtung, aus der Matrix --> man erh�lt die Vorw�rtsrichtung basierend auf der Kameraausrichtung
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// berechnet Rechtsrichtung relativ zur Kamera
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Wendet die Eingabe an, um die Bewegung zu erzeugen
		// AddMovementInput(Direction, Scale)
		// --> Direction = FVector, der angibt, in welche Richtung sich der Character bewegt
		// --> Scale = float, der angibt, wie stark die Bewegung sein soll (zwischen -1.0 und 1.0)
		// W = 1.0 (auf Y-Achse)
		// S = -1.0 (auf Y-Achse)
		// --> W und S beeinflussen die Vorw�rts- bzw. R�ckw�rtsbewegung, d.h. den MovementVector.Y
		AddMovementInput(ForwardDirection, MovementVector.Y); // MovementVector.Y bezieht sich auf die Y-Koordinate auf dem Bildschirm w�hrend sich die ForwardDirection und die dazu geh�rige X-Achse auf das Koordinatensystem des Characters bezieht
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();

	// Koordinatensystem des Controllers versetzen --> wird dann in der Move-Methode entsprechend ausgelesen
	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}


