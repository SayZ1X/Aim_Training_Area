#pragma once

#include "Stalker_Character.h"
#include "Stalker_Projectile.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "AWeapon.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS()
class STALKER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	void Attach(USkeletalMeshComponent *arms_mesh);
	void Detach();

	UFUNCTION(BlueprintCallable, Category = "Weapon") void Attach_To_Socket(USkeletalMeshComponent *character_mesh, FName arm_socket_name);
	UFUNCTION(BlueprintCallable, Category = "Weapon") void Fire(AStalker_Character* character);  // Выстрелить снарядом
	UFUNCTION(BlueprintCallable, Category = "Weapon") void Fire_NPC();  // Выстрелить снарядом

	float FireRange;

	UPROPERTY(EditDefaultsOnly, Category = Projectile) TSubclassOf<class AStalker_Projectile> Projectile_Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay) USoundBase* Fire_Sound = 0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay) UAnimMontage* Fire_Animation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay) UAnimationAsset* Fire_Animation = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay) FVector Muzzle_Offset;  // Смещение дула пушки от позиции персонажа

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay) USkeletalMeshComponent * BP_Weapon_Mesh_Component;
	void HandleHit(AActor* hit_actor);
};
//------------------------------------------------------------------------------------------------------------
