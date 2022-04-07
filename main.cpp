#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <thread>
#include "boost/filesystem.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

#pragma warning(disable : 4996)

void runCommand(std::string command)
{
	std::system(command.c_str());
}

void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	std::size_t pos = data.find(toSearch);

	// Repeat till end is reached
	while (pos != std::string::npos) {
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}

std::string premakePath = "c:\\home\\dev\\sp\\premake";
std::string defPath = "c:\\home\\dev\\";
std::string templatesPath = "c:\\home\\dev\\sp\\templates\\";

void createProject(std::string projectName)
{
	char script[1024 * 50]{};
	std::sprintf(script,
		R"(workspace "%s"

architecture "x86"
location "../"
staticruntime "off"

configurations{
	"Debug",
	"Release",
}

platforms{
	"x86",
}

os.execute("mklink /J ..\\android\\app\\jni\\SDL 				..\\vendor\\SDL")
os.execute("mklink /J ..\\android\\app\\jni\\SDL_image 			..\\vendor\\SDL_image")
os.execute("mklink /J ..\\android\\app\\jni\\SDL_ttf 			..\\vendor\\SDL_ttf")
os.execute("mklink /J ..\\android\\app\\jni\\SDL_mixer 			..\\vendor\\SDL_mixer")
os.execute("mklink /J ..\\android\\app\\jni\\SDL_net 			..\\vendor\\SDL_net")
os.execute("mkdir ..\\android\\app\\src\\main\\assets")
os.execute("mklink /J ..\\android\\app\\src\\main\\assets\\res 	..\\res")

project "%s"
	location "../"
	language "C++"
	cppdialect "C++17"
	kind "ConsoleApp"

	targetdir "../bin/%%{cfg.buildcfg}_%%{cfg.platform}"
	objdir "../bin/obj/%%{cfg.buildcfg}_%%{cfg.platform}"

    includedirs{
        "../vendor/SDL/include",
        "../vendor/SDL_image/include",
        "../vendor/SDL_ttf/include",
        "../vendor/SDL_mixer/include",
        "../vendor/SDL_net/include",
        "../vendor/SDL_gpu/include",
        "../vendor/GLEW/include",
        "../vendor/GLM/include",
        "../vendor/SFML/include",
        "../vendor/PUGIXML/src",
        "../vendor/BOOST",
        "../vendor/STB",
		"../vendor/IMGUI",
        "../vendor/IMGUI/backends",
    }
   
    libdirs{
        "../vendor/SDL/lib/x86",
        "../vendor/SDL_image/lib/x86",
        "../vendor/SDL_ttf/lib/x86",
        "../vendor/SDL_mixer/lib/x86",
        "../vendor/SDL_net/lib/x86",
        "../vendor/SDL_gpu/lib/x86",
        "../vendor/GLEW/lib/x86",
        "../vendor/SFML/lib",
        "../vendor/BOOST/stage/lib",
    }

    links{
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"SDL2_ttf.lib",
		"SDL2_mixer.lib",
		"SDL2_net.lib",
		"SDL2_gpu.lib",
		"glew32.lib",
		"opengl32.lib",
		"sfml-system-s-d.lib",
		"sfml-window-s-d.lib",
		"sfml-graphics-s-d.lib",
		"sfml-main-d.lib",
		"sfml-audio-s-d.lib",
		"sfml-network-s-d.lib",
		"openal32.lib",
		"flac.lib",
		"vorbisenc.lib",
		"vorbisfile.lib",
		"vorbis.lib",
		"ogg.lib",
		"gdi32.lib",
		"freetype.lib",
		"winmm.lib",
		"ws2_32.lib",
    }

	files{
		"../main.cpp",
		"../vendor/PUGIXML/src/pugixml.cpp",
		"../vendor/IMGUI/imgui.cpp",
		"../vendor/IMGUI/imgui_demo.cpp",
		"../vendor/IMGUI/imgui_draw.cpp",
		"../vendor/IMGUI/imgui_tables.cpp",
		"../vendor/IMGUI/imgui_widgets.cpp",
		"../vendor/IMGUI/backends/imgui_impl_sdl.cpp",
		"../vendor/IMGUI/backends/imgui_impl_sdlrenderer.cpp",
		"../vendor/IMGUI/misc/cpp/imgui_stdlib.cpp",
	}

	defines "SFML_STATIC"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
			)",
		projectName.c_str(), projectName.c_str());
	{
		std::ofstream ofs(defPath + projectName + "\\premake\\premake5.lua");
		ofs << script;
	}
	{
		std::ifstream ifs(defPath + projectName + "\\main.cpp");
		std::stringstream ss;
		ss << ifs.rdbuf();
		ifs.close();
		std::string content = ss.str();
		findAndReplaceAll(content, "AppName_", projectName);
		std::ofstream ofs(defPath + projectName + "\\main.cpp");
		ofs << content;
	}
	{
		std::ifstream ifs(defPath + projectName + "\\android\\app\\src\\main\\res\\values\\strings.xml");
		std::stringstream ss;
		ss << ifs.rdbuf();
		ifs.close();
		std::string content = ss.str();
		findAndReplaceAll(content, "AppName_", projectName);
		std::ofstream ofs(defPath + projectName + "\\android\\app\\src\\main\\res\\values\\strings.xml");
		ofs << content;
	}
	std::string lowerCaseProjectName = projectName;
	std::transform(lowerCaseProjectName.begin(), lowerCaseProjectName.end(), lowerCaseProjectName.begin(),
		[](unsigned char c) { return std::tolower(c); });
	{
		std::ifstream ifs(defPath + projectName + "\\android\\app\\src\\main\\AndroidManifest.xml");
		std::stringstream ss;
		ss << ifs.rdbuf();
		ifs.close();
		std::string content = ss.str();
		findAndReplaceAll(content, "org.libsdl.app", "com.huberti." + lowerCaseProjectName);
		std::ofstream ofs(defPath + projectName + "\\android\\app\\src\\main\\AndroidManifest.xml");
		ofs << content;
	}
	boost::filesystem::create_directory(defPath + projectName + "\\android\\app\\src\\main\\java\\com\\huberti\\" + lowerCaseProjectName);
	{
		std::ofstream ofs(defPath + projectName + "\\android\\app\\src\\main\\java\\com\\huberti\\" + lowerCaseProjectName + "\\MyApp.java");
		char javaFile[1024 * 50]{};
		std::sprintf(javaFile,
			R"(package com.huberti.%s;
    
    import org.libsdl.app.SDLActivity; 
    
    /**
     * A sample wrapper class that just calls SDLActivity 
     */ 
    
    public class MyApp extends SDLActivity { }
)", lowerCaseProjectName.c_str());
		ofs << javaFile;
	}
	runCommand("cd " + defPath + projectName + "\\premake && premake5.exe vs2022 && cd.. && START /B " + projectName + ".sln");
}

