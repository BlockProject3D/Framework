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
#include "Framework/Collection/Queue.hpp"
#include "Framework/String.hpp"

namespace bpf
{
    namespace json
    {
        /**
         * A simple Json Lexer
         */
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
                fsize Line;
                ETokenType Type;
            };

        private:
            fsize _line;
            bool _enableComments;
            bool _ignoreNulls;
            bool _comment;
            bool _lineComment;
            bool _string;
            fisize _cursor;
            String _curNbr;
            String _curExponent;
            collection::Queue<Token> _tokens;

            bool CheckString(const String &token);
            bool CheckNumber(const String &token, fchar next);
            bool CheckBasic(const String &token, fchar next);
            static String ReProcessString(const String &str);
            static fchar ProcessUnicode(const String &str, fisize &pos);
            static fchar ProcessStandard(const String &str, fisize &pos);

        public:
            /**
             * Constructs a Lexer
             * @param enableComments true if this Lexer should accept comments, false otherwise
             * @param ignoreNulls true if null values should be omitted, false otherwise
             */
            Lexer(bool enableComments = false, bool ignoreNulls = false);

            /**
             * Loads a string a processes it
             * @param input the input string to load and process
             * @throw JsonParseException when the lexer could not identify a token in the input string
             */
            void LoadString(const String &input);

            /**
             * Extract all tokens from this Lexer, this function also purges the token cache and resets this Lexer's state
             * @return All extracted tokens
             */
            collection::Queue<Token> ReadTokens();

            /**
             * Check if this Lexer/Parser should ommit null values
             * @return true if null values should be omitted, false otherwise
             */
            inline bool IgnoreNulls() const noexcept
            {
                return (_ignoreNulls);
            }
        };
    }
}
