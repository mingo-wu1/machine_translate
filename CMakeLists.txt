cmake_minimum_required(VERSION 3.5.1)

# set the project name and version
project(web_server VERSION 0.1)
set(CMAKE_CXX_STANDARD 14)

###############################################

# find package
find_package(websocketpp REQUIRED)

# set grpc cmake profix path
#find_package(Protobuf REQUIRED)
#CMAKE_PREFIX_PATH是一个分号分隔的路径列表，用来指明软件/库安装路径前缀，以供find_package()，find_program()，find_library()，find_file()和find_path()命令搜索使用，这样就方便搜索可执行文件、头文件、库文件及相关配置文件等。
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_part/c-ares-1.17.2-Linux) # 安装路径前缀

# Proto file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/common.cmake)
get_filename_component(hw_proto "${CMAKE_CURRENT_SOURCE_DIR}/protos/helloworld.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)
# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.h")
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")
EXECUTE_PROCESS(      
      COMMAND python3 -m grpc_tools.protoc 
        --grpc_python_out "${CMAKE_CURRENT_BINARY_DIR}"
        --python_out "${CMAKE_CURRENT_BINARY_DIR}"
        --pyi_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I ../protos ../protos/helloworld.proto)
# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
# hw_grpc_proto
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
list(APPEND GRPC_SRC hw_grpc_proto ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})

##############################################

# include file and add source file
include_directories(include)
aux_source_directory(src SRC)

# gen bin and target include file
add_executable(grpc_server grpc_server.cpp ${SRC})
target_link_libraries(grpc_server pthread ${GRPC_SRC})

add_executable(web_server web_server.cpp ${SRC})
target_link_libraries(web_server pthread ${GRPC_SRC})
