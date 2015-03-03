#pragma once

#include "debug.hpp"

struct commaifier_t
{
    commaifier_t(const std::string& s)
    {
        const int len = s.size();
        s_.reserve(len + (len - 1) / 3);
        int rotor = 3 - len % 3;
        for (auto c : s)
        {
            if (rotor == 0)
            {
                s_.push_back(',');
            }
            rotor = (rotor + 1) % 3;
            s_.push_back(c);
        }
    }

    const std::string& str() const
    {
        return s_;
    }
private:
    std::string s_;
};
