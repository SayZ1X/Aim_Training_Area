#include "AWeapon.h"
#include "NPC.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/SkeletalMeshSocket.h"

//------------------------------------------------------------------------------------------------------------
AWeapon::AWeapon()
{
	FireRange = 3000.0f;
}
//------------------------------------------------------------------------------------------------------------
void AWeapon::Attach(USkeletalMeshComponent *arms_mesh)
{
	if (UPrimitiveComponent* prim_comp = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		prim_comp->SetSimulatePhysics(false);
		prim_comp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	}

	// Аттачим оружие к персонажу
	FAttachmentTransformRules attachment_rules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(arms_mesh, attachment_rules, FName(TEXT("GripPoint")));
}

//------------------------------------------------------------------------------------------------------------
void AWeapon::Detach()
{
	if (UPrimitiveComponent* prim_comp = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		prim_comp->SetSimulatePhysics(true);
		prim_comp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	}

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
//------------------------------------------------------------------------------------------------------------
void AWeapon::Attach_To_Socket(USkeletalMeshComponent *character_mesh, FName arm_socket_name)
{
	if (UPrimitiveComponent* prim_comp = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		prim_comp->SetSimulatePhysics(false);
		prim_comp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	}

	// Аттачим оружие к персонажу
	/*FAttachmentTransformRules attachment_rules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(character_mesh, attachment_rules, arm_socket_name);*/

	if (const USkeletalMeshSocket* weapon_socket = character_mesh->GetSocketByName(arm_socket_name))
		weapon_socket->AttachActor(this, character_mesh);
	else
		UE_LOG(LogTemp, Warning, TEXT("[%S] >>>>>> Missing socket: %s"), __FUNCTION__, *(arm_socket_name.ToString()));
}
//------------------------------------------------------------------------------------------------------------
void AWeapon::Fire(AStalker_Character* character)
{
	if (character == 0 || character->GetController() == 0)
		return;

	//FVector start_loc = BP_Weapon_Mesh_Component->GetSocketLocation("MuzzleFlash");  // луч будет начинаться с сокета в пушке который принадлежит дулу

	FVector start_loc = character->FindCameraComponent(character)->GetComponentLocation();  // луч будет начинаться с ценра экрана
	FVector end_loc = start_loc + character->FindCameraComponent(character)->GetForwardVector() * FireRange;

	DrawDebugLine(GetWorld(), start_loc, end_loc, FColor::Red, false, 0.1f, 0, 1.0f);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, start_loc, end_loc, ECC_Visibility, CollisionParams);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%S] >>>>> Hit location: %s"), __FUNCTION__, *HitResult.ImpactPoint.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%S] >>>>> Line trace didn't hit any object."), __FUNCTION__);
	}

	if (Fire_Animation != 0)
	{
		BP_Weapon_Mesh_Component->PlayAnimation(Fire_Animation, false);
	}

	if (bHit)
	{
		FVector ImpactPoint = HitResult.ImpactPoint;
		DrawDebugPoint(GetWorld(), ImpactPoint, 10, FColor::Green, false, 0.1f);
		
		if (AActor* HitActor = HitResult.GetActor())
		{
			HandleHit(HitActor);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AWeapon::HandleHit(AActor* hit_actor)
{
	if (ANPC_Human *hit_npc = Cast<ANPC_Human>(hit_actor) )
	{
		UE_LOG(LogTemp, Warning, TEXT("[%S] >>>>> Hit actor name: %s"), __FUNCTION__, *hit_npc->GetName());

		hit_npc->Decrease_Health(27.0f);
	}
}
//------------------------------------------------------------------------------------------------------------
void AWeapon::Fire_NPC()
{
	if (Fire_Animation != 0)
	{
		BP_Weapon_Mesh_Component->PlayAnimation(Fire_Animation, false);
	}
}
//------------------------------------------------------------------------------------------------------------
