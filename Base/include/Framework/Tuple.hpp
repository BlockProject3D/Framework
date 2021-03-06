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
#include "Framework/Types.hpp"
#include <type_traits>

namespace bpf
{
    namespace _bpf_internal_tp
    {
        struct CorrectFalseType
        {
            static constexpr bool Value = false;
        };

        template <typename... Args>
        struct IsAllDefaultConstructible;

        template <typename Arg>
        struct IsAllDefaultConstructible<Arg>
        {
            static constexpr bool Value = std::is_default_constructible<Arg>::value;
        };

        template <typename Arg, typename... Args>
        struct IsAllDefaultConstructible<Arg, Args...>
        {
            static constexpr bool Value =
                std::conditional<std::is_default_constructible<Arg>::value, IsAllDefaultConstructible<Args...>,
                                 CorrectFalseType>::type::Value;
        };

        template <fsize MaxN, fsize N, typename Search, typename Arg, typename... Args>
        struct TypeIndexer
        {
            static_assert(N < MaxN - 1, "Type does not exist in Tuple");
            using type = typename TypeIndexer<MaxN, N + 1, Search, Args...>::type;
            static constexpr fsize ID = N;
        };

        template <fsize MaxN, fsize N, typename Search, typename... Args>
        struct TypeIndexer<MaxN, N, Search, Search, Args...>
        {
            using type = TypeIndexer;
            static constexpr fsize ID = N;
        };

        template <typename T>
        struct Type
        {
            using type = T;
        };

        template <fsize N, typename... Args>
        struct Chooser;

        template <fsize N, typename Arg, typename... Args>
        struct Chooser<N, Arg, Args...> : Chooser<N - 1, Args...>
        {
        };

        template <typename Arg, typename... Args>
        struct Chooser<0, Arg, Args...> : Type<Arg>
        {
        };

        template <typename... Args>
        struct Chooser<0, Args...>
        {
            static_assert(sizeof...(Args) > 0, "Tuple index out of range");
        };

        template <fsize... N>
        struct Sizes : Type<Sizes<N...>>
        {
        };

        template <fsize Start, fsize End, typename S>
        struct Range;

        template <fsize Start, fsize End, fsize... N>
        struct Range<Start, End, Sizes<N...>> : Range<Start + 1, End, Sizes<N..., Start>>
        {
        };

        template <fsize Start, fsize... N>
        struct Range<Start, Start, Sizes<N...>> : Sizes<N...>
        {
        };

        template <fsize N, typename Arg>
        class Elem
        {
        private:
            Arg arg;

        public:
            template <fsize N1 = N, typename Arg1 = Arg,
                      typename = typename std::enable_if<std::is_default_constructible<Arg1>::value>::type>
            inline Elem()
            {
            }

            explicit inline Elem(Arg &&a)
                : arg(std::forward<Arg>(a))
            {
            }

            inline Arg &Get()
            {
                return (arg);
            }

            inline const Arg &Get() const
            {
                return (arg);
            }
        };

        template <typename N, typename... Args>
        class Impl;

        template <fsize... N, typename... Args>
        class Impl<Sizes<N...>, Args...> : Elem<N, Args>...
        {
        public:
            template <fsize I>
            using ElemType = typename Chooser<I, Args...>::type;

            template <bool Flag = _bpf_internal_tp::IsAllDefaultConstructible<Args...>::Value,
                      typename = typename std::enable_if<Flag>::type>
            inline Impl()
                : Elem<N, Args>()...
            {
            }

            explicit inline Impl(Args &&... args)
                : Elem<N, Args>(std::forward<Args &&>(args))...
            {
            }

            /**
             * Gets an item by index
             * In case index does not exist, a compile error is generated
             * @tparam I the index of the item
             * @return mutable item
             */
            template <fsize I>
            inline ElemType<I> &Get()
            {
                return (Elem<I, ElemType<I>>::Get());
            }

            /**
             * Gets an item by index
             * In case index does not exist, a compile error is generated
             * @tparam I the index of the item
             * @return immutable item
             */
            template <fsize I>
            inline const ElemType<I> &Get() const
            {
                return (Elem<I, ElemType<I>>::Get());
            }

            /**
             * Gets an item by type
             * In case type does not exist in tuple, a compile error is generated
             * @tparam T the type of the item
             * @return mutable item
             */
            template <typename T>
            inline ElemType<TypeIndexer<sizeof...(Args), 0, T, Args...>::type::ID> &Get()
            {
                return (Get<TypeIndexer<sizeof...(Args), 0, T, Args...>::type::ID>());
            }

            /**
             * Gets an item by type
             * In case type does not exist in tuple, a compile error is generated
             * @tparam T the type of the item
             * @return mutable item
             */
            template <typename T>
            inline const ElemType<TypeIndexer<sizeof...(Args), 0, T, Args...>::type::ID> &Get() const
            {
                return (Get<TypeIndexer<sizeof...(Args), 0, T, Args...>::type::ID>());
            }
        };
    }

    /**
     * Cross platform tuple with fixed DLL interface warnings on Windows
     * Also includes non C++ 11 features like get item by type
     * @tparam Args the types to store in the tuple
     */
    template <typename... Args>
    class BP_TPL_API Tuple
        : public _bpf_internal_tp::Impl<
              typename _bpf_internal_tp::Range<0, sizeof...(Args), _bpf_internal_tp::Sizes<>>::type, Args...>
    {
    public:
        /**
         * Constructs a tuple using default constructor
         */
        template <bool Flag = _bpf_internal_tp::IsAllDefaultConstructible<Args...>::Value,
                  typename = typename std::enable_if<Flag>::type>
        inline Tuple()
            : _bpf_internal_tp::Impl<
                  typename _bpf_internal_tp::Range<0, sizeof...(Args), _bpf_internal_tp::Sizes<>>::type, Args...>()
        {
        }

        /**
         * Constructs a tuple
         */
        explicit inline Tuple(Args &&... args)
            : _bpf_internal_tp::Impl<
                  typename _bpf_internal_tp::Range<0, sizeof...(Args), _bpf_internal_tp::Sizes<>>::type, Args...>(
                  std::forward<Args &&>(args)...)
        {
        }

        /**
         * Number of items in the tuple
         */
        constexpr fsize Size() const
        {
            return (sizeof...(Args));
        }
    };
}
