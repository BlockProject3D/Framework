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
#include "Framework/Json/Json.hpp"
#include "Framework/String.hpp"
#include "Framework/Collection/Stringifier.List.hpp"
#include "Framework/Collection/Stringifier.Map.hpp"

namespace bpf
{
    /**
     * Provides string representation to Json Array type
     */
    template <>
    class String::Stringifier<json::Json::Array>
    {
    public:
        inline static String Stringify(const json::Json::Array &val, const fsize prec)
        {
            return (String::ValueOf(val.Items, prec));
        }
    };

    /**
     * Provides string representation to Json Object type
     */
    template <>
    class String::Stringifier<json::Json::Object>
    {
    public:
        inline static String Stringify(const json::Json::Object &val, const fsize prec)
        {
            return (String::ValueOf(val.Properties, prec));
        }
    };

    /**
     * Provides string representation to Json type
     */
    template <>
    class String::Stringifier<json::Json>
    {
    public:
        inline static String Stringify(const json::Json &val, const fsize prec)
        {
            switch (val.Type())
            {
            case json::Json::EType::STRING:
                return (val.ToString());
            case json::Json::EType::NUMBER:
                return (String::ValueOf(val.ToNumber(), prec));
            case json::Json::EType::BOOLEAN:
                return (String::ValueOf(val.ToBoolean(), prec));
            case json::Json::EType::NONE:
                return ("null");
            case json::Json::EType::ARRAY:
                return (String::ValueOf(val.ToArray(), prec));
            case json::Json::EType::OBJECT:
                return (String::ValueOf(val.ToObject(), prec));
            }
            return ("Unknown value type");
        }
    };
}