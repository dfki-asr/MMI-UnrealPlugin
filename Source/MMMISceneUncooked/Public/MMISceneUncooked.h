#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"

class FMMISceneUncookedModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
