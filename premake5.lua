workspace "PROJECTS"
	configurations {"Debug", "Release"}
	platforms {"Win64", "Static", "Shared"}
	toolset ("clang")
project "DOXYGRAPH"
	location "./"
	kind "ConsoleApp"
	language "C++"
	targetdir "./" --bin/%{cfg.buildcfg}"
	targetname "doxygraph"
	files {"**.hpp", "**.h", "**.c", "**.cpp", "**.cc"}
	includedirs{"include",
	"./include/SFML",
	"./vendor/imgui/include",
	"X:/DEV/boost_1_70_0",
	"vendor/doxygen/filesystem",
	"vendor/doxygen/jquery",
	"vendor/doxygen/liblodepng",
	"vendor/doxygen/libmd5",
	"vendor/doxygen/libmscgen",
	"vendor/doxygen/libversion", 
	"vendor/doxygen/libxml",
	"vendor/doxygen/vhdlparser",
	"vendor/doxygen/src",
	"vendor/doxygen/generated_src"}
	removefiles{}
	links{"glew32s",
	"opengl32",
	"sfml-graphics",
	"sfml-window",
	"sfml-system",
	"ole32",
	"oleaut32",
	"imm32",
	"version",
	"iconv",
	"pthread",
	"z",
	"boost_date_time-clang15-mt-s-x64-1_70",
	"boost_log-clang15-mt-s-x64-1_70",
	"boost_thread-clang15-mt-s-x64-1_70",
	"boost_regex-clang15-mt-s-x64-1_70",
	"boost_filesystem-clang15-mt-s-x64-1_70",
	"boost_log_setup-clang15-mt-s-x64-1_70",
	"boost_atomic-clang15-mt-s-x64-1_70",
	"boost_chrono-clang15-mt-s-x64-1_70"}
	linkoptions{"-lclang.dll"}
	libdirs{"lib", 
	"D:/DEV/boost_1_70_0/static_stage/lib",
	"D:/DEV/LLVM/lib"}
	--libdirs{os.findlib("OpenGL")}
	stl ("libc++")
	buildoptions {"-stdlib=libc++ "}
	filter "files:**.cpp"
		buildoptions {"-std=c++17"}
	filter "configurations:Debug"
		defines {"DEBUG"}
		symbols "On"
		buildoptions{"-g3", "-fdata-sections", "-ffunction-sections"}
	filter "configurations:Release"
		defines {}
		optimize "On"
		linkoptions {"-fno-exceptions", "-fno-rtti", "-fPIC"}
		buildoptions {"-fdata-sections", "-ffunction-sections", "-m64", "-Wall", "-Wextra", "-pthread"}
	filter { "platforms:Win64" }
    	system "Windows"
    	architecture "x86_64"
	filter { "platforms:Static" }
    	kind "StaticLib"
	filter { "platforms:DLL" }
    	kind "SharedLib"
    	defines { "DLL_EXPORTS" }
