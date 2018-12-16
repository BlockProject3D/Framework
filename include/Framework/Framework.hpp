// Copyright (c) 2018, BlockProject
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//     * Neither the name of BlockProject nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * THIS FILE IS THE DEVIL !!!
 */
#ifndef FRAMEWORK_H_
# define FRAMEWORK_H_

# define GETBIT(v, bit) ((v << bit) & (0x01))

# include <stdlib.h>

# define UNUSED(v) (void)v

# define INVOKE(obj, fnc) ((obj)->*(fnc))

/**
 * Returns the default of a given type
 * @tparam T the type to find the default of
 */
/*template <typename T, typename std::enable_if<!std::is_pointer<T>::value>::type * = nullptr>
T DefaultOf();
template <class T, typename std::enable_if<std::is_pointer<T>::value>::type * = nullptr>
inline T DefaultOf()
{
    return (nullptr);
}*/

/*typedef unsigned char uint8;
typedef char int8;
typedef unsigned int uint32;
typedef short int16;
typedef unsigned short uint16;
typedef uint8 byte;
typedef long int int64;
typedef long unsigned int uint64;
typedef uint32 fchar;*/

/**
 * Defines a default for a given type
 */
/*# define DEFINE_DEFAULT(type, ret) \
template <> \
inline type DefaultOf<type>() \
{ \
    return (ret); \
} \

# define LIST(type, ...) std::initializer_list<type>({(__VA_ARGS__)})*/

/*# include "Framework/API.hpp"
# include "Framework/Exception.hpp"
# include "Framework/Memory/Memory.hpp"
# include "Framework/TreeNode.hpp"
# include "Framework/Iterator.hpp"*/
//# include "framework/class.h"
/*# include "Framework/List.hpp"
# include "Framework/Hash.hpp"
# include "Framework/Map.hpp"
# include "Framework/ObjectPtr.hpp"
# include "Framework/Object.hpp"*/
//# include "Framework/Delegate.hpp"
/*# include "Framework/String.hpp"
# include "Framework/Array.hpp"
# include "Framework/ArrayList.hpp"
# include "Framework/Stack.hpp"
# include "Framework/Factory.hpp"
# include "Framework/RuntimeException.hpp"
# include "Framework/IndexException.hpp"
# include "Framework/Path.hpp"
# include "Framework/Color.hpp"
# include "Framework/Profiler.hpp"*/

/*#include "Framework/List.impl.hpp"
#include "Framework/Array.impl.hpp"
#include "Framework/ArrayList.impl.hpp"
#include "Framework/Stack.impl.hpp"
#include "Framework/Map.impl.hpp"*/

/**
 * Returns the type name as a string of a given type
 * @tparam T the type to search the name of
 */
//template<typename T>
//const bpf::String &TypeOf();

/**
 * Defines the type name for a given type
 */
/*# define DEFINE_TYPE(compilename, runtimename) \
template<> \
const bpf::String &TypeOf<compilename>() \
{ \
    static bpf::String staticname = #runtimename; \
    return (staticname); \
} \

DEFINE_DEFAULT(int, 0);
DEFINE_DEFAULT(float, 0);
DEFINE_DEFAULT(double, 0);
DEFINE_DEFAULT(bool, false);*/

#endif /* !FRAMEWORK_H_ */
