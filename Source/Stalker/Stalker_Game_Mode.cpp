#include "Stalker_Game_Mode.h"
#include "Stalker_Character.h"
#include "UObject/ConstructorHelpers.h"

//------------------------------------------------------------------------------------------------------------
AStalker_Game_Mode::AStalker_Game_Mode()
: Super()
{
	// Установим класс для pawn по умолчанию в класс персонажа, определённого а блюпринте
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}
//------------------------------------------------------------------------------------------------------------
