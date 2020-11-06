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
#include <Framework/Types.hpp>
#include <type_traits>

namespace bpf
{
    namespace _bpf_internal_cast
    {
        template <typename T>
        class HasReturn
        {
            struct Fallback
            {
                int Return;
            };
            struct Derived : T, Fallback
            {
            };

            template<typename U, U>
            struct Check;

            typedef uint8 OneByte;
            typedef uint16 TwoBytes;

            template<typename U>
            static OneByte Func(Check<int Fallback::*, &U::Return> *);

            template<typename U>
            static TwoBytes Func(...);

        public:
            static constexpr bool Value = sizeof(Func<Derived>(0)) == 2;
        };

        template <typename T>
        class HasCReturn
        {
            struct Fallback
            {
                int CReturn;
            };
            struct Derived : T, Fallback
            {
            };

            template<typename U, U>
            struct Check;

            typedef uint8 OneByte;
            typedef uint16 TwoBytes;

            template<typename U>
            static OneByte Func(Check<int Fallback::*, &U::CReturn> *);

            template<typename U>
            static TwoBytes Func(...);

        public:
            static constexpr bool Value = sizeof(Func<Derived>(0)) == 2;
        };
    }

    template <typename Target, typename Source>
    class CastOperator
    {
    };

    template <typename Target, typename Source,
              typename =
                  typename std::enable_if<!std::is_pointer<Source>::value
                                          && !_bpf_internal_cast::HasReturn<CastOperator<Target, Source>>::Value
                                          && std::is_constructible<Source, Target>::value>::type>
    inline Target Cast(Source &&source)
    {
        return (static_cast<Target>(source));
    }

    template <typename Target, typename Source,
              typename =
                  typename std::enable_if<!std::is_pointer<Source>::value
                                          && !_bpf_internal_cast::HasCReturn<CastOperator<Target, Source>>::Value
                                          && std::is_constructible<Source, Target>::value>::type>
    inline Target Cast(const Source &source)
    {
        return (static_cast<Target>(source));
    }

    template <typename Target, typename Source>
    inline typename CastOperator<Target, Source>::Return Cast(Source &source)
    {
        return (CastOperator<Target, Source>::Cast(source));
    }

    template <typename Target, typename Source>
    inline typename CastOperator<Target, Source>::CReturn Cast(const Source &source)
    {
        return (CastOperator<Target, Source>::Cast(source));
    }
}
