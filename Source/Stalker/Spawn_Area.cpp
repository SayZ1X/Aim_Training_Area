#include "Spawn_Area.h"
#include "Stalker_Character.h"
#include "Kismet/KismetMathLibrary.h"

//------------------------------------------------------------------------------------------------------------
ASpawn_Area::ASpawn_Area()
{
	Spawner = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawner_Box") );
	Spawner->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
//------------------------------------------------------------------------------------------------------------
void ASpawn_Area::Spawn_Object()
{
	FVector object_rand_location = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), Spawner->GetScaledBoxExtent() );
	Spawnable_NPC->SetActorLocation(object_rand_location);

	AStalker_Character *Player = Cast<AStalker_Character>(GetWorld()->GetFirstPlayerController()->GetPawn() );
	FVector direction_to_player = (Player->GetActorLocation() - Spawnable_NPC->GetActorLocation()).GetSafeNormal();
	FRotator rotation_to_player = FRotationMatrix::MakeFromX(direction_to_player).Rotator();
	Spawnable_NPC->SetActorRotation(rotation_to_player);
}
//------------------------------------------------------------------------------------------------------------
void ASpawn_Area::BeginPlay()
{
	
}
//------------------------------------------------------------------------------------------------------------
