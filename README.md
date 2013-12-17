HQEngine Game Framework
========
1. Prerequisites
----------------------------
- For Windows Desktop version:
	- OpenGL headers, libraries. 
	- NVIDIA Cg 3.1 headers, libraries. Best way is installing Cg Toolkit 3.1 from NVIDIA. 
	- OpenAL headers, libraries. Best way is installing OpenAL SDK from Creative Lab. 
	- DirectX SDK June 2010. 
	- Visual C++ 2008. 
- For Android:
	- Android SDK, NDK. 
	- Cygwin. 
- For Windows Metro/Phone version:
	- Visual Studio 2012. 
 
2. Folder structure
----------------------------
- HQEngine. (main framework folder)
	- Source. (Source code)
	- Android. (Android version)
	- Android-OpenAL-soft. (OpenAL library for Android)
	- VS. (Windows Desktop version, contains Visual Studio 2008 solution)
	- VS2012. (Windows Metro/Phone version)
		- WindowsStore. (Windows Store App version)
		- WindowsPhone. (Windows Phone 8 version)
- java2cpp. (Android framework's JNI wrapper headers)
- libogg. (lib ogg)
- libvorbis. (lib vorbis)

3. How to build
----------------------------
- Windows Desktop:
	- Open 3D Math library's solution "/HQEngine/VS/HQEngineUtilMath.sln".
	- Select Build->Batch Build->Build to build various versions (Debug/Release) of Math library.
	- Open game framework's solution "/HQEngine/VS/HQEngine.sln". Note: it's normal if there's a error popup saying that a C sharp project cannot be opened because you are using Visual C++ Express.
	- Config Prerequisites' include and libraries paths. 
	- Build solution.
	- Output dll and exe files are in "/HQEngine/VS/Output/Debug" folder. 
	- Run "test" project inside Visual Studio. Don't run "test.exe" directly in "/HQEngine/VS/Output/Debug" folder.
- Android:
	- *TO DO*
- Windows Store App:
	- *TO DO*
- Windows Phone 8:
	- *TO DO*
	