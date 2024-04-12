#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Stalker_Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class AWeapon;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

//------------------------------------------------------------------------------------------------------------
UCLASS(config=Game)
class AStalker_Character : public ACharacter
{
	GENERATED_BODY()

public:
	AStalker_Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon) bool Has_Rifle; // Переменная для АnіmВР для переключения на другой набор анимаций
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh) USkeletalMeshComponent* Mesh_1P;  // Меш игрока от первого лица(только свои руки)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) AWeapon *Current_Weapon;
	
	UCameraComponent* FindCameraComponent(AActor* Actor);

protected:
	virtual void BeginPlay();
	virtual void SetupPlayerInputComponent(UInputComponent *input_component);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<AActor *> Interactable_Actors;

private:
	void On_Action_Move(const FInputActionValue& value);  // Обработка movement input
	void On_Action_Look(const FInputActionValue& value); // Обработка for looking input
	void On_Action_Fire(const FInputActionValue& value);
	void On_Action_Use(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable) void Pickup_Weapon(AWeapon* weapon); // Приаттачить пушку к персонажу

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) UCameraComponent *FirstPersonCameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputMappingContext *DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction *Action_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction *Action_Move;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction *Action_Look;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction *Action_Fire;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction *Action_Use;
		
};
//------------------------------------------------------------------------------------------------------------
