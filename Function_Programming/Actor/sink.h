#pragma once

#include <functional>

namespace reactive {

namespace detail {
template <class Sender, class Function,
          class MessageType = typename Sender::value_type>
class sink_impl {
public:
    using value_type = MessageType;
    // 注意，此处 Sender&& 是右值引用，而不是万能引用，这是因为 sender
    // 会沿着反应流向后传递，传递到谁的手中，谁就是 sender 的所有者
    sink_impl(Sender&& sender, Function function)
        : m_sender(std::move(sender)), m_function(function) {
        m_sender.on_message([this](MessageType&& message) {
            process_message(std::move(message));
        });
    }

    void process_message(MessageType&& message) const {
        std::invoke(m_function, std::move(message));
    }

private:
    Sender m_sender;
    Function m_function;
};

template <class Function>
struct sink_helper {
    Function function;
};
}  // namespace detail

namespace operators {

template <class Function>
auto sink(Function&& function) {
    return detail::sink_helper<Function>{std::forward<Function>(function)};
}

template <class Sender, class Function>
auto operator|(Sender&& sender, detail::sink_helper<Function> sink) {
    return detail::sink_impl<std::remove_reference_t<Sender>, Function>(
        std::forward<Sender>(sender), sink.function);
}

}  // namespace operators

}  // namespace reactive