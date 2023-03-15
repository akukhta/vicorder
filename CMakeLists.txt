cmake_minimum_required(VERSION 3.5)

project(vicorder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CONAN_DISABLE_CHECK_COMPILER ON)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR};${CMAKE_MODULE_PATH}")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(${PROJECT_NAME} SHARED 
	include/vicorder/TaskVisitor.h
	include/vicorder/TOTask.h
	include/vicorder/TOTOutputTask.h
	include/vicorder/VRBuilder.h
	include/vicorder/VRBuilderDirector.h
	include/vicorder/OutputTask.h
	include/vicorder/VideoRecorder.h

	src/API/ExternalStorage.h
	src/API/ExternalStorage.cpp
	src/API/ExternalStorageManager.h
	src/API/ExternalStorageManager.cpp
	src/API/VideoRecorder.cpp
	src/common/AviCreateTask.h
	src/common/CaptureTask.h
	src/common/CommonUtiles.h
	src/common/CompressingTask.h
	src/common/dllmain.cpp
	src/common/FrameToDiskTask.h
	src/common/framework.h
	src/common/pch.cpp
	src/common/pch.h
	src/common/TaskVisitor.cpp
	src/common/VideoOutput.h
	src/common/dllinterface.h
)

target_compile_definitions(vicorder PUBLIC -DVICORDER_EXPORTS=1)
target_link_libraries(${PROJECT_NAME}  ${CONAN_LIBS})