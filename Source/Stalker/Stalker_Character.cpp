#include "Stalker_Character.h"
#include "Stalker_Projectile.h"
#include "AWeapon.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// AStalker_Character
//------------------------------------------------------------------------------------------------------------
AStalker_Character::AStalker_Character()
{
	Has_Rifle = false;  // В начале у персонажа нет оружия в руках
	
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f); // Задаем размер капсулы коллизии

	// Создаём компонент камеры	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f)); // Позиционируем камеру
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Создаем компонент меша, который будет видно от первого лица (когда игрок управляет этим персонажем)
	Mesh_1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh_1P->SetOnlyOwnerSee(true);
	Mesh_1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh_1P->bCastDynamicShadow = false;
	Mesh_1P->CastShadow = false;
	//Mesh_1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh_1P->SetRelativeLocation(FVector(-30.0f, 0.0f, -150.0f));

}
//------------------------------------------------------------------------------------------------------------
UCameraComponent* AStalker_Character::FindCameraComponent(AActor* Actor)
{
	if (AStalker_Character* Character = Cast<AStalker_Character>(Actor) )
	{ // Проверяем, является ли переданный актер персонажем

		// Проходим по дереву компонентов персонажа и ищем компонент камеры
		TArray<USceneComponent*> ChildrenComponents;
		Character->GetRootComponent()->GetChildrenComponents(true, ChildrenComponents);
		for (USceneComponent* Child : ChildrenComponents)
		{
			if (UCameraComponent* CameraComponent = Cast<UCameraComponent>(Child) )
			{ // Нашли компонент камеры, возвращаем его
				return CameraComponent;
			}
		}
	}

	return nullptr; // Если компонент камеры не найден, возвращаем nullptr
}
//------------------------------------------------------------------------------------------------------------
void AStalker_Character::BeginPlay()
{
	Super::BeginPlay();

	// Добавляем Mapping Context для ввода
	if (APlayerController *player_controller = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem *input_subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(player_controller->GetLocalPlayer()))
			input_subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

}
//------------------------------------------------------------------------------------------------------------
void AStalker_Character::SetupPlayerInputComponent(UInputComponent *input_component)
{
	// Устанавливаем биндинги действий	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(input_component))
	{
		EnhancedInputComponent->BindAction(Action_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(Action_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(Action_Move, ETriggerEvent::Triggered, this, &AStalker_Character::On_Action_Move);
		EnhancedInputComponent->BindAction(Action_Look, ETriggerEvent::Triggered, this, &AStalker_Character::On_Action_Look);
		EnhancedInputComponent->BindAction(Action_Fire, ETriggerEvent::Triggered, this, &AStalker_Character::On_Action_Fire);
		EnhancedInputComponent->BindAction(Action_Use, ETriggerEvent::Triggered, this, &AStalker_Character::On_Action_Use);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
//------------------------------------------------------------------------------------------------------------
void AStalker_Character::On_Action_Move(const FInputActionValue &value)
{
	FVector2D movement_vector = value.Get<FVector2D>();  // Ввод - это Vector2D

	if (Controller != nullptr)
	{// Добавим движение

		AddMovementInput(GetActorForwardVector(), movement_vector.Y);
		AddMovementInput(GetActorRightVector(), movement_vector.X);
	}
}

void AStalker_Character::On_Action_Look(const FInputActionValue &value)
{
	FVector2D look_axis_vector = value.Get<FVector2D>();  // Ввод - это Vector2D

	if (Controller != nullptr)
	{// Добавим контроллеру Yaw и Pitch

		AddControllerYawInput(look_axis_vector.X);
		AddControllerPitchInput(look_axis_vector.Y);
	}
}
//------------------------------------------------------------------------------------------------------------
void AStalker_Character::On_Action_Fire(const FInputActionValue& value)
{
	if (Current_Weapon != 0)
		Current_Weapon->Fire(this);
}
//------------------------------------------------------------------------------------------------------------
void AStalker_Character::On_Action_Use(const FInputActionValue& value)
{
	int i;
	double min_distance, distance;
	AActor *item, *curr_item;
	FVector player_pos, item_pos;

	if (Interactable_Actors.Num() == 0)
		return;

	if (Interactable_Actors.Num() == 1)
	{
		item = Interactable_Actors[0];
		Interactable_Actors.RemoveAt(0);
	}
	else
	{
		player_pos = GetActorLocation();

		for (i = 0; i < Interactable_Actors.Num(); ++i)
		{
			curr_item = Interactable_Actors[i];

			item_pos = curr_item->GetActorLocation();

			distance = FVector::Distance(player_pos, item_pos);

			if (i == 0 || distance < min_distance)
			{
				min_distance = distance;
				item = curr_item;
			}
		}
		Interactable_Actors.Remove(item);
	}

	if (AWeapon* weapon = Cast<AWeapon>(item) )
		Pickup_Weapon(weapon);
}
//------------------------------------------------------------------------------------------------------------
void AStalker_Character::Pickup_Weapon(AWeapon* weapon)
{
	if (weapon == 0)
		return;

	if (Current_Weapon != 0)
		Current_Weapon->Detach();

	Current_Weapon = weapon;
	Current_Weapon->Attach(Mesh_1P);

	Has_Rifle = true;  // Устанавливаем Has_Rifle, чтобы анимационный блюпринт мог переключиться на другой набор анимаций
}
//------------------------------------------------------------------------------------------------------------
