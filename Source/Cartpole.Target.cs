// © 2023 Kaya Adrian.

using UnrealBuildTool;


// ReSharper disable once InconsistentNaming
public class CartpoleTarget : TargetRules
{
	public CartpoleTarget(TargetInfo Target) : base(Target!)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		ExtraModuleNames.AddRange(new[]
		{
			"Cartpole"
		});
	}
}
