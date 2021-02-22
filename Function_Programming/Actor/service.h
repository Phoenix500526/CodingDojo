#pragma once
#include <boost/asio.hpp>
#include <functional>
#include <iostream>

using boost::asio::ip::tcp;

// session 并不是 actor
template <typename EmitFunction>
class session : public std::enable_shared_from_this<session<EmitFunction>> {
public:
    session(tcp::socket&& socket, EmitFunction emit)
        : m_socket(std::move(socket)), m_emit(emit) {}
    void start() { do_read(); }

private:
    using shared_session = std::enable_shared_from_this<session<EmitFunction>>;
    void do_read() {
        auto self = shared_session::shared_from_this();
        boost::asio::async_read_until(
            m_socket, m_data, '\n',
            //此处传递 self 是为了保证 session 的生命周期，而传递 this
            //是为了方便编码，没有捕获 this 就必须写 self->do_read
            [this, self](const boost::system::error_code& error,
                         std::size_t size) {
                if (!error) {
                    std::istream is(&m_data);
                    std::string line;
                    std::getline(is, line);
                    m_emit(std::move(line));
                    do_read();
                }
            });
    }

    tcp::socket m_socket;
    boost::asio::streambuf m_data;
    EmitFunction m_emit;
};

template <class Socket, class EmitFunction>
auto make_shared_session(Socket&& socket, EmitFunction&& emit) {
    return std::make_shared<session<EmitFunction>>(
        std::forward<Socket>(socket), std::forward<EmitFunction>(emit));
}

class service {
public:
    using value_type = std::string;
    explicit service(boost::asio::io_service& service,
                     unsigned short port = 42042);
    service(const service&) = delete;
    service(service&& other) = default;

    template <class EmitFunction>
    void on_message(EmitFunction emit) {
        m_emit = emit;
        do_accept();
    }

private:
    void do_accept();
    tcp::acceptor m_acceptor;
    tcp::socket m_socket;
    std::function<void(std::string&&)> m_emit;
    friend std::ostream& operator<<(std::ostream& out, const service& service) {
        return out << "service object";
    }
};