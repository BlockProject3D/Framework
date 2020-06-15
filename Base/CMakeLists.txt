cmake_minimum_required(VERSION 3.10)
project(BPF)

include("${CMAKE_CURRENT_SOURCE_DIR}/../CMakes/Framework.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../Version.cmake")

set(SOURCES
    ./include/Framework/IO/File.hpp
    ./include/Framework/IO/EStringSerializer.hpp
    ./include/Framework/IO/EStringEncoder.hpp
    ./include/Framework/IO/FileStream.hpp
    ./include/Framework/IO/MemoryMapper.hpp
    ./include/Framework/IO/IOException.hpp
    ./include/Framework/IO/IInputStream.hpp
    ./include/Framework/IO/IOutputStream.hpp
    ./include/Framework/IO/IDataInputStream.hpp
    ./include/Framework/IO/IDataOutputStream.hpp
    ./include/Framework/IO/ByteBuf.hpp
    ./include/Framework/IO/DynamicByteBuf.hpp
    ./include/Framework/IO/BinaryReader.hpp
    ./include/Framework/IO/BinaryWriter.hpp
    ./include/Framework/IO/TextReader.hpp
    ./include/Framework/IO/TextWriter.hpp
    ./include/Framework/IO/Console.hpp
    ./include/Framework/IO/ConsoleReader.hpp
    ./include/Framework/IO/ConsoleWriter.hpp
    ./include/Framework/IO/EConsoleColor.hpp
    ./include/Framework/IO/Stringifier.File.hpp
    ./include/Framework/Math/Vector.hpp
    ./include/Framework/Math/Vector.impl.hpp
    ./include/Framework/Math/Vector2.hpp
    ./include/Framework/Math/Vector2.impl.hpp
    ./include/Framework/Math/Vector3.hpp
    ./include/Framework/Math/Vector3.impl.hpp
    ./include/Framework/Math/Vector4.hpp
    ./include/Framework/Math/Vector4.impl.hpp
    ./include/Framework/Math/Viewport.hpp
    ./include/Framework/Math/Viewport.impl.hpp
    ./include/Framework/Math/Quaternion.hpp
    ./include/Framework/Math/Quaternion.impl.hpp
    ./include/Framework/Math/Matrix.hpp
    ./include/Framework/Math/Matrix.impl.hpp
    ./include/Framework/Math/Transform2.hpp
    ./include/Framework/Math/Transform2.impl.hpp
    ./include/Framework/Math/Transform3.hpp
    ./include/Framework/Math/Transform3.impl.hpp
    ./include/Framework/Math/Polygon2.hpp
    ./include/Framework/Math/Polygon2.impl.hpp
    ./include/Framework/Math/Polygon3.hpp
    ./include/Framework/Math/Polygon3.impl.hpp
    ./include/Framework/Math/Stringifier.Matrix.hpp
    ./include/Framework/Math/Stringifier.Vector.hpp
    ./include/Framework/Math/Stringifier.Quaternion.hpp
    ./include/Framework/Math/Stringifier.Color.hpp
    ./include/Framework/Math/Math.hpp
    ./include/Framework/Math/MathUtils.hpp
    ./include/Framework/Math/BoundingBox.hpp
    ./include/Framework/Math/IncompatibleMatrixSizeException.hpp
    ./include/Framework/Math/NonSquareMatrixException.hpp
    ./include/Framework/Math/NonInvertibleMatrixException.hpp
    ./include/Framework/Math/Color.hpp
    ./include/Framework/Log/Logger.hpp
    ./include/Framework/Log/ILogHandler.hpp
    ./include/Framework/Log/ELogLevel.hpp
    ./include/Framework/Log/DefaultLogger.hpp
    ./include/Framework/Log/FileLogger.hpp
    ./include/Framework/Memory/ClassCastException.hpp
    ./include/Framework/Memory/Memory.hpp
    ./include/Framework/Memory/MemUtils.hpp
    ./include/Framework/Memory/Memory.Hash.hpp
    ./include/Framework/Memory/Utility.hpp
    ./include/Framework/Memory/MemoryException.hpp
    ./include/Framework/Memory/SharedPtr.hpp
    ./include/Framework/Memory/SharedPtr.impl.hpp
    ./include/Framework/Memory/UniquePtr.hpp
    ./include/Framework/Memory/UniquePtr.impl.hpp
    ./include/Framework/Memory/WeakPtr.hpp
    ./include/Framework/Memory/WeakPtr.impl.hpp
    ./include/Framework/Memory/Object.hpp
    ./include/Framework/Memory/ObjectFactory.hpp
    ./include/Framework/Memory/IObjectConstructor.hpp
    ./include/Framework/Memory/ObjectPtr.hpp
    ./include/Framework/Network/Network.hpp
    ./include/Framework/System/Module.hpp
    ./include/Framework/System/ModuleException.hpp
    ./include/Framework/System/OSException.hpp
    ./include/Framework/System/ModuleManager.hpp
    ./include/Framework/System/ModuleManager.impl.hpp
    ./include/Framework/System/Platform.hpp
    ./include/Framework/System/TypeExpander.hpp
    ./include/Framework/System/DateTime.hpp
    ./include/Framework/System/TimeSpan.hpp
    ./include/Framework/System/Timer.hpp
    ./include/Framework/System/Thread.hpp
    ./include/Framework/System/Mutex.hpp
    ./include/Framework/System/ThreadManager.hpp
    ./include/Framework/System/ModuleInterface.hpp
    ./include/Framework/System/IApplication.hpp
    ./include/Framework/System/ScopeLock.hpp
    ./include/Framework/System/Paths.hpp
    ./include/Framework/System/Stringifier.DateTime.hpp
    ./include/Framework/System/EntryPoint.hpp
    ./include/Framework/Collection/Iterator.hpp
    ./include/Framework/Collection/Array.hpp
    ./include/Framework/Collection/Array.impl.hpp
    ./include/Framework/Collection/ArrayList.hpp
    ./include/Framework/Collection/ArrayList.impl.hpp
    ./include/Framework/Collection/List.hpp
    ./include/Framework/Collection/List.impl.hpp
    ./include/Framework/Collection/HashMap.hpp
    ./include/Framework/Collection/HashMap.impl.hpp
    ./include/Framework/Collection/Map.hpp
    ./include/Framework/Collection/Map.impl.hpp
    ./include/Framework/Collection/Stack.hpp
    ./include/Framework/Collection/Stack.impl.hpp
    ./include/Framework/Collection/Queue.hpp
    ./include/Framework/Collection/Queue.impl.hpp
    ./include/Framework/Collection/PriorityQueue.hpp
    ./include/Framework/Collection/PriorityQueue.impl.hpp
    ./include/Framework/Collection/Utility.hpp
    ./include/Framework/Collection/Stringifier.List.hpp
    ./include/Framework/Collection/Stringifier.ArrayList.hpp
    ./include/Framework/Collection/Stringifier.Array.hpp
    ./include/Framework/Collection/Stringifier.HashMap.hpp
    ./include/Framework/Collection/Stringifier.Map.hpp
    ./include/Framework/Collection/StackException.hpp
    ./include/Framework/Json/Json.hpp
    ./include/Framework/Json/Lexer.hpp
    ./include/Framework/Json/Parser.hpp
    ./include/Framework/Json/Writer.hpp
    ./include/Framework/Json/JsonException.hpp
    ./include/Framework/Json/JsonParseException.hpp
    ./include/Framework/API.hpp
    ./include/Framework/BinUtils.hpp
    ./include/Framework/Types.hpp
    ./include/Framework/Tuple.hpp
    ./include/Framework/TypeInfo.hpp
    ./include/Framework/Delegate.hpp
    ./include/Framework/Hash.hpp
    ./include/Framework/Hash.Base.hpp
    ./include/Framework/Profiler.hpp
    ./include/Framework/Math/Random.hpp
    ./include/Framework/Exception.hpp
    ./include/Framework/RuntimeException.hpp
    ./include/Framework/IndexException.hpp
    ./include/Framework/EvalException.hpp
    ./include/Framework/String.hpp
    ./include/Framework/Name.hpp
    ./include/Framework/MathEval.hpp
    ./include/Framework/MathEval.impl.hpp
    ./include/Framework/BaseConvert.hpp
    ./include/Framework/BaseConvert.impl.hpp
    ./include/Framework/Scalar.hpp
    ./include/Framework/ParseException.hpp
    ./src/Framework/IO/File.cpp
    ./src/Framework/IO/OSPrivate.hpp
    ./src/Framework/IO/OSPrivate.cpp
    ./src/Framework/IO/FileStream.cpp
    ./src/Framework/IO/MemoryMapper.cpp
    ./src/Framework/IO/ByteBuf.cpp
    ./src/Framework/IO/DynamicByteBuf.cpp
    ./src/Framework/IO/BinaryReader.cpp
    ./src/Framework/IO/BinaryWriter.cpp
    ./src/Framework/IO/TextReader.cpp
    ./src/Framework/IO/TextWriter.cpp
    ./src/Framework/IO/Console.cpp
    ./src/Framework/IO/ConsoleReader.cpp
    ./src/Framework/IO/ConsoleWriter.cpp
    ./src/Framework/Log/DefaultLogger.cpp
    ./src/Framework/Log/FileLogger.cpp
    ./src/Framework/Log/Logger.cpp
    ./src/Framework/Math/MathUtils.cpp
    ./src/Framework/Math/Random.cpp
    ./src/Framework/Math/Color.cpp
    ./src/Framework/Math/IncompatibleMatrixSizeException.cpp
    ./src/Framework/Math/NonInvertibleMatrixException.cpp
    ./src/Framework/Math/NonSquareMatrixException.cpp
    ./src/Framework/Memory/Memory.cpp
    ./src/Framework/Memory/Object.cpp
    ./src/Framework/System/Platform.cpp
    ./src/Framework/System/Module.cpp
    ./src/Framework/System/Paths.cpp
    ./src/Framework/System/DateTime.cpp
    ./src/Framework/System/TimeSpan.cpp
    ./src/Framework/System/Thread.cpp
    ./src/Framework/System/Timer.cpp
    ./src/Framework/System/Mutex.cpp
    ./src/Framework/System/ThreadManager.cpp
    ./src/Framework/Json/Json.cpp
    ./src/Framework/Json/Lexer.cpp
    ./src/Framework/Json/Parser.cpp
    ./src/Framework/Json/Writer.cpp
    ./src/Framework/Json/JsonParseException.cpp
    ./src/Framework/Profiler.cpp
    ./src/Framework/Exception.cpp
    ./src/Framework/RuntimeException.cpp
    ./src/Framework/IndexException.cpp
    ./src/Framework/String.cpp
    ./src/Framework/Name.cpp
    ./src/Framework/Scalar.cpp
)

if (PLATFORM STREQUAL "Windows")
    list(APPEND SOURCES "./include/Framework/System/WindowsApp.hpp")
    list(APPEND SOURCES "./src/Framework/System/WindowsApp.cpp")
else (PLATFORM STREQUAL "Windows")
    list(APPEND SOURCES "./include/Framework/System/UnixApp.hpp")
    list(APPEND SOURCES "./src/Framework/System/UnixApp.cpp")
endif (PLATFORM STREQUAL "Windows")

bp_product_properties(BPF
    VERSION_MAJOR ${BP_VERSION_MAJOR}
    VERSION_MINOR ${BP_VERSION_MINOR}
    DESCRIPTION "BlockProject 3D Framework"
)

bp_setup_module(BPF API_MACRO BPF_API PACKAGE)
target_compile_definitions(BPF PRIVATE "BP_VERSION_STRING=\"${BP_VERSION_STRING}\"")
if (NOT WIN32)
	target_link_libraries(BPF PRIVATE pthread)
	target_link_libraries(BPF PRIVATE ${CMAKE_DL_LIBS})
endif (NOT WIN32)
