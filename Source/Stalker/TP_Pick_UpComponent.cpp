#include "TP_Pick_UpComponent.h"

//------------------------------------------------------------------------------------------------------------
UTP_Pick_UpComponent::UTP_Pick_UpComponent()
{
	SphereRadius = 32.0f;  // Радиус сферы коллизий
}
//------------------------------------------------------------------------------------------------------------
void UTP_Pick_UpComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UTP_Pick_UpComponent::On_Sphere_Begin_Overlap);  // Регистрируем событие пересечения (Overlap Event)
}
//------------------------------------------------------------------------------------------------------------
void UTP_Pick_UpComponent::On_Sphere_Begin_Overlap(UPrimitiveComponent *overlapped_component, AActor *other_actor, UPrimitiveComponent *other_comp, int other_body_index, bool from_sweep, const FHitResult &sweep_result)
{
	// Если пересеклись с персонажем от первого лица
	AStalker_Character* сharacter = Cast<AStalker_Character>(other_actor);

	if (сharacter != 0)
	{
		On_Pick_Up.Broadcast(сharacter);  // Уведомить о том, что актора подбирают
		OnComponentBeginOverlap.RemoveAll(this);  // Снимаем регистрацию с события пересечения (Overlap Event), чтобы оно больше не триггерилось
	}
}
//------------------------------------------------------------------------------------------------------------
