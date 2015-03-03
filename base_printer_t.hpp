#ifndef __base_t_hpp__
#define __base_t_hpp__

#include <string>
#include <sstream>
#include <iosfwd>
#include <stack>
#include <unordered_map>
#include "num_t.hpp"

struct base_printer_t
{
   std::string convert
   (
      const num_t& n,
      const std::unordered_map<int, char>& digits,
      const std::string& prefix
   )
   {
      if (n < 1)
      {
         std::string result = prefix + "0";
         return result;
      }

      const std::size_t num_digits = digits.size();
      const std::size_t mask = num_digits - 1;
      std::size_t shift_amount = 0;
      std::size_t mask_copy = mask;
      while (mask_copy)
      {
         ++shift_amount;
         mask_copy >>= 1;
      }
      try
      {

         int_t whole(floor(n).str());
         std::stack<char> s;
         while (whole > 0)
         {
            int digit = static_cast<int>(whole & mask);
            auto p = digits.find(digit);
            if (p == digits.end())
            {
               throw "missing digits";
            }
            s.push(p->second);
            whole >>= shift_amount;
         }
         std::stringstream ss;
         ss << prefix;
         while (!s.empty())
         {
            ss << s.top();
            s.pop();
         }
         std::string result = ss.str();

         return result;
      }
      catch (const std::exception& e)
      {
         // can't convert to int, print normally
      }

      std::string result = n.str();
      return result;
   }
};

#endif
