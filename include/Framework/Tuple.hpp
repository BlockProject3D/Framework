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

#pragma once

namespace bpf
{
    namespace __internal_tp
    {
        template <size_t MaxN, size_t N, typename Search, typename Arg, typename ...Args>
        struct TypeIndexer
        {
            static_assert(N < MaxN - 1, "Type does not exist in Tuple");
            using type = typename TypeIndexer<MaxN, N + 1, Search, Args...>::type;
            static constexpr size_t ID = N;
        };

        template <size_t MaxN, size_t N, typename Search, typename ...Args>
        struct TypeIndexer<MaxN, N, Search, Search, Args...>
        {
            using type = TypeIndexer;
            static constexpr size_t ID = N;
        };

        template <typename T>
        struct Type
        {
            using type = T;
        };

        template <size_t N, typename ...Args>
        struct Chooser;

        template <size_t N, typename Arg, typename ...Args>
        struct Chooser<N, Arg, Args...> : Chooser<N - 1, Args...> {};

        template <typename Arg, typename ...Args>
        struct Chooser<0, Arg, Args...> : Type<Arg> {};

        template <typename ...Args>
        struct Chooser<0, Args...>
        {
            static_assert(sizeof...(Args) > 0, "Tuple index out of range");
        };

        template <size_t ...N>
        struct Sizes : Type<Sizes<N...>> {};

        template <size_t Start, size_t End, typename S>
        struct Range;

        template <size_t Start, size_t End, size_t ...N>
        struct Range<Start, End, Sizes<N...>> : Range<Start + 1, End, Sizes<N..., Start>> {};

        template <size_t Start, size_t ...N>
        struct Range<Start, Start, Sizes<N...>> : Sizes<N...> {};

        template <size_t N, typename Arg>
        class Elem
        {
        private:
            Arg arg;

        public:
            inline Arg &Get()
            {
                return (arg);
            }

            inline const Arg &Get() const
            {
                return (arg);
            }
        };

        template <typename N, typename ...Args>
        class Impl;

        template <size_t ...N, typename ...Args>
        class Impl<Sizes<N...>, Args...> : Elem<N, Args>...
        {
        public:
            template <size_t I>
            using ElemType = typename Chooser<I, Args...>::type;

            template <size_t I>
            inline ElemType<I> &Get()
            {
                return (Elem<I, ElemType<I>>::Get());
            }

            template <size_t I>
            inline const ElemType<I> &Get() const
            {
                return (Elem<I, ElemType<I>>::Get());
            }

            template <typename T>
            inline ElemType<TypeIndexer<sizeof...(Args), 0, T, Args...>::type::ID> &Get()
            {
                return (Get<TypeIndexer<sizeof...(Args), 0, T, Args...>::type::ID>());
            }

            template <typename T>
            inline const ElemType<TypeIndexer<sizeof...(Args), 0, T, Args...>::type::ID> &Get() const
            {
                return (Get<TypeIndexer<sizeof...(Args), 0, T, Args...>::type::ID>());
            }
        };
    }

    template <typename ...Args>
    class BP_TPL_API Tuple : public __internal_tp::Impl<
        typename __internal_tp::Range<0, sizeof...(Args), __internal_tp::Sizes<>>::type, Args...>
    {
    public:
        constexpr size_t Size() const
        {
            return (sizeof...(Args));
        }
    };
}