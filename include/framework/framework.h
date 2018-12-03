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

# include "framework/api.h"
# include "framework/exception.h"
# include "framework/memory.h"
# include "framework/treenode.h"
# include "framework/iterator.h"
//# include "framework/class.h"
# include "framework/list.h"
# include "framework/hash.h"
# include "framework/map.h"
# include "framework/platform.h"
# include "framework/object.h"
# include "framework/delegate.h"
# include "framework/string.h"
# include "framework/array.h"
# include "framework/stack.h"
# include "framework/factory.h"
# include "framework/logger.h"
# include "framework/runtimeexception.h"
# include "framework/path.h"
# include "framework/module.h"
# include "framework/modulemanager.h"
# include "framework/stream.h"
# include "framework/filesystem.h"
# include "framework/network.h"
# include "framework/system.h"
# include "framework/color.h"
# include "framework/file.h"
# include "framework/profiler.h"

namespace Framework
{
# include "framework/list_impl.h"
# include "framework/map_impl.h"
# include "framework/array_impl.h"
# include "framework/stack_impl.h"
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
