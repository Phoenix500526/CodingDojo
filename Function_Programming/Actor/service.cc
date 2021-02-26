#include "service.h"

service::service(boost::asio::io_service& service, unsigned short port)
    : m_acceptor(service, tcp::endpoint(tcp::v4(), port)), m_socket(service) {}

void service::do_accept() {
    m_acceptor.async_accept(
        m_socket, [this](const boost::system::error_code& error) {
            if (!error) {
                make_shared_session(std::move(m_socket), m_emit)->start();
            } else {
                std::cerr << error.message() << '\n';
            }
            do_accept();
        });
}