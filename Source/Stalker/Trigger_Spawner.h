#pragma once

#include "Spawn_Area.h"
#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Trigger_Spawner.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS()
class STALKER_API ATrigger_Spawner : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	ATrigger_Spawner();

	UFUNCTION() void On_Overlap_Begin(AActor* overlapped_actor, AActor* other_actor);
	UFUNCTION() void On_Overlap_End(AActor* overlapped_actor, AActor* other_actor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner") ASpawn_Area *Spawner_Area;

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner") bool Is_Active; 

};
//------------------------------------------------------------------------------------------------------------
