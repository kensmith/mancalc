#ifndef __postfix_t_hpp__
#define __postfix_t_hpp__

#include <deque>
#include <iosfwd>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/math/constants/constants.hpp>
#include "debug.hpp"
#include "num_t.hpp"
#include "stream_saver_t.hpp"
#include "display_types_t.hpp"
#include "make_property.hpp"
#include "hex_printer_t.hpp"
#include "bin_printer_t.hpp"
#include "oct_printer_t.hpp"
#include "commaifier_t.hpp"

/**
 * A postfix calculation engine.
 */
struct postfix_t
{
   postfix_t()
      :enabled_(true)
      ,display_type_(display_types_t::dec)
   {
   }

   /**
    * Remove everything from the results stack.
    */
   void clear()
   {
      numbers_.clear();
   }

   /**
    * Give the engine something to do.
    */
   void push(const std::string& op)
   {
      try
      {
         std::string op_copy = op;
         boost::algorithm::to_lower(op_copy);
         auto handler = handlers_.find(op_copy);
         if (handler != handlers_.end())
         {
            handler->second(*this);
            return;
         }

         op_copy = remove_commas(op_copy);
         if (check_matches_and_strip_hexadecimal(op_copy))
         {
            push_hexadecimal(op_copy);
         }
         else if (check_matches_and_strip_octal(op_copy))
         {
            push_octal(op_copy);
         }
         else if (check_matches_and_strip_binary(op_copy))
         {
            push_binary(op_copy);
         }
         else
         {
            push_decimal(op_copy);
         }
      }
      catch (std::exception& e)
      {
         ooo(eee)
            << "failed to push: "
            << e.what();
      }
   }

   // Overloads for type safety.
   void push(const num_t& number) { numbers_.push_back(number); }
   void push(double number) { numbers_.push_back(number); }
   void push(int number) { numbers_.push_back(number); }
   void push(unsigned int number) { numbers_.push_back(number); }
   void push(short number) { numbers_.push_back(number); }
   void push(unsigned short number) { numbers_.push_back(number); }

   void push(char op)
   {
      std::string converted(1, op);
      this->push(converted);
   }

   void push(unsigned char op)
   {
      std::string converted(1, op);
      this->push(converted);
   }

   /**
    * @return the top of the stack without removing it.
    */
   num_t top() const
   {
      ensure(numbers_.size() > 0);
      return numbers_.back();
   }

   /**
    * @return the top of the stack and remove it.
    */
   num_t pop()
   {
      num_t result = top();
      numbers_.pop_back();
      return result;
   }

   /**
    * @return true when we can apply a binary operation.
    */
   bool can_bop() const
   {
      return numbers_.size() >= 2;
   }

   /**
    * @return true when we can apply a unary operation.
    */
   bool can_uop() const
   {
      return numbers_.size() >= 1;
   }

   /**
    * @return the stack rendered into a readable form.
    */
   std::string str() const
   {
      std::stringstream ss;
      ss << *this;
      std::string result(ss.str());
      return result;
   }

private:
   friend std::ostream& operator<<(std::ostream& o, const postfix_t& p);

   make_property(bool, enabled);
   make_property(display_types_t, display_type);

   // This is not an std::stack because stacks can't iterate
   // and I want to be able to iterate in order to print.
   std::deque<num_t> numbers_;

   /**
    * Let num_t handle the parsing.
    */
   void push_decimal(const std::string& d)
   {
      try
      {
         num_t bignum(d);
         numbers_.push_back(bignum);
      }
      catch (std::runtime_error&)
      {
         throw "failed push";
      }
   }

   std::string remove_commas(const std::string& s)
   {
      std::string result = s;
      result.erase
      (
         std::remove(result.begin(), result.end(), ','), result.end()
      );
      return result;
   }

