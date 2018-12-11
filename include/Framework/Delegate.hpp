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
