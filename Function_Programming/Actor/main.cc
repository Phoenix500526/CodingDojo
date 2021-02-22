#include <boost/asio.hpp>
#include <iostream>

#include "service.h"

using namespace std;

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
    // auto pipeline = service(event_loop);
    // Starting the Boost.ASIO service
    std::cerr << "Service is running...\n";
    event_loop.run();
    return 0;
}