#include "Stalker_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

//------------------------------------------------------------------------------------------------------------
AStalker_Projectile::AStalker_Projectile()
{
	// Используем сферу для представления простых столкновений (коллизии)
	Collision_Comp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	Collision_Comp->InitSphereRadius(5.0f);
	Collision_Comp->BodyInstance.SetCollisionProfileName("Projectile");
	Collision_Comp->OnComponentHit.AddDynamic(this, &AStalker_Projectile::On_Hit);  // Установим уведомление, которое сработает, когда этот компонент ударится обо что-то блокирующее

	// Игроки не могут ходить по объекту
	Collision_Comp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	Collision_Comp->CanCharacterStepUpOn = ECB_No;

	RootComponent = Collision_Comp;  // Установим как корневой (root) компонент

	// Используем ProjectileMovementComponent для управления движением снаряда
	Projectile_Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	Projectile_Movement->UpdatedComponent = Collision_Comp;
	Projectile_Movement->InitialSpeed = 3000.0f;
	Projectile_Movement->MaxSpeed = 3000.0f;
	Projectile_Movement->bRotationFollowsVelocity = true;
	Projectile_Movement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;  // По умолчанию умирает через 3 секунды
}
//------------------------------------------------------------------------------------------------------------
void AStalker_Projectile::On_Hit(UPrimitiveComponent *hit_comp, AActor *other_actor, UPrimitiveComponent *other_comp, FVector normal_impulse, const FHitResult &hit)
{
	// Добавим только импульс другому компоненту и уничтожим снаряд, если ударились об физический объект
	if ((other_actor != 0) && (other_actor != this) && (other_comp != 0) && other_comp->IsSimulatingPhysics())
	{
		other_comp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}
//------------------------------------------------------------------------------------------------------------
