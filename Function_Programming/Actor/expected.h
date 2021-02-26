#pragma once

#ifdef NO_EXCEPTIONS
#define THROW_IF_EXCEPTIONS_ARE_ENABLED(WHAT) std::terminate()
#else
#define THROW_IF_EXCEPTIONS_ARE_ENABLED(WHAT) throw std::logic_error(WHAT)
#endif

// expected 是一个 monad
template <class T, class E>
class expected {
protected:
    union {
        T m_value;
        E m_error;
    };
    bool m_isValid;

    expected() {}

public:
    ~expected() {
        if (m_isValid) {
            m_value.~T();
        } else {
            m_error.~E();
        }
    }

    expected(const expected& other) : m_isValid(other.m_isValid) {
        if (m_isValid) {
            new (&m_value) T(other.m_value);
        } else {
            new (&m_error) E(other.m_error);
        }
    }

    expected(expected&& other) : m_isValid(other.m_isValid) {
        if (m_isValid) {
            new (&m_value) T(std::move(other.m_value));
        } else {
            new (&m_error) E(std::move(other.m_error));
        }
    }

    expected& operator=(expected other) {
        swap(other);
        return *this;
    }

    void swap(expected& other) {
        using std::swap;
        if (m_isValid) {
            if (other.m_isValid) {
                swap(m_value, other.m_value);
            } else {
                auto temp = std::move(other.m_error);
                other.m_error.~E();
                new (&other.m_value) T(std::move(m_value));
                m_value.~T();
                new (&m_error) E(std::move(temp));
                std::swap(m_isValid, other.m_isValid);
            }
        } else {
            if (other.m_isValid) {
                other.swap(*this);
            } else {
                swap(m_error, other.m_error);
            }
        }
    }

    operator bool() const { return is_Valid(); }

    bool is_Valid() const { return m_isValid; }

    template <class... ConsParams>
    static expected success(ConsParams&&... params) {
        expected result;
        result.m_isValid = true;
        new (&result.m_value) T(std::forward<ConsParams>(params)...);
        return result;
    }

    template <class... ConsParams>
    static expected error(ConsParams&&... params) {
        expected result;
        result.m_isValid = false;
        new (&result.m_error) E(std::forward<ConsParams>(params)...);
        return result;
    }

    T& get() {
        if (!m_isValid) {
            THROW_IF_EXCEPTIONS_ARE_ENABLED("expected<T, E> contains no value");
        }
        return m_value;
    }

    const T& get() const {
        if (!m_isValid) {
            THROW_IF_EXCEPTIONS_ARE_ENABLED("expected<T, E> contains no value");
        }
        return m_value;
    }

    T* operator->() { return &get(); }

    const T* operator->() const { return &get(); }

    E& error() {
        if (m_isValid) {
            THROW_IF_EXCEPTIONS_ARE_ENABLED(
                "There is no error in this expected<T, E>");
        }
        return m_error;
    }

    const E& error() const {
        if (m_isValid) {
            THROW_IF_EXCEPTIONS_ARE_ENABLED(
                "There is no error in this expected<T, E>");
        }
        return m_error;
    }

    template <class F>
    void visit(F f) {
        if (m_isValid) {
            f(m_value);
        } else {
            f(m_error);
        }
    }
};

// 注意，有些函数是没有返回值的，因此需要特化 void 版本
template <class E>
class expected<void, E> {
protected:
    union {
        void* m_value;
        E m_error;
    };
    bool m_isValid;

    expected() {}

public:
    ~expected() {
        if (!m_isValid) {
            m_error.~E();
        }
    }

    expected(const expected& other) : m_isValid(other.m_isValid) {
        if (!m_isValid) {
            new (&m_error) E(other.m_error);
        }
    }

    expected(expected&& other) : m_isValid(other.m_isValid) {
        if (!m_isValid) {
            new (&m_error) E(std::move(other.m_error));
        }
    }

    expected& operator=(expected other) {
        swap(other);
        return *this;
    }

    void swap(expected& other) {
        using std::swap;
        if (m_isValid) {
            if (!other.m_isValid) {
                auto temp = std::move(other.m_error);
                other.m_error.~E();
                new (&m_error) E(std::move(temp));
                std::swap(m_isValid, other.m_isValid);
            }
        } else {
            if (other.m_isValid) {
                other.swap(*this);
            } else {
                swap(m_error, other.m_error);
            }
        }
    }

    operator bool() const { return is_Valid(); }

    bool is_Valid() const { return m_isValid; }

    static expected success() {
        expected result;
        result.m_isValid = true;
        result.m_value = nullptr;
        return result;
    }

    template <class... ConsParams>
    static expected error(ConsParams&&... params) {
        expected result;
        result.m_isValid = false;
        new (&result.m_error) E(std::forward<ConsParams>(params)...);
        return result;
    }

    E& error() {
        if (m_isValid) {
            THROW_IF_EXCEPTIONS_ARE_ENABLED(
                "There is no error in this expected<T, E>");
        }
        return m_error;
    }

    const E& error() const {
        if (m_isValid) {
            THROW_IF_EXCEPTIONS_ARE_ENABLED(
                "There is no error in this expected<T, E>");
        }
        return m_error;
    }
};

// mbind：(M_a, f:a -> M_b) -> M_b，其中 M_* 代表 Monad(*)
// mbind 表明了 expected 是一个 monad
template <class T, class E, class Function,
          class ResultType = std::invoke_result_t<Function, T> >
ResultType mbind(const expected<T, E>& exp, Function f) {
    if (exp) {
        return std::invoke(f, exp.get());
    } else {
        return ResultType::error(exp.error());
    }
}

#undef THROW_IF_EXCEPTIONS_ARE_ENABLED