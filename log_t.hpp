/**
 * @file
 * Logging for C++. Heavily inspired by Petru Marginean's
 * article.
 *
 * http://www.drdobbs.com/cpp/logging-in-c/201804215.
 *
 * @copyright Ken Smith kgsmith at gmail.com, 2013.
 * @license This software is released under the Boost
 * Software License, version 1.0.
 * See LICENSE_1_0.txt or
 * http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __log_t_hpp__
#define __log_t_hpp__

#include <cstdio>
#include <sstream>
#include <ctime>
#include <sys/time.h>
#include <algorithm>
#include "stream_saver_t.hpp"

enum log_levels_t
{
   iii,
   eee
};

struct log_t
{
   log_t(log_levels_t lvl)
      :lvl_(lvl)
   {
      struct timeval tv = {0, 0};
      (void) gettimeofday(&tv, nullptr);
      
      ss_
         << ((lvl) == log_levels_t::eee ? "eee" : "iii")
         << ","
         << tv.tv_sec
         << ".";
      {
         stream_saver_t saver(ss_);
         ss_.width(6);
         ss_.fill('0');
         ss_
            << tv.tv_usec;
      }

      ss_
         << ",";
   }

   ~log_t()
   {
      if (threshold() <= lvl_)
      {
         fprintf(stdout, "%s\n", ss_.str().c_str());
         fflush(stdout);
      }
   }

   static log_levels_t & threshold()
   {
#if defined(DEBUG)
      static log_levels_t threshold = iii;
#else
      static log_levels_t threshold = eee;
#endif
      return threshold;
   }

   std::stringstream & get()
   {
      return ss_;
   }
private:
   const log_levels_t lvl_;
   std::stringstream ss_;
};

#define ooo(lvl)\
   if (log_t::threshold() > (lvl))\
      ;\
   else\
      log_t(lvl).get()\
         << __FILE__\
         << ","\
         << __LINE__\
         << ","\
         << __func__\
         << ","

#endif
