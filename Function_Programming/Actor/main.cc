#include <boost/asio.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

#include "expected.h"
#include "filter.h"
#include "join.h"
#include "mtry.h"
#include "service.h"
#include "sink.h"
#include "transform.h"
#include "trim.h"
#include "values.h"

using namespace std;
using namespace reactive::operators;
using json = nlohmann::json;

struct bookmark_t {
    string url;
    string text;
};

using expected_json = expected<json, std::exception_ptr>;
using expected_bookmark = expected<bookmark_t, std::exception_ptr>;

string to_string(const bookmark_t& page) {
    return "[" + page.text + "](" + page.url + ")";
}

ostream& operator<<(std::ostream& out, const bookmark_t& page) {
    return out << '[' << page.text << "](" << page.url << ")";
}

expected_bookmark bookmark_from_json(const json& data) {
    return mtry([&] {
        return bookmark_t{data.at("FirstURL"), data.at("Text")};
    });
}

int main(int argc, char const* argv[]) {
    std::ios_base::sync_with_stdio(false);
    boost::asio::io_service event_loop;
    auto transform = [](auto f) {
        return reactive::operators::transform(lift_with_client(f));
    };
    auto filter = [](auto f) {
        return reactive::operators::filter(apply_with_client(f));
    };
    // Starting the Boost.ASIO service

    auto pipeline =
        reactive::values<unsigned short>{10086, 10011, 10010} |
        reactive::operators::transform([&event_loop](unsigned short port) {
            return service(event_loop, port);
        }) |
        join() | filter([](const string& message) {
            return message.length() > 0 && message[0] != '#';
        }) |
        transform(trim) | transform([](const string& message) {
            return mtry([&] { return json::parse(message); });
        }) |
        transform(
            [](const auto& exp) { return mbind(exp, bookmark_from_json); }) |
        sink([](const auto& message) {
            const auto exp_bookmark = message.value;

            if (!exp_bookmark) {
                message.reply("ERROR: Request not understood\n");
                return;
            }

            if (exp_bookmark->text.find("C++") != std::string::npos) {
                message.reply("OK: " + to_string(exp_bookmark.get()) + "\n");
            } else {
                message.reply("ERROR: Not a C++-related link\n");
            }
        });
    cerr << "Service is running...\n";
    event_loop.run();
    return 0;
}