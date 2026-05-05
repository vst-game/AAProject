//$ Copyright 2021, UsefulCode - All Rights Reserved $//

#include "SnappingHelper.h"
#include "FSnappingHelperCommands.h"
#include "SnappingHelperEdMode.h"
#include "SnappingHelperStyle.h"
#include "SSnappingHelperComboButton.h"

#include "LevelEditor.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FSnappingHelperModule"

void FSnappingHelperModule::StartupModule()
{
	FSnappingHelperStyle::Initialize();
	FSnappingHelperStyle::ReloadTextures();

	BindCommands();

	FEditorModeRegistry::Get().RegisterMode<FSnappingHelperEdMode>(FSnappingHelperEdMode::EM_SnappingHelperEdModeId, LOCTEXT("SnappingHelperEdModeName", "SnappingHelperEdMode"), FSlateIcon(), false);

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetGlobalLevelEditorActions()->Append(SnappingHelperActions.ToSharedRef());

	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	ToolbarExtender->AddToolBarExtension(
		"LocationGridSnap",
		EExtensionHook::Before,
		SnappingHelperActions,
		FToolBarExtensionDelegate::CreateRaw(this, &FSnappingHelperModule::CreateToolbarExtension));

	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);

	OnPerspectiveTypeActive.BindRaw(this, &FSnappingHelperModule::PerspectiveTypeChanged);
}

void FSnappingHelperModule::ShutdownModule()
{
	FSnappingHelperStyle::Shutdown();

	FEditorModeRegistry::Get().UnregisterMode(FSnappingHelperEdMode::EM_SnappingHelperEdModeId);
	OnPerspectiveTypeActive.Unbind();
}

void FSnappingHelperModule::BindCommands()
{
	FSnappingHelperCommands::Register();
	SnappingHelperActions = MakeShareable(new FUICommandList);

	SnappingHelperActions->MapAction(
		FSnappingHelperCommands::Get().SnappingHelper3D,
		FExecuteAction::CreateRaw(this, &FSnappingHelperModule::On3DModeToggle),
		FCanExecuteAction());

	SnappingHelperActions->MapAction(
		FSnappingHelperCommands::Get().SnappingHelper2D,
		FExecuteAction::CreateRaw(this, &FSnappingHelperModule::On2DModeToggle),
		FCanExecuteAction());
}

void FSnappingHelperModule::CreateToolbarExtension(FToolBarBuilder& InToolbarBuilder)
{
	InToolbarBuilder.BeginSection("SnappingHelper");

	InToolbarBuilder.AddWidget(SNew(SSnappingHelperComboButton)
		.Cursor(EMouseCursor::Default)
		.Is2DEnabled_Raw(this, &FSnappingHelperModule::Is2DModeCheckBoxEnabled)
		.Is2DChecked_Raw(this, &FSnappingHelperModule::Is2dModeChecked)
		.Is3DChecked_Raw(this, &FSnappingHelperModule::Is3dModeChecked)
		.On2DCheckStateChanged_Raw(this, &FSnappingHelperModule::On2DModeStateChanged)
		.On3DCheckStateChanged_Raw(this, &FSnappingHelperModule::On3DModeStateChanged)
	);

	InToolbarBuilder.EndSection();
}

bool FSnappingHelperModule::Is2DModeCheckBoxEnabled() const
{
	if (GEditor)
	{
		if (GEditor->GameViewport.Get())
		{
			return false;
		}

		const auto EditorViewportClient = static_cast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());

		const bool bCurrentPerspectiveState {EditorViewportClient->IsPerspective()};
		if (!bIsPerspectiveType && bCurrentPerspectiveState)
		{
			OnPerspectiveTypeActive.ExecuteIfBound(true);
		}
		else if(bIsPerspectiveType && !bCurrentPerspectiveState)
		{
			OnPerspectiveTypeActive.ExecuteIfBound(false);
		}

		return !bCurrentPerspectiveState;
	}

	return false;
}

