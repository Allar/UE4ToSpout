# UE4ToSpout
Sends desired render target texture to Spout if UE4 is running in OpenGL mode

# Requirements
1. Unreal Engine 4 4.8+ built from C++ Source on Github
1. Unreal Engine 4 4.8+ C++ Project (Could be blank C++ template project)
1. Visual Studio 2013, Express should work, recommend Community Edition (its free, yo)
1. Willingness to put up with instability

# Installation
1. Merge repo's Engine folder with your Engine folder, there shouldn't be any overwrites on first merge
1. Compile Spout SDK for Win32 and Win64 platforms
1. Put Spout SDK .lib and .dll files in Engine\Source\ThirdParty\Spout into their respective platform folders
1. Put a copy of those same .dll files in Engine\Binaries\ into their respective platform folders
1. Add a reference to SpoutPlugin in your Unreal Project Build file
1. Run GenerateProjectFiles.bat in your Unreal root folder
1. Compile Unreal Engine 4 and your project
1. Make sure to open your project in OpenGL mode by adding the -opengl command line to your editor
1. At some point during a level play, create a render target using the Create Render Target 2D With One Copy Blueprint node
1. Feed whatever you want into that render target texture
1. Be sure to call Set Spout Sender Texture Source with the render target you created
