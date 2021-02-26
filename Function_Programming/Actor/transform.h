#pragma once
#include <functional>

namespace reactive {

namespace detail {
template <class Sender, class Transformation,
          class SourceMessageType = typename Sender::value_type,
          class MessageType = decltype(std::declval<Transformation>()(
              std::declval<SourceMessageType>()))>
class transform_impl {
public:
    using value_type = MessageType;
    transform_impl(Sender&& sender, Transformation transformation)
        : m_sender(std::move(sender)), m_transformation(transformation) {}

    template <class EmitFunction>
    void on_message(EmitFunction emit) {
        m_emit = emit;
        m_sender.on_message([this](SourceMessageType&& message) {
            process_message(std::move(message));
        });
    }

    void process_message(SourceMessageType&& message) {
        m_emit(std::invoke(m_transformation, std::move(message)));
    }

private:
    Sender m_sender;
    Transformation m_transformation;
    std::function<void(MessageType&&)> m_emit;
};

template <class Transformation>
struct transform_helper {
    Transformation function;
};
}  // namespace detail

namespace operators {
template <class Transformation>
auto transform(Transformation&& transformation) {
    return detail::transform_helper<Transformation>{
        std::forward<Transformation>(transformation)};
}

template <class Sender, class Transformation>
auto operator|(Sender&& sender,
               detail::transform_helper<Transformation> transformation) {
    return detail::transform_impl<std::remove_reference_t<Sender>,
                                  Transformation>(std::forward<Sender>(sender),
                                                  transformation.function);
}

}  // namespace operators

}  // namespace reactive