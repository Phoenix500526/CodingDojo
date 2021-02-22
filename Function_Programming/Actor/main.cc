#include <boost/asio.hpp>
#include <iostream>

#include "service.h"
#include "sink.h"

using namespace std;
using namespace reactive::operators;

struct bookmark_t {
    string url;
    string text;
};

string to_string(const bookmark_t& page) {
    return "[" + page.text + "](" + page.url + ")";
}

ostream& operator<<(std::ostream& out, const bookmark_t& page) {
    return out << '[' << page.text << "](" << page.url << ")";
}

int main(int argc, char const* argv[]) {
    boost::asio::io_service event_loop;
    auto sink_to_cerr =
        sink([](const auto& message) { std::cerr << message << '\n'; });
    // Starting the Boost.ASIO service
    auto pipeline = service(event_loop) | sink_to_cerr;
    cerr << "Service is running...\n";
    event_loop.run();
    return 0;
}