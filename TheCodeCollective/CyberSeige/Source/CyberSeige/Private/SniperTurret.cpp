// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperTurret.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Kismet/KismetMathLibrary.h>
#include "Sound/SoundBase.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ASniperTurret::ASniperTurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the components
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ST BaseMesh"));
	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ST HeadMesh"));
	Barrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ST BarrelMesh"));
	LaserPSComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ST LaserParticleSystemComponent"));
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("ST Collision"));
	PlayerDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("ST PlayerDetection"));
	TextRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ST Text Renderer"));
	TextRendererNot = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ST Text Renderer Not"));

	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ST Bounding Box"));

	// Attach the components to the sockets
	SetRootComponent(Base);

	BoundingBox->SetupAttachment(Base);
	Head->SetupAttachment(Base, TEXT("Head"));
	Barrel->SetupAttachment(Head, TEXT("Barrel"));

	Collision->SetupAttachment(RootComponent);
	PlayerDetection->SetupAttachment(RootComponent);
	TextRenderer->SetupAttachment(RootComponent);
	TextRendererNot->SetupAttachment(TextRenderer);

	// Attach Laser component
	LaserPSComponent->SetupAttachment(Barrel, TEXT("Fire"));

	TextRenderer->SetVisibility(false);
	TextRendererNot->SetVisibility(false);
	PlayerDetection->AreaClass = nullptr;
	Collision->AreaClass = nullptr;

	MaxRange = 10000;
	FireCooldown = 5;
	IsFiring = false;
	IsDead = false;
	Deactivated = false;
	Repairing = false;
	Target = nullptr;
	Damage = 150;
	LastShotTime = 0;
	RepairCost = 0;
	IdleTime = 0;
	bTargetedByPlayer = false;
	bPlayerCanAfford = false;

}

void ASniperTurret::BeginPlay()
{
	Super::BeginPlay();

	// Play built sound
	UGameplayStatics::SpawnSoundAtLocation(this, PlacementSound, GetActorLocation());

}

void ASniperTurret::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);
	SetActorTickInterval(0.02);


	// Repairing
	if (Repairing && (Health > 0)) {

			// Hide UI while repairing
			TextRenderer->SetVisibility(false);
			TextRendererNot->SetVisibility(false);

			FireTimerHandle.Invalidate();

			DeactivateLaser();

			Deactivate(DeltaTime);

			// Flash Health Bar green. Implementable Event

			// Deal negative damage to self based on delta time. 1 HP/s
			Heal(DeltaTime);

			// Sound fx?

			// At full health activate
			if (Health >= MaxHealth) {
				Health = MaxHealth;
				Repairing = false;

				// Stop flashing health bar

				// Go into idle anim
				IdleTime = 5;
			}

			return;
		}
	// Deactivating
	else if (Health < (MaxHealth / 5)) {

			Deactivate(DeltaTime);
			FireTimerHandle.Invalidate();

			if (IsDead && !DestroyTimerHandle.IsValid()) {

				DeactivateLaser();

				GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ASniperTurret::Destruct, 1.f, false);


			}

			InteractionText();

			return;
		}
	// Targeting
	else {

		InteractionText();

		if (IsFiring) {
			LastShotTime += DeltaTime;
			IdleTime = 0;
			Idling = false;
		}
		else {
			IdleTime += DeltaTime;
		}

		if (!IsDead) {

			if (IsValid(Target) && TargetInRange()) {

				// Has enemy in range
				UpdateRotation(DeltaTime);

			}
			else if ((IsValid(LaserPSComponent)) && (LastShotTime > 2.5)) {

				DeactivateLaser();

			}
			else {

				if (IdleTime > 2 || Idling) {
					Idle(DeltaTime);
				}

				ResetFire();

			}

		}
	}
}

