cmake_minimum_required(VERSION 3.16)
project(Helix VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# --- vcpkg 패키지들 ---
find_package(unofficial-inih CONFIG REQUIRED)
find_package(mimalloc CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)

# mimalloc 타깃 선택(환경에 따라 이름이 다를 수 있음)
if(TARGET mimalloc-static)
  set(MI_TGT mimalloc-static)
elseif(TARGET mimalloc)
  set(MI_TGT mimalloc)
else()
  message(FATAL_ERROR "mimalloc target not found")
endif()

# --- Protobuf & gRPC 코드 생성 ---
set(PROTO_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/protos/server_control.proto
)

# protoc과 grpc_cpp_plugin 경로 가져오기
get_target_property(PROTOBUF_PROTOC_EXECUTABLE protobuf::protoc IMPORTED_LOCATION_RELEASE)
if(NOT PROTOBUF_PROTOC_EXECUTABLE)
  get_target_property(PROTOBUF_PROTOC_EXECUTABLE protobuf::protoc IMPORTED_LOCATION_DEBUG)
endif()

get_target_property(GRPC_CPP_PLUGIN gRPC::grpc_cpp_plugin IMPORTED_LOCATION_RELEASE)
if(NOT GRPC_CPP_PLUGIN)
  get_target_property(GRPC_CPP_PLUGIN gRPC::grpc_cpp_plugin IMPORTED_LOCATION_DEBUG)
endif()

# 생성된 파일 경로
set(PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/server_control.pb.cc")
set(PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/server_control.pb.h")
set(GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/server_control.grpc.pb.cc")
set(GRPC_HDRS "${CMAKE_CURRENT_BINARY_DIR}/server_control.grpc.pb.h")

# protobuf 컴파일 커스텀 커맨드
add_custom_command(
  OUTPUT ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS}
  COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
  ARGS --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
       --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
       -I ${CMAKE_CURRENT_SOURCE_DIR}/protos
       --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
       ${PROTO_FILES}
  DEPENDS ${PROTO_FILES}
  COMMENT "Generating gRPC code from proto files"
)

# --- HelixCore ---
add_library(HelixCore
  HelixCore/Logger.cpp
  HelixCore/Network.cpp
  HelixCore/SocketExtension.cpp
  HelixCore/Noncopyable.cpp
  HelixCore/Server.cpp
  HelixCore/ServerConfig.cpp
  HelixCore/INIData.cpp
  # HelixCore/AllocationOverride.cpp  # 임시로 비활성화
  HelixCore/ServerControlService.cpp

  # gRPC generated files
  ${PROTO_SRCS}
  ${PROTO_HDRS}
  ${GRPC_SRCS}
  ${GRPC_HDRS}

  # headers
  HelixCore/Logger.h
  HelixCore/Network.h
  HelixCore/SocketExtension.h
  HelixCore/Noncopyable.h
  HelixCore/Server.h
  HelixCore/ServerConfig.h
  HelixCore/IFileLoader.h
  HelixCore/ILoadable.h
  HelixCore/INIFileLoader.h
  HelixCore/INIData.h
  HelixCore/Types.h
  HelixCore/ServerControlService.h
)

target_include_directories(HelixCore
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/HelixCore
    ${CMAKE_CURRENT_BINARY_DIR}  # for generated proto headers
)

target_link_libraries(HelixCore
  PUBLIC
    unofficial::inih::inireader
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
  PRIVATE
    # ${MI_TGT}  # 임시로 비활성화
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
