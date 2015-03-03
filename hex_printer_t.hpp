#ifndef __hex_printer_t_hpp__
#define __hex_printer_t_hpp__

#include <string>
#include <unordered_map>
#include "num_t.hpp"
#include "base_printer_t.hpp"

struct hex_printer_t : base_printer_t
{
   std::string convert(const num_t& n)
   {
      std::string result = base_printer_t::convert
      (
         n,
         digits,
         "0x"
      );
      return result;
   }
private:
   std::unordered_map<int, char> digits =
   {
      {0, '0'}, {1, '1'}, {2, '2'}, {3, '3'},
      {4, '4'}, {5, '5'}, {6, '6'}, {7, '7'},
      {8, '8'}, {9, '9'}, {10, 'a'}, {11, 'b'},
      {12, 'c'}, {13, 'd'}, {14, 'e'}, {15, 'f'},
   };
};

#endif
