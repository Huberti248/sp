workspace "SpCreator"

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

project "SpCreator"
	location "../"
	language "C++"
	cppdialect "C++17"
	kind "ConsoleApp"

	targetdir "../bin/%{cfg.buildcfg}_%{cfg.platform}"
	objdir "../bin/obj/%{cfg.buildcfg}_%{cfg.platform}"

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
			