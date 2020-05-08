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

namespace bpf
{
	namespace __internal_evale
	{
		enum EEvalExprOperator
		{
			EVAL_EXPR_ADD = 0,
			EVAL_EXPR_SUB,
			EVAL_EXPR_MUL,
			EVAL_EXPR_DIV,
			EVAL_EXPR_MOD,
			EVAL_EXPR_NONE,
		};

		enum EEvalExprCode
		{
			EVAL_EXPR_PARENTHESIS = 0,
			EVAL_EXPR_DIVIDE_ZERO,
			EVAL_EXPR_MODULO_ZERO,
			EVAL_EXPR_SYNTHAX_INCORRECT,
			EVAL_EXPR_INVALID_NUMBER,
			EVAL_EXPR_SUCCESS,
		};

		template <typename T>
		static EEvalExprCode Operation(char const** expr, T& res, bool const parenthesis);

		static unsigned char IsOperator(char const expr)
		{
			return (expr == '+' || expr == '-' || expr == '*' || expr == '/');
		}

		static void DeleteSpace(char const** expr)
		{
			while (**expr && (**expr == ' ' || **expr == '\t'))
				++(*expr);
		}

		template <typename T>
		static EEvalExprCode Calc(T const left, T const right, T& res, EEvalExprOperator const op)
		{
			EEvalExprCode status = EVAL_EXPR_SUCCESS;

			switch (op)
			{
			case EVAL_EXPR_ADD:
				res = left + right;
				break;
			case EVAL_EXPR_SUB:
				res = left - right;
				break;
			case EVAL_EXPR_MUL:
				res = left * right;
				break;
			case EVAL_EXPR_DIV:
				if (right)
					res = left / right;
				else
					status = EVAL_EXPR_DIVIDE_ZERO;
				break;
			case EVAL_EXPR_MOD:
				if (right != 0.0)
					res = (T)(((uint64)left) % ((uint64)right));
				else
					status = EVAL_EXPR_MODULO_ZERO;
				break;
			default:
				status = EVAL_EXPR_SYNTHAX_INCORRECT;
				break;
			}
			return status;
		}

		template <typename T>
		T EvalNbr(const char* expr, char** endptr)
		{
			return (std::strtol(expr, endptr, 0));
		}

		template <>
		fint EvalNbr(char const* expr, char** endptr)
		{
			return (std::strtol(expr, endptr, 0));
		}

		template <>
		uint32 EvalNbr(char const* expr, char** endptr)
		{
			return (std::strtoul(expr, endptr, 0));
		}

		template <>
		int64 EvalNbr(char const* expr, char** endptr)
		{
			return (std::strtoll(expr, endptr, 0));
		}

		template <>
		uint64 EvalNbr(char const* expr, char** endptr)
		{
			return (std::strtoull(expr, endptr, 0));
		}

		template <>
		float EvalNbr(char const* expr, char** endptr)
		{
			return (std::strtof(expr, endptr));
		}

		template <>
		double EvalNbr(char const* expr, char** endptr)
		{
			return (std::strtod(expr, endptr));
		}

		template <typename T>
		static EEvalExprCode Number(char const** expr, T& num)
		{
			EEvalExprCode status = EVAL_EXPR_SUCCESS;

			DeleteSpace(expr);
			if (**expr)
			{
				num = EvalNbr<T>(*expr, (char**)expr);
				if (**expr == '(')
				{
					if (num == 0.0)
					{
						++(*expr);
						status = Operation(expr, num, true);
					}
				}
				else if (!(!**expr || IsOperator(**expr) || **expr == ' '
					|| **expr == '\t' || **expr == ')'))
					status = EVAL_EXPR_INVALID_NUMBER;
			}
			return status;
		}

