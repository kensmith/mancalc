#ifndef __num_t_hpp__
#define __num_t_hpp__

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using int_t = boost::multiprecision::number
<
   boost::multiprecision::cpp_int_backend<>,
   boost::multiprecision::expression_template_option::et_off
>;

using num_t = boost::multiprecision::number
<
   boost::multiprecision::cpp_dec_float<1024>,
   boost::multiprecision::expression_template_option::et_off
>;

#endif
