#include "NPC.h"

// ANPC
//------------------------------------------------------------------------------------------------------------
ANPC::ANPC()
{
}
//------------------------------------------------------------------------------------------------------------
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//------------------------------------------------------------------------------------------------------------
void ANPC::BeginPlay()
{
	Super::BeginPlay();
}
//------------------------------------------------------------------------------------------------------------




// ANPC_Human
//------------------------------------------------------------------------------------------------------------
ANPC_Human::ANPC_Human()
{
	Max_Health = 100.0f;
	Current_Health = Max_Health;
}
//------------------------------------------------------------------------------------------------------------
void ANPC_Human::Attach_Weapon(USkeletalMeshComponent* character_mesh, FName arm_socket_name, AWeapon* weapon)
{
	if (weapon == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%S] >>>>>> weapon == 0"), __FUNCTION__);
		return;
	}

	if (Current_Weapon == 0)
		Current_Weapon->Detach();

	Current_Weapon = weapon;
	Current_Weapon->Attach_To_Socket(character_mesh, arm_socket_name);
}
//------------------------------------------------------------------------------------------------------------
void ANPC_Human::Decrease_Health(float damage)
{
	Current_Health -= damage;

	if (Current_Health <= 0.0f)
	{
		Die();
	}
}
//------------------------------------------------------------------------------------------------------------
void ANPC_Human::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("[%S] >>>>>> NPS \"%S\" is DIE!!!"), __FUNCTION__, *this->GetName() );
}
//------------------------------------------------------------------------------------------------------------
