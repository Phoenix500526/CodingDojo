#pragma once
#include <functional>
#include <list>

namespace reactive {
namespace detail {

template <class Sender, class SourceMessageType = typename Sender::value_type,
          class MessageType = typename SourceMessageType::value_type>
class join_impl {
public:
    using value_type = MessageType;
    join_impl(Sender&& sender) : m_sender(std::move(sender)) {}

    template <class EmitFunction>
    void on_message(EmitFunction emit) {
        m_emit = emit;
        m_sender.on_message([this](SourceMessageType&& message) {
            process_message(std::move(message));
        });
    }
    // 注意 join 的使用场景
    void process_message(SourceMessageType&& source) {
        m_sources.emplace_back(std::move(source));
        m_sources.back().on_message(m_emit);
    }

private:
    Sender m_sender;
    std::function<void(MessageType&&)> m_emit;
    std::list<SourceMessageType> m_sources;
};

struct join_helper {};
}  // namespace detail

namespace operators {

inline auto join() { return detail::join_helper{}; }

template <class Sender>
auto operator|(Sender&& sender, detail::join_helper join) {
    return detail::join_impl<std::remove_reference_t<Sender>>(
        std::forward<Sender>(sender));
}

}  // namespace operators
}  // namespace reactive