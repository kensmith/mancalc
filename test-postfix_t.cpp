#include ".test.hpp"
#include "postfix_t.hpp"

TEST(basic_add)
{
   postfix_t postfix;
   THROW(postfix.push("+"), const char*);
   postfix.push(10);
   THROW(postfix.push("+"), const char*);
   postfix.push(10);
   postfix.push("+");
   EQ(20, postfix.top());
}

TEST(bigger_add)
{
   postfix_t postfix;
   postfix.push(num_t("2251799813685248"));
   postfix.push(num_t(
      "115792089237316195423570985008687907853"
      "269984665640564039457584007913129639936"));
   postfix.push("+");
   EQ(num_t(
         "1157920892373161954235709850086879078532"
         "69984665640564039457586259712943325184"),
      postfix.top());
}

TEST(test_mf)
{
   postfix_t postfix;
   postfix.push(20);
   postfix.push("mf");
   CL(65.6168, postfix.top(), 0.00001);
}

TEST(basic_negate)
{
   postfix_t postfix;
   postfix.push(num_t(12345));
   postfix.push("neg");
   EQ(num_t(-12345), postfix.top());
}

TEST(basic_subtract)
{
   postfix_t p;
   p.push(12345);
   p.push(5);
   p.push("-");
   EQ(12340, p.top());
}

TEST(basic_multiply)
{
   postfix_t p;
   p.push(12345);
   p.push(5);
   p.push("*");
   EQ(61725, p.top());
}

TEST(basic_divide)
{
   postfix_t p;
   p.push(12345);
   p.push(5);
   p.push("/");
   EQ(2469, p.top());
}

TEST(basic_lshift)
{
   postfix_t p;
   p.push(12345);
   p.push(2);
   p.push("<<");
   EQ(12345*4, p.top());
}

TEST(basic_rshift)
{
   postfix_t p;
   p.push(12345);
   p.push(2);
   p.push(">>");
   EQ(3086.25, p.top());
}

TEST(basic_pow)
{
   postfix_t p;
   p.push(3);
   p.push(3);
   p.push("^");
   EQ(27, p.top());
}

TEST(big_pow)
{
   postfix_t p;
   p.push(2);
   p.push(512);
   p.push("^");
   EQ(num_t(
         "134078079299425970995740249982058461274"
         "7936582059239337772356144372176403007354"
         "6976801874298166903427690031858186486050"
         "853753882811946569946433649006084096"),
      p.top());
}

TEST(div_by_zero)
{
   postfix_t p;
   p.push(0);
   p.push(0);
   p.push("/");
   EQ("nan", p.top().str());

   p.push(0);
   p.push("/");
   EQ("nan", p.top().str());
}

TEST(pretty_sqrts)
{
   postfix_t p;
   p.push(65536);//16
   p.push("sqrt");
   EQ(256, p.top());//8
   p.push("sqrt");
   EQ(16, p.top());//4
   p.push("sqrt");
   EQ(4, p.top());//2
   p.push("sqrt");
   EQ(2, p.top());//1
   p.push("sqrt");
   CL(num_t(
         "1.41421356237309504880168872420969807856"
         "9671875376948073176679737990732478462107"
         "0388503875343276415727350138462309122970"
         "2492483605585073721264412149709993583141"
         "3222665927505592755799950501152782060571"
         "4701095599716059702745345968620147285174"
         "1864088919860955232923048430871432145083"
         "9762603627995251407989687253396546331808"
         "8296406206152583523950547457502877599617"
         "2983557522033753185701135437460340849884"
         "7160386899970699004815030544027790316454"
         "2478230684929369186215805784631115966687"
         "1301301561856898723723528850926486124949"
         "7715421833420428568606014682472077143585"
         "4874155657069677653720226485447015858801"
         "6207584749226572260020855844665214583988"
         "9394437092659180031138824646815708263010"
         "0594858704003186480342194897278290641045"
         "0726368813137398552561173220402450912277"
         "0022694112757362728049573810896750401836"
         "9868368450725799364729060762996941380475"
         "6548237289971803268024744206292691248590"
         "5218100445984215059112024944134172853147"
         "8105803603371077309182869314710171111683"
         "9165817268894197587165821521282295184884"
         "7208969463386289156288276595263525479309"
         "79"),
      p.top(),
      0.0000000000000000000000000000000000001);
}

