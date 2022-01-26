#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <thread>

#pragma warning(disable : 4996)

namespace fs = std::filesystem;

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

int main(int argc, char* argv[])
{
	try {
		std::string premakePath = "c:\\home\\dev\\sp\\premake";
		std::string defPath = "c:\\home\\dev\\";
		std::string templatesPath = "c:\\home\\dev\\sp\\templates\\";
		std::string projName;
		std::cout << "Type project name (may include \\ to put in subfolder): ";
		std::getline(std::cin, projName);
		while (fs::exists(defPath + projName)) {
			std::cout << defPath + projName << " already exists. Type project name: ";
			std::getline(std::cin, projName);
		}
		std::string number;
		std::cout << "1. basic" << std::endl << "2. full" << std::endl << "3. website" << std::endl << "Type number: ";
		std::cin >> number;
		while (number != "1" && number != "2" && number != "3") {
			std::cout << "Unknown number. Type number: ";
			std::cin >> number;
		}
		// TODO: Error handling?
		// TODO: Instead of hard codeing numbers and folder names one could just scan templates directory to get them.
		// TODO: Use threads when copying to make it much more faster.
		fs::create_directory(defPath + projName);
		if (number == "1") {
			for (auto entry : fs::directory_iterator(templatesPath + "basic")) {
				fs::copy(entry.path(), defPath + projName + (entry.is_directory() ? "\\" + entry.path().filename().string() : ""), fs::copy_options::recursive);
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
			)", projName.c_str(), projName.c_str());
			{
				std::ofstream ofs(defPath + projName + "\\premake\\premake5.lua");
				ofs << script;
			}
			runCommand("cd " + defPath + projName + "\\premake && premake5.exe vs2019 && START /B ..\\" + projName + ".sln");
		}
		if (number == "2") {
			for (auto entry : fs::directory_iterator(templatesPath + "full")) {
				fs::copy(entry.path(), defPath + projName + (entry.is_directory() ? "\\" + entry.path().filename().string() : ""), fs::copy_options::recursive);
			}
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
	}

	defines "SFML_STATIC"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
			)", projName.c_str(), projName.c_str());
			{
				std::ofstream ofs(defPath + projName + "\\premake\\premake5.lua");
				ofs << script;
			}
			{
				std::ifstream ifs(defPath + projName + "\\main.cpp");
				std::stringstream ss;
				ss << ifs.rdbuf();
				ifs.close();
				std::string content = ss.str();
				findAndReplaceAll(content, "AppName_", projName);
				std::ofstream ofs(defPath + projName + "\\main.cpp");
				ofs << content;
			}
			{
				std::ifstream ifs(defPath + projName + "\\android\\app\\src\\main\\res\\values\\strings.xml");
				std::stringstream ss;
				ss << ifs.rdbuf();
				ifs.close();
				std::string content = ss.str();
				findAndReplaceAll(content, "AppName_", projName);
				std::ofstream ofs(defPath + projName + "\\android\\app\\src\\main\\res\\values\\strings.xml");
				ofs << content;
			}
#if 0 // NOTE: App stops to work when it's turned on
			std::string lowerCaseProjName = projName;
			std::transform(lowerCaseProjName.begin(), lowerCaseProjName.end(), lowerCaseProjName.begin(),
				[](unsigned char c) { return std::tolower(c); });
			{
				std::ifstream ifs(defPath + projName + "\\android\\app\\src\\main\\java\\com\\nextcode\\AppName_\\MyApp.java");
				std::stringstream ss;
				ss << ifs.rdbuf();
				ifs.close();
				std::string content = ss.str();
				findAndReplaceAll(content, "AppName_", lowerCaseProjName);
				std::ofstream ofs(defPath + projName + "\\android\\app\\src\\main\\java\\com\\nextcode\\AppName_\\MyApp.java");
				ofs << content;
			}
			fs::rename(defPath + projName + "\\android\\app\\src\\main\\java\\com\\nextcode\\AppName_", defPath + projName + "\\android\\app\\src\\main\\java\\com\\nextcode\\" + lowerCaseProjName);
#endif
#if 1
			runCommand("cd " + defPath + projName + "\\premake && premake5.exe vs2019 && START /B ..\\" + projName + ".sln");
#else // NOTE: With git -> but it takes much more time
			runCommand("cd " + defPath + projName + "\\premake && premake5.exe vs2019 && cd.. && git init && git add * && git commit -a -m \"Initial commit\" && START /B ..\\" + projName + ".sln");
#endif
		}
		else if (number == "3")
		{
			for (auto entry : fs::directory_iterator(templatesPath + "website")) {
				fs::copy(entry.path(), defPath + projName + (entry.is_directory() ? "\\" + entry.path().filename().string() : ""), fs::copy_options::recursive);
			}
			runCommand("cd " + defPath + projName + " && code index.html");
		}
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}