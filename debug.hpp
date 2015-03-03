#ifndef __debug_hpp__
#define __debug_hpp__

#include "log_t.hpp"

#define ensure(pred)\
   do\
   {\
      if (!(pred))\
      {\
         throw #pred;\
      }\
   } while (0)

#define show(x)\
   ooo(iii)\
      << #x\
      << " = "\
      << x

#define hex(x)\
   ooo(iii)\
      << #x\
      << " = 0x"\
      << std::hex\
      << x

#endif