TEST(test_hw)
{
   postfix_t p;
   p.push(1000);
   p.push("hw");
   CL(745700, p.top(), 0.1);
}

TEST(test_jf)
{
   postfix_t p;
   p.push(3000);
   p.push("jf");
   CL(2212.686, p.top(), 0.0001);
}

TEST(test_kp)
{
   postfix_t p;
   p.push(100);
   p.push("kp");
   CL(220.462, p.top(), 0.001);
}

TEST(test_lg)
{
   postfix_t p;
   p.push(1000);
   p.push("lg");
   CL(264.172, p.top(), 0.0001);
}

TEST(logarithms)
{
   postfix_t p;
   p.push(10);
   p.push("log");
   EQ(1, p.top());
   p.push("e");
   CL(num_t("2.71828182845904523536028"),
      num_t(p.top()),
      1e-20);
   p.push("ln");
   CL(1, p.top(), 1e-100);
}

TEST(test_abs_val)
{
   postfix_t p;
   p.push(10);
   EQ(10, p.top());
   p.push("abs");
   EQ(10, p.top());
   p.push(-10);
   EQ(-10, p.top());
   p.push("abs");
   EQ(10, p.top());
}

TEST(test_fc)
{
  postfix_t p;
  p.push(50);
  p.push("fc");
  EQ(10, p.top());
  p.push(68);
  p.push("fc");
  EQ(20, p.top());
  p.push(86);
  p.push("fc");
  EQ(30, p.top());
}

TEST(test_floor)
{
   postfix_t p;
   p.push(123.456);
   CL(123.456, p.top(), 0.00001);
   p.push("floor");
   EQ(123, p.top());
}

TEST(test_c)
{
  postfix_t p;
  p.push("c");
  EQ(299792458, p.top());
}

TEST(test_ceil)
{
   postfix_t p;
   p.push(123.456);
   CL(123.456, p.top(), 0.00001);
   p.push("ceil");
   EQ(124, p.top());
}

TEST(test_trunc)
{
   postfix_t p;
   p.push(123.456);
   CL(123.456, p.top(), 0.00001);
   p.push("trunc");
   EQ(123, p.top());
   p.push(-123.456);
   p.push("trunc");
   EQ(-123, p.top());
}

TEST(test_wh)
{
   postfix_t p;
   p.push(100000);
   p.push("wh");
   CL(134.10221, p.top(), 0.00001);
}

TEST(test_round)
{
   postfix_t p;
   p.push(1.1);
   p.push("round");
   EQ(1, p.top());
   p.push(1.5);
   p.push("round");
   EQ(2, p.top());
}

TEST(test_fj)
{
   postfix_t p;
   p.push(3000);
   p.push("fj");
   CL(4067.454, p.top(), 0.0001);
}

TEST(test_frac)
{
  postfix_t p;
  p.push(123.456);
  p.push("frac");
  CL(p.top(), 0.456, 0.0001);
}

TEST(test_frexp)
{
   postfix_t p;
   p.push(-4);
   p.push("frexp");
   num_t exp = p.pop();
   EQ(3, exp);
   num_t r = p.top();
   EQ(-0.5, r);
}

TEST(test_swap)
{
   postfix_t p;
   p.push(12);
   p.push(34);
   p.push("swap");
   EQ(12, p.pop());
   EQ(34, p.top());

   p.push("clr");
   p.push(12);
   p.push(34);
   p.push("sw");
   EQ(12, p.pop());
   EQ(34, p.top());
}

TEST(test_seq0)
{
  postfix_t p;
  p.push(10);
  p.push("seq0");
  for (int i = 9; i >= 0; --i)
  {
    EQ(i, p.pop());
  }
}

TEST(test_seq1)
{
  postfix_t p;
  p.push(10);
  p.push("seq1");
  for (int i = 10; i >= 1; --i)
  {
    EQ(i, p.pop());
  }
}

