cmake_minimum_required(VERSION 3.16)
project(HelixServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# Core 라이브러리
add_library(HelixCore
    HelixCore/Logger.cpp
    HelixCore/Network.cpp
    HelixCore/Noncopyable.cpp
    HelixCore/Server.cpp
    ThirdParty/inih/cpp/INIReader.cpp

    HelixCore/Logger.h
    HelixCore/Network.h
    HelixCore/Noncopyable.h
    HelixCore/Server.h
    HelixCore/Types.h
    ThirdParty/inih/cpp/INIReader.h
)

target_include_directories(HelixCore
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/HelixCore
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/inih
		${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/inih/cpp
)

add_library(Helix::Core ALIAS HelixCore)

# 서버 실행 파일
add_executable(GameServer
    GameServer/main.cpp
    GameServer/GameServer.cpp
    GameServer/GameServer.h
)

target_link_libraries(GameServer PRIVATE HelixCore)