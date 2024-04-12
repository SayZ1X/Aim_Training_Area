#pragma once

#include "NPC.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawn_Area.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS()
class STALKER_API ASpawn_Area : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawn_Area();

	UFUNCTION(BlueprintCallable) void Spawn_Object();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner") ANPC_Human *Spawnable_NPC;
	UBoxComponent* Spawner;

protected:
	virtual void BeginPlay();

};
//------------------------------------------------------------------------------------------------------------
