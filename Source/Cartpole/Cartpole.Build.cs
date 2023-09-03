// © 2023 Kaya Adrian.

using UnrealBuildTool;


// ReSharper disable once InconsistentNaming
public class Cartpole : ModuleRules
{
	public Cartpole(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseRTTI = true;
		bEnableExceptions = true;
	
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Atum",
			"Core",
			"CoreUObject",
			"Engine",
			"LibTorch"
		});
	}
}
