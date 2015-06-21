// Copyright(c) 2015, Michael Allar
// All rights reserved.

namespace UnrealBuildTool.Rules
{
	public class SpoutPlugin : ModuleRules
	{
		public SpoutPlugin(TargetInfo Target)
		{
			AddThirdPartyPrivateStaticDependencies(Target, "Spout");

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
                    "Engine",
                    "RHI",
                    "RenderCore",
					// ... add other public dependencies that you statically link with here ...
				}
				);
		}
	}
}