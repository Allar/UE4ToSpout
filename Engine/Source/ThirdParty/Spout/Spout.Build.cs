// Copyright(c) 2015, Michael Allar
// All rights reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class Spout : ModuleRules
{
	public Spout(TargetInfo Target)
	{
		Type = ModuleType.External;

		PublicIncludePaths.Add(UEBuildConfiguration.UEThirdPartySourceDirectory + "Spout/include");
		
		//@TODO: Solve requiring Spout.dll in Engine\Binaries folder

        if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            PublicLibraryPaths.Add(UEBuildConfiguration.UEThirdPartySourceDirectory + "Spout/Win32");
            PublicAdditionalLibraries.Add("Spout.lib");

            string SpoutBinariesDir = String.Format("$(EngineDir)/Binaries/ThirdParty/Spout/Win32/");
            RuntimeDependencies.Add(new RuntimeDependency(SpoutBinariesDir + "Spout.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicLibraryPaths.Add(UEBuildConfiguration.UEThirdPartySourceDirectory + "Spout/Win64");
            PublicAdditionalLibraries.Add("Spout.lib");

            string SpoutBinariesDir = String.Format("$(EngineDir)/Binaries/ThirdParty/Spout/Win64/");
            RuntimeDependencies.Add(new RuntimeDependency(SpoutBinariesDir + "Spout.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
			//PublicAdditionalLibraries.Add(UEBuildConfiguration.UEThirdPartySourceDirectory + "portmidi/lib/Mac/libportmidi.a");
			//PublicAdditionalFrameworks.Add( new UEBuildFramework( "CoreAudio" ));
			//PublicAdditionalFrameworks.Add( new UEBuildFramework( "CoreMIDI" ));
        }
	}
}
