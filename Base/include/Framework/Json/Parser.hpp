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
#include "Framework/Json/Lexer.hpp"

namespace bpf
{
    namespace json
    {
        /**
         * A simple Json Parser
         */
        class BPF_API Parser
        {
        private:
            collection::Queue<Lexer::Token> _tokens;
            fsize _line; //Keep track of last line
            bool _ignoreNulls;

            bool CheckObject(Json &j);
            bool CheckArray(Json &j);
            bool CheckNumber(Json &j);
            bool CheckString(Json &j);
            bool CheckBasic(Json &j);
            void CheckColon();
            void CheckComa();
            Json CheckJson();
        public:
            /**
             * Constructs a Parser
             * @param lexer the lexer instance to extract tokens from
             */
            explicit inline Parser(Lexer &&lexer)
                : _tokens(lexer.ReadTokens())
                , _line(1)
                , _ignoreNulls(lexer.IgnoreNulls())
            {
            }

            /**
             * Parses all tokens into a Json value and returns it
             * @throw JsonParseException when the parser could not interpret a token sequence
             * @return new Json value from all parsed tokens
             */
            Json Parse();
        };
    }
}
