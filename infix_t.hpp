#ifndef __infix_t_hpp__
#define __infix_t_hpp__

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <functional>
#include "debug.hpp"
#include "postfix_t.hpp"

/**
 * An infix expression parser.
 */

struct infix_t
{
   /**
    * @param[in] p the computation engine
    */
   infix_t(postfix_t& p)
   :p_(p)
   {
   }

   /**
    * @param[in] s the infix expression candidate
    * @return a tuple whose first member is an iterator
    * revealing where the parse ended. On a successful
    * parse, this value will be equal to s.cend(). When the
    * parse is not successful, the second member is a
    * description of how the parse failed.
    */
   std::tuple
   <
      std::string::const_iterator,
      const char*
   >
   parse(const std::string& s)
   {
      const char* response_message = "";
      i_ = begin_ = s.cbegin();
      end_ = s.cend();
      try
      {
         descend(0);
      }
      catch (const char* e)
      {
         response_message = e;
      }

      return std::make_tuple(i_, response_message);
   }
private:
   enum types_t
   {
      infix,
      prefix,
      postfix
   };

   struct level_t
   {
      std::vector<std::string> glyphs;
      types_t type;
   };

   std::vector<level_t> levels_ =
   {
      {{"<<", ">>"}, types_t::infix},
      {{"+", "-"}, types_t::infix},
      {{"*", "/", "%"}, types_t::infix},
      {{"^"}, types_t::infix},
      {{"~", "-"}, types_t::prefix},
      {{"!"}, types_t::postfix},
   };

   std::string consume_one_of(
      const std::vector<std::string>& glyphs
   )
   {
      for (auto g : glyphs)
      {
         int len = g.size();
         if (len <= end_ - i_)
         {
            std::string candidate(i_, i_ + len);
            if (candidate == g)
            {
               i_ += len;
               return g;
            }
         }
      }
      return "";
   }

   void do_infix(std::size_t level)
   {
      advance();
      descend(level + 1);
      std::string g;
      while
      (
         advance()
         &&
         !(g = consume_one_of(levels_[level].glyphs)).empty()
      )
      {
         if (g.empty())
         {
            throw "missing infix operator";
         }
         descend(level + 1);
         p_.push(g);
      }
   }

   void do_prefix(std::size_t level)
   {
      advance();
      auto g = consume_one_of(levels_[level].glyphs);
      descend(level + 1);
      if (!g.empty())
      {
         if (g == "-")
         {
            std::string whole_thing(begin_, end_);
            boost::trim(whole_thing);
            if (whole_thing == "-")
            {
               p_.push("-");
            }
            else
            {
               p_.push("neg");
            }
         }
         else
         {
            p_.push(g);
         }
      }
   }

   void do_postfix(std::size_t level)
   {
      advance();
      descend(level + 1);
      auto g = consume_one_of(levels_[level].glyphs);
      if (!g.empty())
      {
         p_.push(g);
      }
   }

   void do_terminal()
   {
      advance();

      if (*i_ == '(')
      {
         ++i_;
         descend(0);
         advance();
         if (i_ < end_)
         {
            ensure(*i_ == ')');
            ++i_;
         }
      }
      else
      {
         auto saved = i_;
         while (i_ < end_ && is_terminal_glyph())
         {
            ++i_;
         }

         if (saved < i_)
         {
            std::string terminal(saved, i_);
            p_.push(terminal);
         }
      }
   }

   void descend(std::size_t level)
   {
      if (levels_.size() <= level)
      {
         do_terminal();
         return;
      }

      switch (levels_[level].type)
      {
         case types_t::infix:
            do_infix(level);
            break;
         case types_t::prefix:
            do_prefix(level);
            break;
         case types_t::postfix:
            do_postfix(level);
            break;
         default:
            throw "invalid type";
      }
   }

   bool advance()
   {
      while (is_whitespace() && i_ < end_)
      {
         ++i_;
      }

      bool result = i_ != end_;
      return result;
   }

   bool is_point() const
   {
      bool result = *i_ == '.';
      return result;
   }

   bool is_comma() const
   {
      bool result = *i_ == ',';
      return result;
   }

   bool is_decimal_digit() const
   {
      bool result =
         '0' <= *i_ && *i_ <= '9';
      return result;
   }

   bool is_terminal_glyph() const
   {
      bool result =
         is_decimal_digit()
         ||
         is_point()
         ||
         is_comma()
         ||
         is_alpha()
      ;
      return result;
   }

   bool is_alpha() const
   {
      bool result =
         ('A' <= *i_ && *i_ <= 'Z')
         ||
         ('a' <= *i_ && *i_ <= 'z');
      return result;
   }

   bool is_whitespace() const
   {
      bool result =*i_ <= ' ' || *i_ > '~';
      return result;
   }

   postfix_t& p_;
   std::string::const_iterator begin_;
   std::string::const_iterator i_;
   std::string::const_iterator end_;
};

#endif
