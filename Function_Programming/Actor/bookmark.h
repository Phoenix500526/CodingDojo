#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

#include "expected.h"
#include "mtry.h"

using json = nlohmann::json;

struct bookmark_t {
    std::string url;
    std::string text;
};

std::string to_string(const bookmark_t& page) {
    return "[" + page.text + "](" + page.url + ")";
}

std::ostream& operator<<(std::ostream& out, const bookmark_t& page) {
    return out << '[' << page.text << "](" << page.url << ")";
}

using expected_json = expected<json, std::exception_ptr>;
using expected_bookmark = expected<bookmark_t, std::exception_ptr>;

expected_bookmark bookmark_from_json(const json& data) {
    return mtry([&] {
        return bookmark_t{data.at("FirstURL"), data.at("Text")};
    });
}