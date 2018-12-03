#ifndef DELEGATE_H_
# define DELEGATE_H_

# include <functional>

namespace Framework
{
    template <typename /* Returns */ T, typename ...Args>
    class ENGINE_API FDelegateFunction
    {
    private:
        std::function<T(Args...)> FuncPtr;
    public:
        inline FDelegateFunction()
            : FuncPtr(Null)
        {
        }
        inline explicit FDelegateFunction(const std::function<T(Args...)> &fnc)
            : FuncPtr(fnc)
        {
        }

        /**
         * Invokes this function delegate, returns DefaultOf<T> in case of error
         * @param args the arguments to the function
         */
        inline T operator()(Args... args) const
        {
            if (!FuncPtr)
                return (DefaultOf<T>());
            return (FuncPtr(args...));
        }
    };

    template <typename ...Args>
    class ENGINE_API FDelegateFunction<void, Args...>
    {
    private:
        std::function<void(Args...)> FuncPtr;
    public:
        inline FDelegateFunction()
            : FuncPtr(Null)
        {
        }
        inline explicit FDelegateFunction(const std::function<void(Args...)> &fnc)
            : FuncPtr(fnc)
        {
        }

        /**
         * Invokes this function delegate
         * @param args the arguments to the function
         */
        inline void operator()(Args... args) const
        {
            if (!FuncPtr)
                return;
            FuncPtr(args...);
        }
    };

    template <typename /* Returns */ T, typename ...Args>
    class ENGINE_API FDelegate
    {
    private:
        bpf::List<FDelegateFunction<T, Args...>> List;
    public:
        inline int GetDelegatesNum()
        {
            return (List.Size());
        }
        inline bool HasDelegates()
        {
            return (GetDelegatesNum() > 0);
        }
        inline void operator+=(const FDelegateFunction<T, Args...> &fnc)
        {
            List.Add(fnc);
        }
        inline void operator+=(const std::function<T(Args...)> &fnc)
        {
            List.Add(FDelegateFunction<T, Args...>(fnc));
        }
        inline T Invoke(Args... args)
        {
            if (!HasDelegates())
                return (DefaultOf<T>());
            T res = DefaultOf<T>();
            for (auto it = List.Begin() ; it; ++it)
            {
                if ((res = (*it)(args...)) != DefaultOf<T>())
                    return (res);
            }
            return (res);
        }
    };

    template <typename ...Args>
    class ENGINE_API FDelegate<void, Args...>
    {
    private:
        bpf::List<FDelegateFunction<void, Args...>> List;
    public:
        inline int GetDelegatesNum()
        {
            return (List.Size());
        }
        inline bool HasDelegates()
        {
            return (GetDelegatesNum() > 0);
        }
        inline void operator+=(const FDelegateFunction<void, Args...> &fnc)
        {
            List.Add(fnc);
        }
        inline void operator+=(const std::function<void(Args...)> &fnc)
        {
            List.Add(FDelegateFunction<void, Args...>(fnc));
        }
        inline void Invoke(Args... args)
        {
            if (!HasDelegates())
                return;
            for (auto it = List.Begin() ; it; ++it)
                (*it)(args...);
        }
    };
};

#endif /* !DELEGATE_H_ */