		template <typename T>
		static EEvalExprCode Product(char const** expr, T& res)
		{
			T leftOperand = 0;
			T rightOperand = 0;
			EEvalExprOperator op = EVAL_EXPR_MUL;
			EEvalExprCode status = EVAL_EXPR_SUCCESS;

			if ((status = Number(expr, leftOperand)) == EVAL_EXPR_SUCCESS)
			{
				DeleteSpace(expr);
				switch (**expr)
				{
				case '*':
				case '/':
					if (**expr == '/')
						op = EVAL_EXPR_DIV;
				case '%':
					if (**expr == '%')
						op = EVAL_EXPR_MOD;
					++(*expr);
					DeleteSpace(expr);
					status = (IsOperator(**expr)) ? EVAL_EXPR_SYNTHAX_INCORRECT : Product(expr, rightOperand);
					break;
				case '(':
					status = EVAL_EXPR_SYNTHAX_INCORRECT;
					break;
				case '+':
				case '-':
				case ')':
				case '\0':
					rightOperand = (T)1.0;
					break;
				default:
					status = EVAL_EXPR_SYNTHAX_INCORRECT;
					break;
				}
				if (status == EVAL_EXPR_SUCCESS)
				{
					if (rightOperand != 1.0 || op == EVAL_EXPR_MOD)
						status = Calc(leftOperand, rightOperand, res, op);
					else
						res = leftOperand;
				}
			}
			return status;
		}

		template <typename T>
		static EEvalExprCode Sum(char const** expr, T& res)
		{
			T leftOperand = 0;
			T rightOperand = 0;
			EEvalExprOperator op = EVAL_EXPR_ADD;
			EEvalExprCode status = EVAL_EXPR_SUCCESS;

			if ((status = Product(expr, leftOperand)) == EVAL_EXPR_SUCCESS)
			{
				switch (**expr)
				{
				case '-':
					op = EVAL_EXPR_SUB;
				case '+':
					++(*expr);
					DeleteSpace(expr);
					status = (IsOperator(**expr)) ? EVAL_EXPR_SYNTHAX_INCORRECT : Sum(expr, rightOperand);
					if (status == EVAL_EXPR_SUCCESS)
						status = Calc(leftOperand, rightOperand, res, op);
					break;
				default:
					res = leftOperand;
					break;
				}
			}
			return status;
		}

		template <typename T>
		static EEvalExprCode Operation(char const** expr, T& res, bool const parenthesis)
		{
			T operand = 0;
			EEvalExprCode status = EVAL_EXPR_SUCCESS;

			DeleteSpace(expr);
			switch (**expr)
			{
			case '\0':
				break;
			default:
				if (((status = Sum(expr, res)) == EVAL_EXPR_SUCCESS) && **expr == '(')
				{
					++(*expr);
					status = Operation(expr, operand, true);
				}
				break;
			}
			if (parenthesis)
			{
				if (**expr == ')')
					++(*expr);
				else
					status = EVAL_EXPR_PARENTHESIS;
			}
			res += operand;
			return status;
		}
	}

	template <typename T>
	T MathEval<T>::Evaluate(const bpf::String &str)
	{
		const char *ptr = *str;
		T res = 0;
		__internal_evale::EEvalExprCode code = __internal_evale::Operation(&ptr, res, false);

		switch (code)
		{
		case __internal_evale::EVAL_EXPR_DIVIDE_ZERO:
			throw EvalException("Division by zero");
		case __internal_evale::EVAL_EXPR_INVALID_NUMBER:
			throw EvalException("Number expected");
		case __internal_evale::EVAL_EXPR_MODULO_ZERO:
			throw EvalException("Modulo by zero");
		case __internal_evale::EVAL_EXPR_SYNTHAX_INCORRECT:
			throw EvalException("Syntax error");
		case __internal_evale::EVAL_EXPR_PARENTHESIS:
			throw EvalException("Missing parenthesis");
		default:
			return (res);
		}
	}

	template <typename T>
	T MathEval<T>::EvalNbr(const char* expr, char** endptr)
	{
		return (__internal_evale::EvalNbr<T>(expr, endptr));
	}
}
