#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Stalker_Character.h"
#include "TP_Pick_UpComponent.generated.h"

//------------------------------------------------------------------------------------------------------------
// Обьявление делегата, вызываемого, когда кто-то подберёт этот предмет
// Персонаж, подбирающий этот объект, будет передан как параметр уведомления.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn_Pick_Up, AStalker_Character*, PickUpCharacter);
//------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STALKER_API UTP_Pick_UpComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UTP_Pick_UpComponent();

	UPROPERTY(BlueprintAssignable, Category = "Interaction") FOn_Pick_Up On_Pick_Up; // Делегат, на который может подписаться кто угодно, чтобы получать это событие

protected:
	virtual void BeginPlay() override;

	/** Code for when something overlaps this component */
	UFUNCTION() void On_Sphere_Begin_Overlap(UPrimitiveComponent *overlapped_component, AActor *other_actor, UPrimitiveComponent *other_comp, int other_body_index, bool from_sweep, const FHitResult &sweep_result);
};
//------------------------------------------------------------------------------------------------------------
