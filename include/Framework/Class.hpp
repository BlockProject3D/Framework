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

#ifndef CLASS_H
# define CLASS_H

# include "framework/factory.h"

namespace Framework
{
    /**
     * Provides a template to an abstract factory for type T
     * @tparam T the parent type
     * @tparam Args the constructor arguments
     */
    template <class /* ? extends */ T, typename ...Args>
    class FClass
    {
    private:
        IFactory<T, Args...> *Factory;
    public:
        inline FClass()
            : Factory(Null)
        {
        }

        /**
         * Initializes a FClass using a given concrete type factory
         * @param factory the factory to store
         */
        inline FClass(IFactory<T, Args...> *factory)
            : Factory(factory)
        {
        }

        /**
         * Returns a new instance of a T using the stored factory, Null if failed
         * @param args arguments to pass to the constructor
         */
        inline T *NewInstance(Args... args)
        {
            if (Factory == NULL)
                return (NULL);
            return (Factory->Create(args...));
        }
    };
};

#endif /* !CLASS_H */
