using UnrealBuildTool;

public class ProjectAAClient : ModuleRules
{
    public ProjectAAClient(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
	    PublicIncludePaths.AddRange(new string[] { "ProjectAAClient" });
        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "ProjectAA",
                "UMG",
                "GameplayTags"
            }
        );
    }
}