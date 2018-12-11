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

#ifndef FACTORY_H_
# define FACTORY_H_

namespace Framework
{
    /**
     * Constructor abstraction
     * @tparam T the type of parent to generate
     * @tparam Args the arguments to the constructor
     */
    template <class /* ? extends */ T, typename ...Args>
    class ENGINE_API IConstructor
    {
    public:
        virtual ~IConstructor() {}

        /**
         * Creates a new instance of the given type
         * @param args arguments to the constructor
         */
        virtual T *NewInstance(Args... args) = 0;
    };
    
    template <class /* ? extends */ T, typename ...Args>
    class ENGINE_API FFactory
    {
    private:
        bpf::Map<bpf::String, IConstructor<T, Args...> *> Registry;
    public:
        inline void AddClass(const bpf::String &name, IConstructor<T, Args...> *c)
        {
            Registry[name] = c;
        }
        inline T *New(const bpf::String &name, Args&&... args)
        {
            if (!Registry.HasKey(name))
                return (Null);
            return (Registry[name]->NewInstance(std::forward<Args>(args)...));
        }
    };
};

#endif /* !FACTORY_H_ */
