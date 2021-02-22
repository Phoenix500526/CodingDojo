#include <iostream>
#include <boost/asio.hpp>
#include "services.h"

using namespace std;

struct bookmark_t
{
	string url;
	string text;
};

string to_string(const bookmark_t& page){
	return "[" + page.text + "](" + page.url + ")";
}

ostream& operator<<(std::ostream& out, const bookmark_t& page){
	return out << '[' << page.text << "](" << page.url << ")";
}

int main(int argc, char const *argv[]) { 
	boost::asio::io_service event_loop;
	return 0; 
}