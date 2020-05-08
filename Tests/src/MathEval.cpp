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

#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include <Framework/MathEval.hpp>

TEST(MathEval, EvaluateDouble)
{
	bpf::MathEval<double> calc;
	double res;
	res = calc.Evaluate("4+4");
	EXPECT_EQ(res, 8);
	res = calc.Evaluate("4+4*8");
	EXPECT_EQ(res, 36);
	res = calc.Evaluate("4*1+4*1+2");
	EXPECT_EQ(res, 10);
	res = calc.Evaluate("32/32");
	EXPECT_EQ(res, 1);
	res = calc.Evaluate("32/1");
	EXPECT_EQ(res, 32);
	res = calc.Evaluate("(((32/32)*32)/1)");
	EXPECT_EQ(res, 32);
	res = calc.Evaluate("-(1 + 1)");
	EXPECT_EQ(res, -2);
	res = calc.Evaluate("-1 + 1");
	EXPECT_EQ(res, 0);
	res = calc.Evaluate("-(1 + 1) * (4 - 1)");
	EXPECT_EQ(res, -6);
	res = calc.Evaluate("8 % 2");
	EXPECT_EQ(res, 0);
	res = calc.Evaluate("1 / 3");
	EXPECT_LE(res - 0.333333333333, 10e-11);
}

TEST(MathEval, EvaluateInt)
{
	bpf::MathEval<int> calc;
	int res;
	res = calc.Evaluate("4+4");
	EXPECT_EQ(res, 8);
	res = calc.Evaluate("4+4*8");
	EXPECT_EQ(res, 36);
	res = calc.Evaluate("4*1+4*1+2");
	EXPECT_EQ(res, 10);
	res = calc.Evaluate("32/32");
	EXPECT_EQ(res, 1);
	res = calc.Evaluate("32/1");
	EXPECT_EQ(res, 32);
	res = calc.Evaluate("(((32/32)*32)/1)");
	EXPECT_EQ(res, 32);
	res = calc.Evaluate("-(1 + 1)");
	EXPECT_EQ(res, -2);
	res = calc.Evaluate("-1 + 1");
	EXPECT_EQ(res, 0);
	res = calc.Evaluate("-(1 + 1) * (4 - 1)");
	EXPECT_EQ(res, -6);
	res = calc.Evaluate("8 % 2");
	EXPECT_EQ(res, 0);
}

TEST(MathEval, EvalNbr)
{
	bpf::MathEval<int> calc;
	bpf::fsize ptr;
	int res = calc.EvalNbr("1.", ptr);
	EXPECT_EQ(res, 1);
	EXPECT_EQ(ptr, 1U);
	res = calc.EvalNbr("42abc42", ptr);
	EXPECT_EQ(res, 42);
	EXPECT_EQ(ptr, 2U);
	res = calc.EvalNbr("-1", ptr);
	EXPECT_EQ(res, -1);
	EXPECT_EQ(ptr, 2U);
}

TEST(MathEval, EvaluateErr)
{
	bpf::MathEval<double> calc;
	EXPECT_THROW(calc.Evaluate("8 ff 2"), bpf::EvalException);
	EXPECT_THROW(calc.Evaluate(" 0 / 0 "), bpf::EvalException);
}
