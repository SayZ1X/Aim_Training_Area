#include "TP_WeaponComponent.h"
#include "Stalker_Character.h"
#include "Stalker_Projectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

//------------------------------------------------------------------------------------------------------------
UTP_WeaponComponent::UTP_WeaponComponent()
{
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);  // Смещение по умолчанию от персонажа до места спавна снаряда	
}
//------------------------------------------------------------------------------------------------------------
void UTP_WeaponComponent::Attach_Weapon(AStalker_Character *target_character)
{
	Character = target_character;
	if (Character == 0 || Character->Has_Rifle)
		return;

	// Аттачим оружие к персонажу
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->Mesh_1P, AttachmentRules, FName(TEXT("GripPoint")));

	Character->Has_Rifle = true;  // Устанавливаем Has_Rifle, чтобы анимационный блюпринт мог переключиться на другой набор анимаций

	// Устанавливаем биндинг действий
	if (APlayerController* player_controller = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* input_subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(player_controller->GetLocalPlayer()))
			input_subsystem->AddMappingContext(FireMappingContext, 1);  // Устанавливаем приоритет меппинга в 1, так что это перекроет действие Jump действием Fire при использовании ввода с точскрина

		if (UEnhancedInputComponent* enhanced_input_component = Cast<UEnhancedInputComponent>(player_controller->InputComponent))
			enhanced_input_component->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
	}
}
//------------------------------------------------------------------------------------------------------------
void UTP_WeaponComponent::Fire()
{
	if (Character == 0 || Character->GetController() == 0)
		return;

	// Пытаемся выстрелить снарядом
	if (ProjectileClass != 0)
	{
		if (UWorld *world = GetWorld())
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			FRotator spawn_rotation = PlayerController->PlayerCameraManager->GetCameraRotation();

			// Т.к. MuzzleOffset представлен в пространстве камеры, его надо перевести в мировое пространство прежде, чем смещать от позиции персонажа для того, чтобы получить итоговую позицию дула
			FVector spawn_location = GetOwner()->GetActorLocation() + spawn_rotation.RotateVector(MuzzleOffset);
	
			// Устанавливаем обработку столкновения при спавне
			FActorSpawnParameters actor_spawn_params;
			actor_spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			world->SpawnActor<AStalker_Projectile>(ProjectileClass, spawn_location, spawn_rotation, actor_spawn_params);  // Спавним снаряд на дуле
		}
	}
	
	if (FireSound != 0)
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());  // Пытаемся проиграть звук выстрела, если он указан
	
	if (FireAnimation != 0)
	{
		if (UAnimInstance *anim_instance = Character->Mesh_1P->GetAnimInstance() )
			anim_instance->Montage_Play(FireAnimation, 1.0f);  // Пытаемся проиграть анимацию, если она указана
	}
}
//------------------------------------------------------------------------------------------------------------
void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type end_play_reason)
{
	if (Character == 0)
		return;

	if (APlayerController* player_controller = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem *input_subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(player_controller->GetLocalPlayer()))
			input_subsystem->RemoveMappingContext(FireMappingContext);
	}
}
//------------------------------------------------------------------------------------------------------------
