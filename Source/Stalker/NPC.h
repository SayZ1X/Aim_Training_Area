#pragma once

#include "AWeapon.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS()
class STALKER_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	ANPC();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
//------------------------------------------------------------------------------------------------------------
UCLASS()
class STALKER_API ANPC_Human : public ANPC
{
	GENERATED_BODY()

public:
	ANPC_Human();

	UFUNCTION(BlueprintCallable, Category = "Stalker") void Attach_Weapon(USkeletalMeshComponent* character_mesh, FName arm_socket_name, AWeapon* weapon);
	UFUNCTION(BlueprintCallable, Category = "Stalker") void Decrease_Health (float damage);
	UFUNCTION(BlueprintCallable, Category = "Stalker") void Die();
	UFUNCTION(BlueprintCallable, Category = "Stalker") float Get_Max_Health() { return Max_Health; };
	UFUNCTION(BlueprintCallable, Category = "Stalker") float Get_Current_Health() { return Current_Health; };
	UFUNCTION(BlueprintCallable, Category = "Stalker") void Set_Current_Health(float added_HP) { Current_Health += added_HP; }


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stalker") AWeapon *Current_Weapon;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stalker") float Max_Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stalker") float Current_Health;

};
//------------------------------------------------------------------------------------------------------------