int main(int argc, char* argv[])
{
	try {
		std::string projectName;
		std::cout << "Type project name: ";
		std::getline(std::cin, projectName);
		while (boost::filesystem::exists(defPath + projectName)) {
			std::cout << defPath + projectName << " already exists. Type project name: ";
			std::getline(std::cin, projectName);
		}
		std::string number;
		std::cout << "1. c++WithStdLiblaries" << std::endl << "2. c++WithStdAndExternalLiblaries" << std::endl << "3. website" << std::endl << "Type number: ";
		std::cin >> number;
		while (number != "1" && number != "2" && number != "3") {
			std::cout << "Unknown number. Type number: ";
			std::cin >> number;
		}
		// TODO: Error handling?
		// TODO: Use threads when copying to make it much more faster.
		if (number == "1") {
			boost::filesystem::create_directory(defPath + projectName);
			for (boost::filesystem::directory_entry entry : boost::filesystem::directory_iterator(templatesPath + "c++WithStdLiblaries")) {
				boost::filesystem::copy(entry.path(), defPath + projectName + (boost::filesystem::is_directory(entry) ? "\\" + entry.path().filename().string() : ""), boost::filesystem::copy_options::recursive);
			}
			char script[1024 * 50]{};
			std::sprintf(script,
				R"(workspace "%s"

architecture "x86"
location "../"
staticruntime "on"

configurations{
	"Debug",
	"Release",
}

platforms{
	"x86",
}

project "%s"
	location "../"
	language "C++"
	cppdialect "C++17"
	kind "ConsoleApp"

	targetdir "../bin/%%{cfg.buildcfg}_%%{cfg.platform}"
	objdir "../bin/obj/%%{cfg.buildcfg}_%%{cfg.platform}"

	files{
		"../main.cpp",
	}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
			)", projectName.c_str(), projectName.c_str());
			{
				std::ofstream ofs(defPath + projectName + "\\premake\\premake5.lua");
				ofs << script;
			}
			runCommand("cd " + defPath + projectName + "\\premake && premake5.exe vs2022 && START /B ..\\" + projectName + ".sln");
		}
		if (number == "2") {
			if (boost::filesystem::exists(defPath + "zProject1"))
			{
				boost::filesystem::rename(defPath + "zProject1", defPath + projectName);
				createProject(projectName);
			}
			else if (boost::filesystem::exists(defPath + "zProject2"))
			{
				boost::filesystem::rename(defPath + "zProject2", defPath + projectName);
				createProject(projectName);
			}
			else if (boost::filesystem::exists(defPath + "zProject3"))
			{
				boost::filesystem::rename(defPath + "zProject3", defPath + projectName);
				createProject(projectName);
			}
		}
		else if (number == "3")
		{
			boost::filesystem::create_directory(defPath + projectName);
			for (boost::filesystem::directory_entry entry : boost::filesystem::directory_iterator(templatesPath + "website")) {
				boost::filesystem::copy(entry.path(), defPath + projectName + (boost::filesystem::is_directory(entry) ? "\\" + entry.path().filename().string() : ""), boost::filesystem::copy_options::recursive);
			}
			runCommand("cd " + defPath + projectName + " && code index.html");
		}
	}
	catch (std::exception e) {
		MessageBoxA(0, e.what(), "Error", 0);
		std::cout << e.what() << std::endl;
	}
}