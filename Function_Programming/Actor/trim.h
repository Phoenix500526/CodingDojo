#pragma once

#include <algorithm>
#include <locale>
#include <string_view>

namespace detail {

bool is_not_space(char c) { return !std::isspace(c, std::locale()); }

}  // namespace detail

std::string trim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), detail::is_not_space));
    s.erase(std::find_if(s.rbegin(), s.rend(), detail::is_not_space).base(),
            s.end());
    return s;
}