TEST(test_seq2)
{
  postfix_t p;
  p.push(15);
  p.push(20);
  p.push("seq2");
  for (int i = 20; i >= 15; --i)
  {
    EQ(i, p.pop());
  }
}

TEST(test_seq3)
{
  postfix_t p;
  p.push(15);
  p.push(0.5);
  p.push(20);
  p.push("seq3");
  for (num_t i = 20; i >= 15; i -= 0.5)
  {
    CL(i, p.pop(), 1e-10);
  }
}

TEST(test_mod_fun)
{
   postfix_t p;
   p.push(123.456);
   p.push(23.123);
   p.push("%");
   CL(num_t("7.840999999999997527311279554851353168487548828125"),
      p.top(),
      1e-30);
}

TEST(test_cos)
{
   postfix_t p;
   p.push("pi");
   p.push(2);
   p.push("/");
   p.push("cos");
   LT(p.top(), 1e-100);
}

TEST(test_sin)
{
   postfix_t p;
   p.push("pi");
   p.push(2);
   p.push("/");
   p.push("sin");
   CL(1, p.top(), 1e-100);
}

TEST(test_tan)
{
   postfix_t p;
   p.push("pi");
   p.push(4);
   p.push("/");
   p.push("tan");
   CL(1, p.top(), 1e-100);
}

TEST(test_acos)
{
   postfix_t p;
   p.push(1);
   p.push("acos");
   CL(0, p.top(), 1e-100);
}

TEST(test_asin)
{
   postfix_t p;
   p.push(1);
   p.push(2);
   p.push("sqrt");
   p.push("/");
   p.push("asin");
   CL(3.14159/4, p.top(), 1e-4);
}

TEST(test_atan)
{
   postfix_t p;
   p.push(1);
   p.push("atan");
   CL(3.14159/4, p.top(), 1e-4);
}

TEST(test_cosh)
{
   postfix_t p;
   p.push(1);
   p.push("cosh");
   CL(1.5430806348152, p.top(), 1e-9);
}

TEST(test_sinh)
{
   postfix_t p;
   p.push(1);
   p.push("sinh");
   CL(1.1752011936438, p.top(), 1e-9);
}

TEST(test_tanh)
{
   postfix_t p;
   p.push(1);
   p.push("tanh");
   CL(0.76159415595576, p.top(), 1e-9);
}

TEST(test_atan2)
{
   postfix_t p;
   p.push(1);
   p.push(2);
   p.push("atan2");
   CL(0.46364760900081, p.top(), 1e-9);
}

TEST(test_mil)
{
  postfix_t p;
  p.push(420); // distance
  p.push(3); // speed
  p.push("mil");
  CL(3.4920492975026125, p.top(), 1e-9);
}

TEST(test_mph)
{
  postfix_t p;
  p.push(425); // distance
  p.push(2.43); // mrad / s
  p.push("mph");
  CL(2.1124473397499175, p.top(), 1e-9);
}

TEST(test_gam)
{
  postfix_t p;
  p.push(1000);
  p.push("gam");
  CL(1.00000000000556325028031451679, p.top(), 1e-12);
}

TEST(stringize)
{
   postfix_t p;
   p.push("1");
   p.push("2");
   p.push("3");
   p.push("4");
   EQ("1   2   3   4", p.str());
}

TEST(engineering_notation)
{
   postfix_t p;
   p.push("e");
   p.push("pi");
   p.push("eng");
   EQ("2.71828   3.14159", p.str());
}

TEST(test_cf)
{
  postfix_t p;
  p.push(10);
  p.push("cf");
  EQ(50, p.top());
  p.push(20);
  p.push("cf");
  EQ(68, p.top());
  p.push(30);
  p.push("cf");
  EQ(86, p.top());
}

TEST(test_past)
{
  // seconds to pasteurize for t in fahrenheit
  // no guarantees for safety, use your own judgement
  postfix_t p;
  p.push(126);
  p.push("past");
  CL(281*60, p.top(), 1); // 281 minutes to pasteurize at 126

  p.push(130);
  p.push("past");
  CL(112*60, p.top(), 1);

  p.push(135);
  p.push("past");
  CL(35*60, p.top(), 1);

  p.push(140);
  p.push("past");
  CL(11*60, p.top(), 1);

  p.push(145);
  p.push("past");
  CL(3.49*60, p.top(), 1);

  p.push(160);
  p.push("past");
  CL(0.109*60, p.top(), 1);

  p.push(125);
  p.push("past");
  EQ("inf", p.top().str());
}