   /**
    * Attempt to interpret a nondecimal number.
    * @param[in] the nondecimal number with prefix removed.
    * @param[in] digits the mapping of digit to value for
    * all values in the base. Alphabetical characters should
    * be lowercase.
    */
   void push_nondecimal
   (
      const std::string& n,
      const std::unordered_map<char, num_t>& digits
   )
   {
      std::vector<std::string> components;
      boost::split(components, n, boost::is_any_of("."), boost::token_compress_on);
      num_t result = 0;
      num_t multiplier = 1;
      num_t base_multiplier = digits.size();
      num_t divider = base_multiplier;
      switch (components.size())
      {
         case 2:
            {
               std::string::const_iterator i =
                  components[1].cbegin();
               std::string::const_iterator end =
                  components[1].cend();
               for (;i < end; ++i)
               {
                  auto p = digits.find(*i);
                  if (p == digits.end())
                  {
                     throw "push_nondecimal invalid digit";
                  }
                  result += p->second / divider;
                  divider *= base_multiplier;
               }
            }
            // intentional fallthrough
         case 1:
            {
               std::string::const_iterator i =
                  components[0].cend() - 1;
               std::string::const_iterator begin =
                  components[0].cbegin();
               for (; begin <= i; --i)
               {
                  auto p = digits.find(*i);
                  if (p == digits.end())
                  {
                     throw "push_nondecimal invalid digit";
                  }
                  result += multiplier * p->second;
                  multiplier *= base_multiplier;
               }
            }
            break;
         case 0:
            throw "push_nondecimal no components";
         default:
            throw "push_nondecimal too many components";
      }
      numbers_.push_back(result);
   }

