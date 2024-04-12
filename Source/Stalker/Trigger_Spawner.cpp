#include "Trigger_Spawner.h"
#include "Stalker_Character.h"

// Trigger_Spawner
//------------------------------------------------------------------------------------------------------------
ATrigger_Spawner::ATrigger_Spawner()
{
	OnActorBeginOverlap.AddDynamic(this, &ATrigger_Spawner::On_Overlap_Begin); 
	OnActorEndOverlap.AddDynamic(this, &ATrigger_Spawner::On_Overlap_End);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);

	Is_Active = false;
}
//------------------------------------------------------------------------------------------------------------
void ATrigger_Spawner::On_Overlap_Begin(AActor* overlapped_actor, AActor* other_actor)
{
	if (other_actor == 0 || other_actor == this)
		return;

	if (other_actor->IsA(ACharacter::StaticClass()))
	{
		Is_Active = true;
		FString active_state = Is_Active ? TEXT("True") : TEXT("False");
		UE_LOG(LogTemp, Warning, TEXT("[%S] >>>>> Button state is \"%S\""), __FUNCTION__, *active_state);

		if (Is_Active)
		{
			Spawner_Area->Spawn_Object();

			if (Spawner_Area->Spawnable_NPC->Get_Current_Health() <= 0.0f)
			{
				Spawner_Area->Spawnable_NPC->Set_Current_Health(Spawner_Area->Spawnable_NPC->Get_Max_Health() );
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void ATrigger_Spawner::On_Overlap_End(AActor* overlapped_actor, AActor* other_actor)
{
	if (other_actor == 0 || other_actor == this)
		return;

	if (other_actor->IsA(ACharacter::StaticClass()) )
	{
		Is_Active = false;
		FString active_state = Is_Active ? TEXT("True") : TEXT("False");
		UE_LOG(LogTemp, Warning, TEXT("[%S] >>>>> Button state is \"%S\""), __FUNCTION__, *active_state);
	}
}
//------------------------------------------------------------------------------------------------------------
void ATrigger_Spawner::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(false);
}
//------------------------------------------------------------------------------------------------------------
void ATrigger_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Is_Active)
	{
		if (Spawner_Area->Spawnable_NPC->Get_Current_Health() <= 0.0f)
		{
			Spawner_Area->Spawnable_NPC->Set_Current_Health(Spawner_Area->Spawnable_NPC->Get_Max_Health());
			Spawner_Area->Spawn_Object();
		}
	}
}
//------------------------------------------------------------------------------------------------------------
