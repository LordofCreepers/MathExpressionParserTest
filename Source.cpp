#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define _USE_MATH_DEFINES
#include "MathExpressionParser/MathExpressions.hpp"
#include "doctest/doctest.h"

#define ME_EVAL(expr, func, ...) \
{ std::vector<Parser::TokenPtr> tokens; Tree<Parser::TokenPtr> ast; \
long double res = MathExpressions::Evaluate(expr, MathExpressions::Environment{ ##__VA_ARGS__## }, tokens, ast); \
func(tokens, ast, res); }

#define MEP_CHECK(expr, res, ...) \
{ ME_EVAL(expr, [](const std::vector<Parser::TokenPtr>&, const Tree<Parser::TokenPtr>&, long double result) \
{ CHECK(res == result); }, __VA_ARGS__); }

#define ME_STRINGIFY_CHECK(expr, res_string) \
{ std::string e = expr; Parser::Engine parser; std::vector<Parser::TokenPtr> tokens; Tree<Parser::TokenPtr> ast; \
parser.Tokenize(MathExpressions::GetTokenFactories(), e, tokens); parser.Parse(tokens, ast); std::string out_str; \
parser.Stringify(tokens, out_str); CHECK(out_str == res_string); out_str.clear(); parser.Stringify(ast, out_str); \
CHECK(out_str == res_string); }

template<typename T>
T ctg(T x)
{
	return 1 / tan(x);
}

TEST_CASE("Testing evaluation of math expressions")
{
	MEP_CHECK("1", 1);
	MEP_CHECK("0.5555555", 0.5555555);
	MEP_CHECK(".1", .1);
	MEP_CHECK("pi", M_PI);
	MEP_CHECK("e", M_E);
	MEP_CHECK("x", 1, { "x", 1 });
	MEP_CHECK("1 + 1", 2);
	MEP_CHECK("1 - 1", 0);
	MEP_CHECK("-1", -1);
	MEP_CHECK("1 * 0.5", 0.5);
	MEP_CHECK("1 / 2", 0.5);
	MEP_CHECK("2 ^ 2", 4);
	MEP_CHECK("4 ^ 0.5", 2);
	MEP_CHECK("(1 + 1) * 2", 4);
	MEP_CHECK("|-1|", 1);
	MEP_CHECK("ln(e)", 1);
	MEP_CHECK("log2(8)", 3);
	MEP_CHECK("log10(100)", 2);
	MEP_CHECK("log(27, 3)", 3);
	MEP_CHECK("exp(1) - e", 0);
	MEP_CHECK("sqrt(81)", 9);
	MEP_CHECK("sign(-56)", -1);
	MEP_CHECK("sign(97)", 1);
	MEP_CHECK("sign(0)", 0);
	MEP_CHECK("sin(pi)", sin(M_PI));
	MEP_CHECK("cos(pi)", cos(M_PI));
	MEP_CHECK("tg(pi)", tan(M_PI));
	MEP_CHECK("ctg(pi)", ctg(M_PI));
	MEP_CHECK("asin(sin(1))", 1);
	MEP_CHECK("acos(cos(pi/2))", M_PI/2);
	MEP_CHECK("atan(tg(pi/2))", M_PI/2);
	MEP_CHECK("sinh(pi)", sinh(M_PI));
	MEP_CHECK("cosh(pi)", cosh(M_PI));
	MEP_CHECK("x * y", 4, { "x", 2 }, { "y", 2 });
	MEP_CHECK("7 + 5 * (3 ^ 2) - (5 / 0.5)", 42);
	MEP_CHECK("7 + 5 * 3 ^ 2 - 5 / 0.5", 42);
}

TEST_CASE("Testing conversion of tokens to source expression")
{
	ME_STRINGIFY_CHECK("1", "1");
	ME_STRINGIFY_CHECK("0.5555555", "0.5555555");
	ME_STRINGIFY_CHECK(".1", ".1");
	ME_STRINGIFY_CHECK("pi", "pi");
	ME_STRINGIFY_CHECK("e", "e");
	ME_STRINGIFY_CHECK("1 + 1", "1+1");
	ME_STRINGIFY_CHECK("-1", "-1");
	ME_STRINGIFY_CHECK("(1 + 1) * 2", "(1+1)*2");
	ME_STRINGIFY_CHECK("|-1|", "|-1|");
	ME_STRINGIFY_CHECK("ln(e)", "ln(e)");
	ME_STRINGIFY_CHECK("log(27, 3)", "log(27,3)");
	ME_STRINGIFY_CHECK("7 + 5 * (3 ^ 2) - (5 / 0.5)", "7+5*(3^2)-(5/0.5)");
}