   std::unordered_map<char, num_t> hex_digits =
   {
      {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3},
      {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
      {'8', 8}, {'9', 9}, {'a', 10}, {'b', 11},
      {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15},
   };
   /**
    * @param[in] a hexidecimal number string
    */
   void push_hexadecimal(const std::string& x)
   {
      push_nondecimal(x, hex_digits);
   }

   std::unordered_map<char, num_t> oct_digits =
   {
      {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3},
      {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
   };
   /**
    * @param[in] an octal number string
    */
   void push_octal(const std::string& o)
   {
      push_nondecimal(o, oct_digits);
   }
   
   std::unordered_map<char, num_t> bin_digits =
   {
      {'0', 0}, {'1', 1},
   };
   /**
    * @param[in] a binary number string
    */
   void push_binary(const std::string& b)
   {
      push_nondecimal(b, bin_digits);
   }

   /**
    * Check a string to see if it begins with any of the
    * candidate prefixes and remove it. The first matched
    * prefix ends the operation.
    * @param[in] candidate_prefixes the set of prefixes
    * @param[in] s the string to check
    * @return true if one of the prefixes matched and was
    * removed.
    */
   bool check_matches_and_strip_prefix
   (
      const std::vector<std::string>& candidate_prefixes,
      std::string& s
   )
   {
      bool result = false;
      for (auto prefix : candidate_prefixes)
      {
         result = result || boost::starts_with(s, prefix);
         if (result)
         {
            s.erase(0, prefix.size());
            break;
         }
      }
      return result;
   }

   std::vector<std::string> hex_prefixes = 
   {
      "x", "h", "0x", "0h"
   };
   /**
    * Checks to see if a number is marked with a hexadecimal
    * prefix and removes it.
    * @param[in] x the potential hexadecimal number
    * @return true if x was hexadecimal
    */
   bool check_matches_and_strip_hexadecimal(std::string& x)
   {
      bool result = check_matches_and_strip_prefix(hex_prefixes, x);
      return result;
   }

   std::vector<std::string> octal_prefixes =
   {
      "o", "0o"
   };
   /**
    * Checks to see if a number is marked with a octal
    * prefix and removes it.
    * @param[in] x the potential octal number
    * @return true if x was octal
    */
   bool check_matches_and_strip_octal(std::string& o)
   {
      bool result = check_matches_and_strip_prefix(octal_prefixes, o);
      return result;
   }

   std::vector<std::string> binary_prefixes =
   {
      "b", "0b"
   };
   /**
    * Checks to see if a number is marked with a binary
    * prefix and removes it.
    * @param[in] x the potential binary number
    * @return true if x was binary
    */
   bool check_matches_and_strip_binary(std::string& b)
   {
      bool result = check_matches_and_strip_prefix(binary_prefixes, b);
      return result;
   }
   /**
    * The operators.
    */
   std::unordered_map
   <
      std::string,
      std::function<void(postfix_t&)>
   > handlers_ =
   {
      {
         "+",
         [](postfix_t& p)
         {
            ensure(p.can_bop());
            num_t rhs = p.pop();
            num_t lhs = p.pop();
            num_t result = lhs + rhs;
            p.push(result);
         }
      },
      {
         "-",
         [](postfix_t& p)
         {
            ensure(p.can_bop());
            num_t rhs = p.pop();
            num_t lhs = p.pop();
            num_t result = lhs - rhs;
            p.push(result);
         }
      },
      {
         "*",
         [](postfix_t& p)
         {
            ensure(p.can_bop());
            num_t rhs = p.pop();
            num_t lhs = p.pop();
            num_t result = lhs * rhs;
            p.push(result);
         }
      },
      {
         "/",
         [](postfix_t& p)
         {
            ensure(p.can_bop());
            num_t rhs = p.pop();
            num_t lhs = p.pop();
            num_t result = lhs / rhs;
            p.push(result);
         }
      },
      {
         "%",
         [](postfix_t& p)
         {
            ensure(p.can_bop());
            num_t rhs = p.pop();
            num_t lhs = p.pop();
            num_t result = fmod(lhs, rhs);
            p.push(result);
         }
      },
      {
         "<<",
         [](postfix_t& p)
         {
            ensure(p.can_bop());
            num_t rhs = p.pop();
            num_t orig_rhs = rhs;
            num_t lhs = p.pop();
            num_t orig_lhs = lhs;
            num_t result = lhs;
            while (p.enabled() && rhs > 0)
            {
               result *= 2;
               --rhs;
            }
            if (p.enabled())
            {
               p.push(result);
            }
            else
            {
               p.push(orig_lhs);
               p.push(orig_rhs);
            }
         }
      },
      {
         ">>",
         [](postfix_t& p)
         {
            ensure(p.can_bop());
            num_t rhs = p.pop();
            num_t orig_rhs = rhs;
            num_t lhs = p.pop();
            num_t orig_lhs = lhs;
            num_t result = lhs;
            while (p.enabled() && rhs > 0)
            {
               result /= 2;
               --rhs;
            }
            if (p.enabled())
            {
               p.push(result);
            }
            else
            {
               p.push(orig_lhs);
               p.push(orig_rhs);
            }
         }
      },
      {
         "^",
         [](postfix_t& p)
         {
            ensure(p.can_bop());
            num_t rhs = p.pop();
            num_t lhs = p.pop();
            num_t result = pow(lhs, rhs);
            p.push(result);
         }
      },
      {
         "!",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t orig_rhs = rhs;
            num_t result = 1;
            if (rhs == 0)
            {
               p.push(result);
               return;
            }
            while
            (
               p.enabled()
               &&
               rhs > 1
               && boost::math::isfinite(result)
            )
            {
               result *= rhs;
               --rhs;
            }
            if (p.enabled())
            {
               p.push(result);
            }
            else
            {
               p.push(orig_rhs);
            }
         }
      },
      {
         "++",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = rhs + 1;
            p.push(result);
         }
      },
      {
         "--",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = rhs - 1;
            p.push(result);
         }
      },
      {
         "abs",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = fabs(rhs);
            p.push(result);
         }
      },
      {
         "acos",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = acos(rhs);
            p.push(result);
         }
      },
      {
         "asin",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = asin(rhs);
            p.push(result);
         }
      },
      {
         "atan",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = atan(rhs);
            p.push(result);
         }
      },
      {
         "atan2",
         [](postfix_t& p)
         {
            ensure(p.can_bop());
            num_t rhs = p.pop();
            num_t lhs = p.pop();
            num_t result = atan2(lhs, rhs);
            p.push(result);
         }
      },
      {
         "avg",
         [](postfix_t& p)
         {
            num_t result = 0;
            num_t count = 0;
            while (p.can_uop())
            {
               ++count;
               result += p.pop();
            }
            result /= count;
            p.push(result);
         }
      },
      {
         "bin",
         [](postfix_t& p)
         {
            p.display_type(display_types_t::bin);
         }
      },
      {
         "ceil",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = ceil(rhs);
            p.push(result);
         }
      },
      {
         "clear",
         [](postfix_t& p)
         {
            p.clear();
         }
      },
      {
         "clr",
         [](postfix_t& p)
         {
            p.clear();
         }
      },
      {
         "cos",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = cos(rhs);
            p.push(result);
         }
      },
      {
         "cosh",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = cosh(rhs);
            p.push(result);
         }
      },
      {
         "dec",
         [](postfix_t& p)
         {
            p.display_type(display_types_t::dec);
         }
      },
      {
         "com",
         [](postfix_t& p)
         {
            p.display_type(display_types_t::com);
         }
      },
      {
         "e",
         [](postfix_t& p)
         {
            p.push(exp(num_t(1)));
         }
      },
      {
         "exp",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = exp(rhs);
            p.push(result);
         }
      },
      {
         "eng",
         [](postfix_t& p)
         {
            p.display_type(display_types_t::eng);
         }
      },
      {
         "floor",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = floor(rhs);
            p.push(result);
         }
      },
      {
         "frac",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = rhs - trunc(rhs);
            p.push(result);
         }
      },
      {
         "frexp",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            int exp = 0;
            num_t rhs = p.pop();
            num_t result = frexp(rhs, &exp);
            p.push(result);
            p.push(exp);
         }
      },
      {
         "hex",
         [](postfix_t& p)
         {
            p.display_type(display_types_t::hex);
         }
      },
      {
         "log",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = log10(rhs);
            p.push(result);
         }
      },
      {
         "ln",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = log(rhs);
            p.push(result);
         }
      },
      {
         "neg",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = -rhs;
            p.push(result);
         }
      },
      {
         "oct",
         [](postfix_t& p)
         {
            p.display_type(display_types_t::oct);
         }
      },
      {
         "pi",
         [](postfix_t& p)
         {
            p.push(boost::math::constants::pi<num_t>());
         }
      },
      {
         "pop",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            p.pop();
         }
      },
      {
         "prod",
         [](postfix_t& p)
         {
            num_t result = 1;
            while (p.can_uop())
            {
               result *= p.pop();
            }
            p.push(result);
         }
      },
      {
         "round",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = round(rhs);
            p.push(result);
         }
      },
      {
         "sin",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = sin(rhs);
            p.push(result);
         }
      },
      {
         "sinh",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = sinh(rhs);
            p.push(result);
         }
      },
      {
         "sqrt",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = sqrt(rhs);
            p.push(result);
         }
      },
      {
         "stddev",
         [](postfix_t& p)
         {
            num_t sum = 0;
            num_t count = 0;
            std::vector<num_t> copies;
            while (p.can_uop())
            {
               num_t x = p.pop();
               sum += x;
               ++count;
               copies.push_back(x);
            }
            num_t avg = sum / count;
            num_t sum_of_square_diffs = 0;
            for (auto n : copies)
            {
               num_t diff = n - avg;
               diff *= diff;
               sum_of_square_diffs += diff;
            }
            num_t result = sum_of_square_diffs / (count - 1);
            p.push(result);
         }
      },
      {
         "sum",
         [](postfix_t& p)
         {
            num_t result = 0;
            while (p.can_uop())
            {
               result += p.pop();
            }
            p.push(result);
         }
      },
      {
         "swap",
         [](postfix_t& p)
         {
            ensure(p.can_bop());
            num_t rhs = p.pop();
            num_t lhs = p.pop();
            p.push(rhs);
            p.push(lhs);
         }
      },
      {
         "tan",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = tan(rhs);
            p.push(result);
         }
      },
      {
         "tanh",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = tanh(rhs);
            p.push(result);
         }
      },
      {
         "trunc",
         [](postfix_t& p)
         {
            ensure(p.can_uop());
            num_t rhs = p.pop();
            num_t result = trunc(rhs);
            p.push(result);
         }
      },
   };
};

/**
 * Ostream support for postfix_t.
 */
inline std::ostream& operator<<(std::ostream& o, const postfix_t& p)
{
   static hex_printer_t hex_printer;
   static oct_printer_t oct_printer;
   static bin_printer_t bin_printer;
   stream_saver_t saver(o);

   const std::size_t count = p.numbers_.size();
   std::size_t i = 0;
   for (num_t num : p.numbers_)
   {
      switch (p.display_type())
      {
         case display_types_t::hex:
            o << hex_printer.convert(num);
            break;
         case display_types_t::oct:
            o << oct_printer.convert(num);
            break;
         case display_types_t::bin:
            o << bin_printer.convert(num);
            break;
         case display_types_t::eng:
            o << num;
            break;
         case display_types_t::com:
            o << commaifier_t(num.str()).str();
            break;
         case display_types_t::dec:
         default:
            o << num.str();
            break;
      }
      if (i < count - 1) o << "   ";
      ++i;
   }

   return o;
}

#endif
