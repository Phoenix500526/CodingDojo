#pragma once

#include <functional>

namespace reactive {

template <class T>
class values {
public:
    using value_type = T;
    explicit values(std::initializer_list<T> values) : m_values(values) {}
    template <class EmitFunction>
    void on_message(EmitFunction emit) {
        m_emit = emit;
        std::for_each(m_values.cbegin(), m_values.cend(),
                      [&](T value) { m_emit(std::move(value)); });
    }

private:
    std::vector<T> m_values;
    std::function<void(T&&)> m_emit;
};

}  // namespace reactive