TEST(clear_stack)
{
   postfix_t p;
   p.push(1234);
   p.push(1234);
   p.push(1234);
   p.push(1234);
   EQ("1234   1234   1234   1234", p.str());
   p.push("clear");
   EQ("", p.str());
   p.push(1234);
   p.push(1234);
   p.push(1234);
   p.push(1234);
   p.push("com");
   EQ("1,234   1,234   1,234   1,234", p.str());
   p.push("clr");
   EQ("", p.str());
}

TEST(factorial)
{
   postfix_t p;
   p.push(10);
   p.push("!");
   EQ(3628800, p.top());
}

TEST(increment)
{
   postfix_t p;
   p.push(1);
   p.push("++");
   EQ(2, p.top());
}

TEST(decrement)
{
   postfix_t p;
   p.push(1);
   p.push("--");
   EQ(0, p.top());
}

TEST(case_insensitivity)
{
   postfix_t p;
   p.push("PI");
   p.push("COS");
   EQ(-1, p.top());
}

TEST(test_fm)
{
   postfix_t p;
   p.push(60);
   p.push("fm");
   CL(18.288, p.top(), 0.0001);
}

TEST(test_gl)
{
   postfix_t p;
   p.push(450);
   p.push("gl");
   CL(1703.44, p.top(), 0.001);
}

TEST(hexadecimal)
{
   postfix_t p;
   p.push("xabcd");
   EQ(43981, p.top());
   p.push("xabcd.8");
   EQ(43981.5, p.top());
   p.push("0xAbCd");
   EQ(43981, p.top());
   p.push("habce");
   EQ(43982, p.top());
   p.push("0haBcE");
   EQ(43982, p.top());
}

TEST(octal)
{
   postfix_t p;
   p.push("o777");
   EQ(511, p.top());
}

TEST(binary)
{
   postfix_t p;
   p.push("0b10100101");
   EQ(128+32+4+1, p.top());
}

TEST(sum_list)
{
   postfix_t p;
   for (int i = 1; i <= 10; i++)
   {
      p.push(i);
   }
   p.push("sum");
   EQ(55, p.top());
}

TEST(test_pk)
{
   postfix_t p;
   p.push(100);
   p.push("pk");
   CL(45.3592, p.top(), 0.0001);
}

TEST(prod_list)
{
   postfix_t p;
   for (int i = 1; i <= 10; i++)
   {
      p.push(i);
   }
   p.push("prod");
   EQ(3628800, p.top());
}

TEST(hex_display)
{
   postfix_t p;
   p.push(0x1234abcd);
   p.push("hex");
   EQ(0x1234abcd, p.top());
   EQ("0x1234abcd", p.str());
}

TEST(oct_display)
{
   postfix_t p;
   p.push(0777123);
   p.push("oct");
   EQ(0777123, p.top());
   EQ("0o777123", p.str());
}

TEST(bin_display)
{
   postfix_t p;
   p.push(0xface5);
   p.push("bin");
   EQ(0xface5, p.top());
   EQ("0b11111010110011100101", p.str());
}

TEST(hex_display_null)
{
   postfix_t p;
   p.push(0.125);
   p.push("hex");
   EQ(0.125, p.top());
   EQ("0x0", p.str());
}

TEST(avg_fun)
{
   postfix_t p;
   p.push(1);
   p.push(2);
   p.push(3);
   p.push(4);
   p.push("avg");
   EQ(2.5, p.top());
}

TEST(stddev_fun)
{
   postfix_t p;
   p.push(2);
   p.push(4);
   p.push(4);
   p.push(4);
   p.push(5);
   p.push(5);
   p.push(7);
   p.push(9);
   p.push("stddev");
   CL(4.57142857142857142857142857142857, p.top(), 1e-12);
}

TEST(removes_commas)
{
   postfix_t p;
   p.push("123,123,123");
   EQ(123123123, p.top());
}
