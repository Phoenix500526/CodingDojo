#pragma once

#include "expected.h"

// mtry 相当于类型构造器，它能够将 Ret 类型构造成 expected monad 类型
// 对 mtry 做一些简单的封装就可以传递给 mbind 作为类型构造器使用
template <class F, class Ret = typename std::invoke_result_t<F>,
          class Exp = expected<Ret, std::exception_ptr> >
Exp mtry(F f) {
    try {
        return Exp::success(f());
    } catch (...) {
        // std::current_exception 捕获当前系统产生的异常，
        // 并返回一个 exception_ptr 的智能指针，该指针指向
        // 当前的异常。exception_ptr 可以在函数间传播，并
        // 在其他地方重新抛出，如 std::rethrow_exception(ptr)
        return Exp::error(std::current_exception());
    }
}