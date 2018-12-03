project(BlockProject::Framework)
cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 11)

set(SRC_FRAMEWORK
	./include/Framework/API.hpp
	./include/Framework/Array.hpp
	./include/Framework/Array.impl.hpp
	./include/Framework/Math/BMath.hpp
	./include/Framework/Class.hpp
	./include/Framework/Color.hpp
	./include/Framework/Delegate.hpp
	./include/Framework/EntryPoint.hpp
	./include/Framework/Exception.hpp
	./include/Framework/Factory.hpp
	./include/Framework/IO/File.hpp
	./include/Framework/IO/FileSystem.hpp
	./include/Framework/Framework.hpp
	./include/Framework/Hash.hpp
	./include/Framework/Iterator.hpp
	./include/Framework/List.hpp
	./include/Framework/List.impl.hpp
	./include/Framework/Logger.hpp
	./include/Framework/Map.hpp
	./include/Framework/Map.impl.hpp
	./include/Framework/Math/Matrix.hpp
	./include/Framework/System/Memory.hpp
	./include/Framework/System/Module.hpp
	./include/Framework/System/ModuleManager.hpp
	./include/Framework/Network/Network.hpp
	./include/Framework/Object.hpp
	./include/Framework/Path.hpp
	./include/Framework/System/Platform.hpp
	./include/Framework/Profiler.hpp
	./include/Framework/Math/Quaternion.hpp
	./include/Framework/Random.hpp
	./include/Framework/RuntimeException.hpp
	./include/Framework/Stack.hpp
	./include/Framework/Stack.impl.hpp
	./include/Framework/IO/Stream.hpp
	./include/Framework/String.hpp
	./include/Framework/System/System.hpp
	./include/Framework/Math/Transform.hpp
	./include/Framework/TreeNode.hpp
	./include/Framework/Math/Triangle.hpp
	./include/Framework/Math/Vector.hpp
	./include/Framework/Math/Vertex.hpp
	./include/Framework/Math/Viewport.hpp
	./src/Framework/Math/BMath.cpp
	./src/Framework/Color.cpp
	./src/Framework/Exception.cpp
	./src/Framework/IO/File.cpp
	./src/Framework/Logger.cpp
	./src/Framework/Math/Matrix.cpp
	./src/Framework/System/Memory.cpp
	./src/Framework/System/Module.cpp
	./src/Framework/System/ModuleManager.cpp
	./src/Framework/Object.cpp
	./src/Framework/Path.cpp
	./src/Framework/System/Platform.cpp
	./src/Framework/Profiler.cpp
	./src/Framework/Math/Quaternion.cpp
	./src/Framework/Random.cpp
	./src/Framework/RuntimeException.cpp
	./src/Framework/IO/Stream.cpp
	./src/Framework/String.cpp
	./src/Framework/System/System.cpp
	./src/Framework/Math/Transform.cpp
	./src/Framework/Math/Triangle.cpp
	./src/Framework/Math/Vector.cpp
	./src/Framework/Math/Vertex.cpp
)

add_library(BPFramework SHARED ${SRC_FRAMEWORK})
if (WIN32)
	target_link_libraries(BPFramework PRIVATE pthread)
endif (WIN32)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             PREFIX "BPFramework"
             FILES ${SRC_FRAMEWORK})
