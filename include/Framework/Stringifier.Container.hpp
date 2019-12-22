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
#include "Framework/String.hpp"
#include "Framework/Array.hpp"
#include "Framework/List.hpp"
#include "Framework/ArrayList.hpp"
#include "Framework/HashMap.hpp"

namespace bpf
{
    template <typename T, fsize I>
    class String::Stringifier<Array<T, I>>
    {
    public:
        inline static String Stringify(const Array<T, I> &arr)
        {
            String res = "[";

            for (fsize i = 0; i < arr.Size(); ++i)
            {
                res += String::ValueOf(arr[i]);
                if (i < arr.Size() - 1)
                    res += ", ";
            }
            res += "]";
            return (res);
        }
    };
    
    template <typename T>
    class String::Stringifier<List<T>>
    {
    public:
        inline static String Stringify(const List<T> &lst)
        {
            String res = "[";

            for (fsize i = 0; i < lst.Size(); ++i)
            {
                res += String::ValueOf(lst[i]);
                if (i < lst.Size() - 1)
                    res += ", ";
            }
            res += "]";
            return (res);
        }
    };

	template <typename T>
	class String::Stringifier<ArrayList<T>>
	{
	public:
		inline static String Stringify(const ArrayList<T> &arr)
		{
			String res = "[";

			for (fsize i = 0; i < arr.Size(); ++i)
			{
				res += String::ValueOf(arr[i]);
				if (i < arr.Size() - 1)
					res += ", ";
			}
			res += "]";
			return (res);
		}
	};

	template <typename K, typename V>
	class String::Stringifier<HashMap<K, V>>
	{
	public:
		inline static String Stringify(const HashMap<K, V> &map)
		{
			String res = "{";
			fsize i = 0;

			for (auto &entry : map)
			{
				res += String('\'') + String::ValueOf(entry.Key) + "': " + String::ValueOf(entry.Value);
				if (i < map.Size() - 1)
					res += ", ";
				++i;
			}
			res += "}";
			return (res);
		}
	};
}
