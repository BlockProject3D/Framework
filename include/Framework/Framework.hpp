#ifndef FRAMEWORK_H_
# define FRAMEWORK_H_

# define GETBIT(v, bit) ((v << bit) & (0x01))

# include <stdlib.h>
# include <type_traits>

# define UNUSED(v) (void)v

# define INVOKE(obj, fnc) ((obj)->*(fnc))

/**
 * Returns the default of a given type
 * @tparam T the type to find the default of
 */
template <typename T, typename std::enable_if<!std::is_pointer<T>::value>::type * = nullptr>
T DefaultOf();
template <class T, typename std::enable_if<std::is_pointer<T>::value>::type * = nullptr>
inline T DefaultOf()
{
    return (nullptr);
}

typedef unsigned char uint8;
typedef char int8;
typedef unsigned int uint32;
typedef short int16;
typedef unsigned short uint16;
typedef uint8 byte;
typedef long int int64;
typedef long unsigned int uint64;
typedef uint32 fchar;

/**
 * Defines a default for a given type
 */
# define DEFINE_DEFAULT(type, ret) \
template <> \
inline type DefaultOf<type>() \
{ \
    return (ret); \
} \

# define LIST(type, ...) std::initializer_list<type>({(__VA_ARGS__)})

# include "Framework/API.hpp"
# include "Framework/Exception.hpp"
# include "Framework/System/Memory.hpp"
# include "Framework/TreeNode.hpp"
# include "Framework/Iterator.hpp"
//# include "framework/class.h"
# include "Framework/List.hpp"
# include "Framework/Hash.hpp"
# include "Framework/Map.hpp"
# include "Framework/System/Platform.hpp"
# include "Framework/Object.hpp"
# include "Framework/Delegate.hpp"
# include "Framework/String.hpp"
# include "Framework/Array.hpp"
# include "Framework/Stack.hpp"
# include "Framework/Factory.hpp"
# include "Framework/Logger.hpp"
# include "Framework/RuntimeException.hpp"
# include "Framework/Path.hpp"
# include "Framework/System/Module.hpp"
# include "Framework/System/ModuleManager.hpp"
# include "Framework/IO/Stream.hpp"
# include "Framework/IO/FileSystem.hpp"
# include "Framework/Network/Network.hpp"
# include "Framework/System/System.hpp"
# include "Framework/Color.hpp"
# include "Framework/IO/File.hpp"
# include "Framework/Profiler.hpp"

namespace Framework
{
# include "Framework/List.impl.hpp"
# include "Framework/Map.impl.hpp"
# include "Framework/Array.impl.hpp"
# include "Framework/Stack.impl.hpp"
};

/**
 * Returns the type name as a string of a given type
 * @tparam T the type to search the name of
 */
template<typename T>
const Framework::FString &TypeOf();

/**
 * Defines the type name for a given type
 */
# define DEFINE_TYPE(compilename, runtimename) \
template<> \
const Framework::FString &TypeOf<compilename>() \
{ \
    static Framework::FString staticname = #runtimename; \
    return (staticname); \
} \

DEFINE_DEFAULT(int, 0);
DEFINE_DEFAULT(float, 0);
DEFINE_DEFAULT(double, 0);
DEFINE_DEFAULT(bool, false);

#endif /* !FRAMEWORK_H_ */
