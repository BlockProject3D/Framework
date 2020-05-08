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

#include "Framework/EvalException.hpp"
#include "Framework/String.hpp"
#include "Framework/IndexException.hpp"

namespace bpf
{
	/**
	 * Utility to evaluate simple math expressions
	 * @tparam T the type to evaluate to
	 */
	template <typename T>
	class BP_TPL_API MathEval
	{
	private:
		static T EvalNbr(const char* expr, char** endptr);
	public:
		/**
		 * Evaluates a math expression string
		 * @param str the expression string
		 * @throw EvalException when there is a math error
		 * @return evaluated number
		 */
		T Evaluate(const String &str);

		/**
		 * Parses a number at the start of a string and return how many characters were read
		 * @param str the string containing the number
		 * @param endpos output position in str where the last character could be found
		 * @return parsed number
		 */
		inline T EvalNbr(const String& str, fsize& endpos)
		{
			const char *data = *str;
			char *ptr;
			T num = EvalNbr(data, &ptr);

			endpos = (fsize)(ptr - data);
			return (num);
		}

		/**
		 * Parses a number at a given position in a string and return how many characters were read
		 * @param str the string containing the number
		 * @param startpos the start position in the string
		 * @param endpos output position in str where the last character could be found
		 * @throw IndexException when startpos is out of the string bounds
		 * @return parsed number
		 */
		inline T EvalNbr(const String& str, const fsize startpos, fsize& endpos)
		{
			if (startpos >= str.Size())
				throw IndexException();
			const char *data = *str + startpos;
			char *ptr;
			T num = EvalNbr(data, &ptr);

			endpos = (fsize)(ptr - data) + startpos;
			return (num);
		}
	};
}

#include "Framework/MathEval.impl.hpp"