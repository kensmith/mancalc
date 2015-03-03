#include ".test.hpp"
#include "infix_t.hpp"

TEST(decimal_literal)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("1234");
   EQ(1234, p.top());
}

TEST(float_literal)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("12.34");
   CL(12.34, p.top(), 1e-5);
}

TEST(negative_float_literal)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("-12.34");
   CL(-12.34, p.top(), 1e-5);
}

TEST(leading_whitespace_negative_float_literal)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse(" -12.34");
   CL(-12.34, p.top(), 1e-5);
}

TEST(trailing_whitespace_negative_float_literal)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("-12.34 ");
   CL(-12.34, p.top(), 1e-5);
}

TEST(leading_and_trailing_whitespace_negative_float_literal)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse(" -12.34 ");
   CL(-12.34, p.top(), 1e-5);
}

TEST(exponential_expression)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("2^8");
   EQ(256, p.top());
}

TEST(bigger_exponential_expression)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("2^512");
   EQ
   (
      num_t
      (
         "13407807929942597099"
         "57402499820584612747"
         "93658205923933777235"
         "61443721764030073546"
         "97680187429816690342"
         "76900318581864860508"
         "53753882811946569946"
         "433649006084096"
      ),
      p.top()
   );
}

TEST(airy_exponential_expression)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("    2 ^   512  ");
   EQ
   (
      num_t
      (
         "13407807929942597099"
         "57402499820584612747"
         "93658205923933777235"
         "61443721764030073546"
         "97680187429816690342"
         "76900318581864860508"
         "53753882811946569946"
         "433649006084096"
      ),
      p.top()
   );
}

TEST(multiplication)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("123.456 * -47.0001");
   CL(-5802.4443456, p.top(), 1e-12);
}

TEST(division)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("123.456 / -47.0001");
   CL(-2.626717815494, p.top(), 1e-12);
}

TEST(modulo)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("123.456 % -47.0001");
   CL(-17.5443, p.top(), 1e-12);
}

TEST(addition)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("1234 + 4576");
   EQ(5810, p.top());
}

TEST(subtraction)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("1234 - 4576");
   EQ(-3342, p.top());
}
TEST(compound_expression)
{
   postfix_t p;
   infix_t infix(p);
   infix.parse("2^8+15*32-14/3");
   CL(731.33333333333, p.top(), 1e-9);
   infix.parse(" 2 ^ 8 + 15 * 32 - 14 / 3");
   CL(731.33333333333, p.top(), 1e-9);
}

TEST(parse_minus)
{
   postfix_t p;
   infix_t infix(p);
   auto t = infix.parse("3");
   t = infix.parse("4");
   t = infix.parse("-");
   EQ(-1, p.top());
}

TEST(parse_subtract_negative)
{
   postfix_t p;
   infix_t infix(p);
   auto t = infix.parse("3 - -4");
   EQ(7, p.top());
}

TEST(parse_point)
{
   postfix_t p;
   infix_t infix(p);
   auto t = infix.parse(".");
   EQ(0, p.top());
}

TEST(parenthetical)
{
   postfix_t p;
   infix_t infix(p);
   std::string expr = "(1+2)";
   auto t = infix.parse(expr);
   auto i = std::get<0>(t);
   auto msg = std::get<1>(t);
   EQ(3, p.top());
}

TEST(interesting_parenthetical)
{
   postfix_t p;
   infix_t infix(p);
   std::string expr = "(1+2)*7+2^(4+1)";
   auto t = infix.parse(expr);
   auto i = std::get<0>(t);
   auto msg = std::get<1>(t);
   EQ(53, p.top());
}

TEST(factorial)
{
   postfix_t p;
   infix_t infix(p);
   std::string expr = "10!";
   auto t = infix.parse(expr);
   EQ(3628800, p.top());
}

TEST(more_factorial)
{
   postfix_t p;
   infix_t infix(p);
   std::string expr = "10! + 1";
   auto t = infix.parse(expr);
   EQ(3628801, p.top());
}

TEST(operation_with_constant)
{
   postfix_t p;
   infix_t infix(p);
   std::string expr = "5! + 2*pi";
   auto t = infix.parse(expr);
   CL(126.2831853, p.top(), 1e-5);
}

TEST(infix_lshift)
{
   postfix_t p;
   infix_t infix(p);
   std::string expr = "1 << 32";
   auto t = infix.parse(expr);
   EQ(4294967296, p.top());
}

TEST(infix_rshift)
{
   postfix_t p;
   infix_t infix(p);
   std::string expr = "4294967296 >> 32";
   auto t = infix.parse(expr);
   EQ(1, p.top());
}

TEST(infix_rshift_precedence)
{
   postfix_t p;
   infix_t infix(p);
   std::string expr = "4294967296 >> 32 - 2";
   auto t = infix.parse(expr);
   EQ(4, p.top());
}

TEST(nested_parens)
{
   postfix_t p;
   infix_t infix(p);
   std::string expr = "2^(2^(2^2)) + 1";
   infix.parse(expr);
   EQ(65537, p.top());
}

TEST(factorial_bug)
{
   postfix_t p;
   infix_t infix(p);
   std::string expr = "(5*5)! + 1";
   auto t = infix.parse(expr);
   auto i = std::get<0>(t);
   auto msg = std::get<1>(t);
   if (i != expr.cend())
   {
      ooo(eee)
         << "'"
         << msg
         << "'";
   }
   EQ(num_t("15511210043330985984000001"), p.top());
}
