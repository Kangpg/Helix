cmake_minimum_required(VERSION 3.16)
project(Helix)  # 솔루션 이름

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# --- vcpkg 패키지들 ---
find_package(unofficial-inih CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(mimalloc CONFIG REQUIRED)

# mimalloc 타깃 선택(환경에 따라 이름이 다를 수 있음)
if(TARGET mimalloc-static)
  set(MI_TGT mimalloc-static)
elseif(TARGET mimalloc)
  set(MI_TGT mimalloc)
else()
  message(FATAL_ERROR "mimalloc target not found")
endif()

# --- HelixCore ---
add_library(HelixCore
  HelixCore/Logger.cpp
  HelixCore/Network.cpp
  HelixCore/Noncopyable.cpp
  HelixCore/Server.cpp
  HelixCore/ServerConfig.cpp
  HelixCore/INIData.cpp
  HelixCore/AllocationOverride.cpp  # ← 전역 new/delete 오버라이드
  # headers
  HelixCore/Logger.h
  HelixCore/Network.h
  HelixCore/Noncopyable.h
  HelixCore/Server.h
  HelixCore/ServerConfig.h
  HelixCore/IFileLoader.h
  HelixCore/ILoadable.h
  HelixCore/INIFileLoader.h
  HelixCore/INIData.h
  HelixCore/Types.h
)

target_include_directories(HelixCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/HelixCore)

target_link_libraries(HelixCore
  PUBLIC
    unofficial::inih::inireader
  PRIVATE
    CURL::libcurl
    ${MI_TGT}
    # Windows system libs (pragma 대신 CMake로 연결)
    ws2_32
    mswsock
    crypt32
    dbghelp
)

add_library(Helix::Core ALIAS HelixCore)

# --- Executable ---
add_executable(Helix
  GameServer/main.cpp
  GameServer/GameServer.cpp
  GameServer/GameServer.h
)

target_link_libraries(Helix PRIVATE HelixCore)

# VS에서 기본 시작 프로젝트
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Helix)
