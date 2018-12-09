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
