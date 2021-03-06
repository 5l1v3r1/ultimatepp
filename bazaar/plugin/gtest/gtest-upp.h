#ifndef GTEST_INCLUDE_GTEST_UPP_H
#define GTEST_INCLUDE_GTEST_UPP_H

#include <Core/config.h>

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)

	#define TEST_APP_MAIN \
	namespace Upp { extern bool PanicMode; } \
	\
	void TestMainFn(int argc, char** argv); \
	\
	int main(int argc, char** argv) \
	{ \
		Upp::PanicMode = true; \
	\
		testing::InitGoogleTest(&argc, argv); \
		TestMainFn(argc, argv); \
		return RUN_ALL_TESTS(); \
	} \
	\
	void TestMainFn(int argc, char** argv)
	
#else

	#define TEST_APP_MAIN \
	void TestMainFn(int argc, char** argv); \
	\
	int main(int argc, char** argv) \
	{ \
		testing::InitGoogleTest(&argc, argv); \
		TestMainFn(argc, argv); \
		return RUN_ALL_TESTS(); \
	} \
	\
	void TestMainFn(int argc, char** argv)
	
#endif

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	#define TEST_GUI_APP_MAIN \
	void TestGuiMainFn(); \
	\
	int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) \
	{ \
		UPP::AppInitEnvironment__(); \
		UPP::Ctrl::InitWin32(hInstance); \
		testing::InitGoogleTest(&__argc, __argv); \
		int testsResult = RUN_ALL_TESTS(); \
		UPP::Ctrl::ExitWin32(); \
		UPP::AppExit__(); \
		return testsResult; \
	} \
	void TestGuiMainFn()

#elif defined(PLATFORM_X11)
	
	#define TEST_GUI_APP_MAIN \
	void TestGuiMainFn(); \
	\
	int main(int argc, char **argv, const char **envptr) { \
		UPP::AppInit__(argc, (const char **)argv, envptr); \
		UPP::InitGtkApp(argc, argv, envptr); \
		testing::InitGoogleTest(&argc, argv); \
		int testsResult = RUN_ALL_TESTS(); \
		UPP::ExitGtkApp(); \
		UPP::AppExit__(); \
		return testsResult; \
	} \
	void TestGuiMainFn()

#endif


#endif 
