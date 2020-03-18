// Copyright (c) 2020, BlockProject
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
#include "Framework/Collection/Queue.hpp"
#include "Framework/String.hpp"

namespace bpf
{
    namespace json
    {
        class BPF_API Lexer
        {
        public:
            enum class ETokenType
            {
                STRING,
                NUMBER,
                BASIC
            };

            struct BPF_API Token
            {
                String Data;
                int Line;
                ETokenType Type;
            };

        private:
            int _line;
            bool _enableComments;
            bool _ignoreNulls;
            bool _comment;
            bool _lineComment;
            bool _string;
            int _cursor;
            String _curNbr;
            String _curExponent;
            collection::Queue<Token> _tokens;

            bool CheckString(const String &token);
            bool CheckNumber(const String &token, const fchar next);
            bool CheckBasic(const String &token, const fchar next);
            String ReProcessString(const String &str);
            fchar ProcessUnicode(const String &str, int &pos);
            fchar ProcessStandard(const String &str, int &pos);

        public:
            Lexer(const bool enableComments = false, const bool ignoreNulls = false);
            void LoadString(const String &input);

            collection::Queue<Token> ReadTokens();
            inline bool IgnoreNulls() const noexcept
            {
                return (_ignoreNulls);
            }
        };
    }
}