void ASniperTurret::InteractionText()
{

	// Damaged check
	if (Health > (MaxHealth * 0.9f)) {
		TextRenderer->SetVisibility(false);
		TextRendererNot->SetVisibility(false);
		bTargetedByPlayer = false;
		return;
	}


	// Player looking at defense check
	TArray<AActor*> Overlaps;
	PlayerDetection->GetOverlappingActors(Overlaps);

	if (Overlaps.Num() > 0)
	{
		for (AActor* Actor : Overlaps)
		{
			APawn* Pawn = Cast<APawn>(Actor);

			// Player check
			if (Pawn != UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
			{

				TextRenderer->SetVisibility(false);
				TextRendererNot->SetVisibility(false);
				bTargetedByPlayer = false;
				continue;

			}

			Player = Pawn;

			// Raycast the player's vision
			FVector StartPos = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
			FVector EndPos = (StartPos + (UKismetMathLibrary::GetForwardVector(Pawn->GetBaseAimRotation()) * 500));

			TArray<FHitResult> OutHits;
			GetWorld()->LineTraceMultiByChannel(OutHits, StartPos, EndPos, ECC_PhysicsBody);

			// Check if one of the hits is this actor
			for (const FHitResult Result : OutHits)
			{
				if (Result.Actor == this)
				{
					bTargetedByPlayer = true;

					// Player scrap count check
					RepairCost = (MaxHealth - Health);
					bPlayerCanAfford = CheckPlayerScrap(RepairCost);

					TextRenderer->SetVisibility(true);

					break;
				}
				else
				{
					TextRenderer->SetVisibility(false);
					TextRendererNot->SetVisibility(false);
					bTargetedByPlayer = false;
				}
			}

			return;
		}
	}
	else
	{
		TextRenderer->SetVisibility(false);
		TextRendererNot->SetVisibility(false);
		bTargetedByPlayer = false;
	}

}

void ASniperTurret::ResetFire() {

	IsFiring = false;
	LastShotTime = 0;
	FireTimerHandle.Invalidate();
	Target = nullptr;
	FindTarget();

}

void ASniperTurret::Idle(float DeltaTime)
{

	FRotator CurRot = Head->GetComponentRotation();
	FRotator NewRotation = FRotator(CurRot.Pitch, CurRot.Yaw, CurRot.Roll);

	if (((int)CurRot.Pitch != 0) || !Idling) {

		Idling = true;

		StartRot = FRotator(0.f, CurRot.Yaw + 1, CurRot.Roll);
		RightRot = FRotator(0.f, (CurRot.Yaw + 36.f), CurRot.Roll);
		LeftRot = FRotator(0.f, (CurRot.Yaw - 34.f), CurRot.Roll);

		// Lerp between the current rotation and the base idle rotation
		NewRotation = FMath::RInterpTo(CurRot, StartRot, DeltaTime, 0.5f);

	}
	else if (bRotation) {

		if ((int)CurRot.Yaw >= (int)RightRot.Yaw - 1) {
			bRotation = !bRotation;
			IdleTime = -2;
			return;
		}

		// Lerp between the current rotation and the left idle rotation
		NewRotation = FMath::RInterpTo(CurRot, RightRot, DeltaTime, 0.5f);

	}
	else {

		if ((int)CurRot.Yaw <= (int)LeftRot.Yaw + 1) {
			bRotation = !bRotation;
			IdleTime = -2;
			return;
		}

		// Lerp between the current rotation and the right idle rotation
		NewRotation = FMath::RInterpTo(CurRot, LeftRot, DeltaTime, 0.5f);

	}

	// Set the new rotation to the Top static mesh
	Head->SetWorldRotation(NewRotation);

}

void ASniperTurret::FindTarget() {

	float BestDistance = MaxRange + 1;
	float CurDistance = -1;
	AActor* BestTarget = nullptr;

	// Define the sphere trace parameters
	TArray<FHitResult> HitResults;
	FCollisionQueryParams TraceParams(FName(TEXT("SphereTrace")), false, this);
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TraceParams.AddIgnoredActor(PlayerActor);
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel1;

	// Random selection chance
	float RandomTargetChance = 0.65f;

	// Perform the sphere trace targeting pawns
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * MaxRange), FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(MaxRange), TraceParams);

	for (int HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx) {
		APawn* TargetPawn = Cast<APawn>(HitResults[HitIdx].GetActor());

		if (TargetPawn) {
			// Calculate the distance
			CurDistance = (TargetPawn->GetActorLocation() - GetActorLocation()).Size();

			// Randomly decide whether to select the current target or stick with the best target
			float RandomSelection = FMath::FRand();

			if (CurDistance < BestDistance) {

				BestTarget = TargetPawn;
				BestDistance = CurDistance;

				RandomTargetChance += 0.05f;

			}

			if (RandomSelection < RandomTargetChance)
			{
				BestTarget = TargetPawn;
				break;
			}
		}

		RandomTargetChance = FMath::Clamp(RandomTargetChance, 0.5f, 1.0f);

	}

	Target = BestTarget;

}

