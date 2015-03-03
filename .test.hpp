/**
 * @file
 * @copyright Ken Smith kgsmith at gmail.com, 2013.
 * @license This software is released under the Boost
 * Software License, version 1.0.
 * See LICENSE_1_0.txt or
 * http://www.boost.org/LICENSE_1_0.txt
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "stream_saver_t.hpp"
#include "log_t.hpp"

#define TEST BOOST_AUTO_TEST_CASE
#define FTST BOOST_FIXTURE_TEST_CASE
#define EQ BOOST_REQUIRE_EQUAL
#define NE BOOST_REQUIRE_NE
#define CL BOOST_REQUIRE_CLOSE
#define GT BOOST_REQUIRE_GT
#define GE BOOST_REQUIRE_GE
#define LT BOOST_REQUIRE_LT
#define LE BOOST_REQUIRE_LE
#define THROW BOOST_REQUIRE_THROW
#define NO_THROW BOOST_REQUIRE_NO_THROW

inline std::string vecstr(const std::vector<char>& rhs)
{
   std::string s(rhs.begin(), rhs.end());
   return s;
}

inline std::vector<char> vecstr(const std::string& rhs)
{
   std::vector<char> v(rhs.begin(), rhs.end());
   return v;
}

inline std::ostream& operator<<(std::ostream& os, const std::vector<char>& rhs)
{
   stream_saver_t saver(os);

   os
      << "["
      << rhs.size()
      << "]"
      << std::hex << "{";
   for (char byte : rhs)
   {
      os << "0x" << uintptr_t(uint8_t(byte)) << ",";
   }
   os << "}";

   return os;
}

#define testname()\
do\
{\
   ooo(iii)\
      << typeid(*this).name();\
} while(false)
