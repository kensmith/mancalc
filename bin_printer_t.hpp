#ifndef __bin_printer_t_hpp__
#define __bin_printer_t_hpp__

#include <string>
#include <unordered_map>
#include "num_t.hpp"
#include "base_printer_t.hpp"

struct bin_printer_t : base_printer_t
{
   std::string convert(const num_t& n)
   {
      std::string result = base_printer_t::convert
      (
         n,
         digits,
         "0b"
      );
      return result;
   }
private:
   std::unordered_map<int, char> digits =
   {
      {0, '0'}, {1, '1'}
   };
};

#endif