ECheckBoxState FSnappingHelperModule::Is2dModeChecked() const
{
	ECheckBoxState OutState;
	GLevelEditorModeTools().IsModeActive(FSnappingHelperEdMode::EM_SnappingHelperEdModeId) ? OutState = ECheckBoxState::Checked : OutState = ECheckBoxState::Unchecked;

	if (const FSnappingHelperEdMode* EdMode {GetSnappingHelperEdMode()})
	{
		EdMode->bIs3DMode ? OutState = ECheckBoxState::Unchecked : OutState = ECheckBoxState::Checked;
	}

	return OutState;
}

void FSnappingHelperModule::On2DModeStateChanged(ECheckBoxState InState)
{
	if (InState == ECheckBoxState::Checked)
	{
		ActivateEdMode2D();
	}
	else
	{
		DeactivateEdMode();
	}
}

void FSnappingHelperModule::On2DModeToggle()
{
	if (Is2DModeCheckBoxEnabled())
	{
		if (Is2dModeChecked() == ECheckBoxState::Unchecked)
		{
			ActivateEdMode2D();
		}
		else
		{
			DeactivateEdMode();
		}
	}
}

void FSnappingHelperModule::ActivateEdMode2D() const
{
	GLevelEditorModeTools().ActivateMode(FSnappingHelperEdMode::EM_SnappingHelperEdModeId);
	if (FSnappingHelperEdMode* EdMode {GetSnappingHelperEdMode()})
	{
		EdMode->bIs3DMode = false;
	}
}

ECheckBoxState FSnappingHelperModule::Is3dModeChecked() const
{
	ECheckBoxState OutState;
	GLevelEditorModeTools().IsModeActive(FSnappingHelperEdMode::EM_SnappingHelperEdModeId) ? OutState = ECheckBoxState::Checked : OutState = ECheckBoxState::Unchecked;

	if (const FSnappingHelperEdMode* EdMode {GetSnappingHelperEdMode()})
	{
		EdMode->bIs3DMode ? OutState = ECheckBoxState::Checked : OutState = ECheckBoxState::Unchecked;
	}

	return OutState;
}

void FSnappingHelperModule::On3DModeStateChanged(ECheckBoxState InState)
{
	if (InState == ECheckBoxState::Checked)
	{
		ActivateEdMode3D();
	}
	else
	{
		DeactivateEdMode();
	}
}

void FSnappingHelperModule::On3DModeToggle()
{
	if (Is3dModeChecked() == ECheckBoxState::Unchecked)
	{
		ActivateEdMode3D();
	}
	else
	{
		DeactivateEdMode();
	}
}

void FSnappingHelperModule::ActivateEdMode3D() const
{
	GLevelEditorModeTools().ActivateMode(FSnappingHelperEdMode::EM_SnappingHelperEdModeId);
	if (FSnappingHelperEdMode* EdMode {GetSnappingHelperEdMode()})
	{
		EdMode->bIs3DMode = true;
	}
}

void FSnappingHelperModule::DeactivateEdMode()
{
	GLevelEditorModeTools().DeactivateMode(FSnappingHelperEdMode::EM_SnappingHelperEdModeId);
}

void FSnappingHelperModule::PerspectiveTypeChanged(const bool bIsPerspectiveActive)
{
	bIsPerspectiveType = bIsPerspectiveActive;
	if (bIsPerspectiveType)
	{
		if (Is2dModeChecked() == ECheckBoxState::Checked)
		{
			ActivateEdMode3D();
		}
	}
}

FSnappingHelperEdMode* FSnappingHelperModule::GetSnappingHelperEdMode() const
{
	return static_cast<FSnappingHelperEdMode*>(GLevelEditorModeTools().GetActiveMode(FSnappingHelperEdMode::EM_SnappingHelperEdModeId));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSnappingHelperModule, SnappingHelper)