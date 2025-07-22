cmake_minimum_required(VERSION 3.16)
project(HelixServer)

set(CMAKE_CXX_STANDARD 20)

# Core 라이브러리
add_library(HelixCore
    HelixCore/Logger.cpp
    HelixCore/Network.cpp
    HelixCore/Noncopable.cpp
    HelixCore/Server.cpp
    HelixCore/Util/INIReader.cpp

    HelixCore/Logger.h
    HelixCore/Network.h
    HelixCore/Noncopable.h
    HelixCore/Server.h
    HelixCore/Types.h
    HelixCore/Util/INIReader.h
)

# 서버 실행 파일
add_executable(GameServer
    GameServer/main.cpp
    GameServer/GameServer.cpp
    GameServer/GameServer.h
)

target_link_libraries(GameServer PRIVATE HelixCore)