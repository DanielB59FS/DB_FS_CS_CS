// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Components/HealthComponent.h"
#include "GameModes/WaveGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Actors/Castle.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseEnemy::ABaseEnemy()
{
	LeftHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandCollision"));
	RightHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandCollision"));

	LeftHandCollision->SetupAttachment(GetMesh());
	RightHandCollision->SetupAttachment(GetMesh());

	PawnSenseComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSenseComponent"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

}
// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnDeath.AddDynamic(this, &ABaseEnemy::Die);

	AWaveGameMode* gameMode = Cast<AWaveGameMode>(GetWorld()->GetAuthGameMode());

	if (gameMode)
	{
		OnEnemySpawned.AddDynamic(gameMode, &AWaveGameMode::EnemyAdded);
		OnEnemyDied.AddDynamic(gameMode, &AWaveGameMode::EnemyDied);
	}

	OnEnemySpawned.Broadcast();

	Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);

	TArray<AActor*> castles;
	TSubclassOf<class ACastle> castleClass;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), castleClass, castles);
	if (castles.Num() != 0)
	{
		Castle = Cast<class ACastle>(castles[0]);
		Blackboard->SetValueAsObject(BBCastleKeyName, Castle);
	}

	TArray<UUserWidget*> widgets;
	TSubclassOf<class UUserWidget> widgetClass;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), widgets, widgetClass, false);
	if (widgets.Num() != 0)
	{
		// Need Widget Blueprint
	}
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Need animation blueprint
	// animation blueprint speed = RootComponent->GetComponentVelocity().Size();
	//  animation blueprint direction = UAnimInstance::CalculateDirection(RootComponent->GetComponentVelocity().Size(), GetActorRotation());
}

void ABaseEnemy::Die(float healthRatio)
{
	IsDead = true;
	// Need Animation Blueprint
	CharMoveComp->StopMovementImmediately();
	CharMoveComp->DisableMovement();
	UAIBlueprintHelperLibrary::GetAIController(this);
	SetActorEnableCollision(false);
	OnEnemyDied.Broadcast();
}

void ABaseEnemy::HandleSeenPawn(APawn* Pawn)
{
	CanSeePlayer = true;
	CharMoveComp->StopActiveMovement();
	Blackboard->SetValueAsObject(BBPlayerKeyName, Pawn);
}

void ABaseEnemy::HandleComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (int i = 0; i < OtherActor->Tags.Num(); i++)
	{
		if (OtherActor != this && OtherActor->Tags[i] != "Enemy")
		{
			UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, GetInstigatorController(), GetOwner(), nullptr);
		}
	}
}

void ABaseEnemy::HandlePerceptionUpdated(AActor* Pawn, int Stimulus)
{

}

void ABaseEnemy::AllEnemiesDead()
{
	Blackboard->SetValueAsObject(BBPlayerKeyName, nullptr);
}

void ABaseEnemy::Attack(UObject* ControlledPawn)
{
	ABaseEnemy* baseEnemy = Cast<ABaseEnemy>(ControlledPawn);

	if (baseEnemy)
	{
		//Need Animation Blueprint
	}
}