bool ASniperTurret::TargetInRange() {

	return (MaxRange > (Target->GetActorLocation() - GetActorLocation()).Size()) && ((Target->GetActorLocation() - GetActorLocation()).Size() > 150);

}

void ASniperTurret::UpdateRotation(float DeltaTime) {

	SetTargetHitbox();

	// Local variables and components
	float RotationSpeed = 10.f;

	// Find rotation to target and adjust
	FRotator TargetRot = ((HitboxLocation + (Target->GetVelocity() * DeltaTime)) - Head->GetComponentLocation()).Rotation();
	FRotator CurRot = Head->GetComponentRotation();

	// Clamp the target rotation in the Y-axis
	TargetRot.Pitch = FMath::Clamp(TargetRot.Pitch, -35.f, 85.f);

	// Lerp between the current rotation and the target rotation
	FRotator NewRotation = FMath::RInterpTo(CurRot, TargetRot, DeltaTime, RotationSpeed);

	// Set the new rotation to the Top static mesh
	Head->SetWorldRotation(NewRotation);

	if (!IsFiring) {
		IsFiring = true;

		// Timer until first shot
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ASniperTurret::TurretFire, FireCooldown, false);
	}

}

void ASniperTurret::SetTargetHitbox()
{
	// Get all static mesh components of the hit actor
	TArray<UStaticMeshComponent*> StaticMeshComponents;
	Target->GetComponents<UStaticMeshComponent>(StaticMeshComponents);
	UStaticMeshComponent* MeshComponent;

	// Iterate through the static mesh components
	for (int i = 0; i < StaticMeshComponents.Num(); i++)
	{
		MeshComponent = StaticMeshComponents[i];
		if (MeshComponent->GetName() == TEXT("Hitbox"))
		{
			// Get the location of the hitbox
			HitboxLocation = MeshComponent->GetComponentLocation();

			break;
		}
	}
}

void ASniperTurret::TurretFire() {

	if (IsValid(Target) && TargetInRange()) {

		// Line of Sight check with dot product?

		FireLaser();

		FireAnimation();
	}

}

void ASniperTurret::FireLaser()
{
	// Set up the trace parameters
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("SniperLaserTrace")), false, this);
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TraceParams.AddIgnoredActor(PlayerActor);
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);

	// Calculate the start and end points for the line trace
	FVector StartLocation = Barrel->GetSocketLocation("Fire");
	FVector EndLocation = StartLocation + (Barrel->GetForwardVector() * MaxRange);

	// Spawn the laser particle effect
	LaserPSComponent = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		LaserParticleSystemTemplate,
		StartLocation,
		Barrel->GetForwardVector().Rotation()
	);

	// Start Sound Effect
	UGameplayStatics::SpawnSoundAtLocation(this, LaserSound, StartLocation);

	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectParams, TraceParams);

	if (bHit) {
		AActor* HitActor = HitResult.GetActor();

		// Start Visual Effect
		LaserPSComponent->SetBeamSourcePoint(0, StartLocation, 0);
		LaserPSComponent->SetBeamTargetPoint(0, HitResult.Location, 0);

		if (IsValid(HitActor)) {

			// Damage enemy
			HitActor->TakeDamage(Damage, FDamageEvent(), this->GetInstigatorController(), this);

		}
	}
	else {

		// Start Visual Effect
		LaserPSComponent->SetBeamSourcePoint(0, StartLocation, 0);
		LaserPSComponent->SetBeamTargetPoint(0, EndLocation, 0);

	}

	// Stop Effect timer
	GetWorld()->GetTimerManager().SetTimer(LTimerHandle, this, &ASniperTurret::DeactivateLaser, 0.1f, false);

}

void ASniperTurret::DeactivateLaser()
{
	if (IsValid(LaserPSComponent)) {

		LaserPSComponent->DeactivateImmediate();

	}
	IsFiring = false;
}

void ASniperTurret::Destruct() {

	this->Destroy();

}

void ASniperTurret::Deactivate(float DeltaTime)
{

	FRotator CurRot = Head->GetComponentRotation();
	FRotator TargetRot = FRotator(-15.f, CurRot.Yaw, CurRot.Roll);

	// Lerp between the current rotation and the target rotation
	FRotator NewRotation = FMath::RInterpTo(CurRot, TargetRot, DeltaTime, 5.f);

	// Set the new rotation to the Top static mesh
	Head->SetWorldRotation(NewRotation);

	Deactivated = true;

}
