// Copyright (c) 2020, BlockProject 3D
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
//     * Neither the name of BlockProject 3D nor the names of its contributors
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

#pragma once
#include "Framework/Collection/HashMap.hpp"
#include "Framework/Memory/ObjectConstructor.hpp"
#include "Framework/String.hpp"

namespace bpf
{
    namespace memory
    {
        /**
         * Factory, creates object instances from object constructor names
         * @tparam T the type of base class
         * @tparam Args the arguments to the constructor
         */
        template <class /* ? extends */ T, typename ...Args>
        class BP_TPL_API ObjectFactory
        {
        private:
            collection::HashMap<String, ObjectConstructor<T, Args...> *> Registry;

        public:
            /**
             * Registers a new class with this factory
             * @tparam C the class type to register (must have a BP_DEFINE_TYPENAME linked)
             */
            template <class C>
            inline void AddClass()
            {
                ObjectConstructor<T, Args...> *raw = typename C::template GetConstructor<Args...>();
                Registry[TypeName<C>()] = raw;
            }

            /**
             * Instantiates a new class from this factory
             * @param name the name of the class
             * @param args the arguments to the constructor
             * @throw MemoryException in case allocation is impossible
             * @return new unique pointer, null if class cannot be found
             */
            inline UniquePtr<T> MakeUnique(const String &name, Args &&... args)
            {
                if (!Registry.HasKey(name))
                    return (Null);
                return (Registry[name]->MakeUnique(std::forward<Args>(args)...));
            }

            /**
             * Instantiates a new class from this factory
             * @param name the name of the class
             * @param args the arguments to the constructor
             * @throw MemoryException in case allocation is impossible
             * @return new shared pointer, null if class cannot be found
             */
            inline SharedPtr<T> MakeShared(const String &name, Args &&... args)
            {
                if (!Registry.HasKey(name))
                    return (Null);
                return (Registry[name]->MakeShared(std::forward<Args>(args)...));
            }
        };
    }
}
