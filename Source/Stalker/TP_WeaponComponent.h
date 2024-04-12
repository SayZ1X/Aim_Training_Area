#pragma once

#include "Stalker_Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_WeaponComponent.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKER_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UTP_WeaponComponent();

	UFUNCTION(BlueprintCallable, Category = "Weapon") void Attach_Weapon(AStalker_Character* target_character);  // Приаттачить пушку к персонажу
	UFUNCTION(BlueprintCallable, Category = "Weapon") void Fire();  // Выстрелить снарядом

	UPROPERTY(EditDefaultsOnly, Category=Projectile) TSubclassOf<class AStalker_Projectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay) USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay) UAnimMontage* FireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay) FVector MuzzleOffset;  // Смещение дула пушки от позиции персонажа

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true")) UInputMappingContext* FireMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true")) UInputAction* FireAction;

protected:
	virtual void EndPlay(const EEndPlayReason::Type end_play_reason) override;

private:
	AStalker_Character* Character;  // Персонаж, который держит это оружие
};
