#include <gtest/gtest.h>

#include <nlohmann/json.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <vector>

#include "bookmark.h"
#include "expected.h"
#include "filter.h"
#include "join.h"
#include "mtry.h"
#include "service.h"
#include "sink.h"
#include "transform.h"
#include "trim.h"
#include "values.h"

using namespace std::literals::string_literals;
using namespace reactive::operators;

template <class MessageType>
struct with_expected_reply {
    MessageType value;
    std::string expected_reply;
    void reply(const std::string& message) const {
        EXPECT_EQ(message, expected_reply);
    }
};

template <class MessageType>
auto make_with_expected_reply(MessageType&& value,
                              const std::string& expected_reply) {
    return with_expected_reply<MessageType>{std::forward<MessageType>(value),
                                            expected_reply};
}

template <class F>
auto lift_with_expected_reply(F&& function) {
    return [function = std::forward<F>(function)](auto&& ws) {
        return make_with_expected_reply(std::invoke(function, ws.value),
                                        ws.expected_reply);
    };
}

template <class F>
auto apply_with_expected_reply(F&& function) {
    return [function = std::forward<F>(function)](auto&& ws) {
        return std::invoke(function, std::forward<decltype(ws)>(ws).value);
    };
}

TEST(ActorTest, ActorTest) {
    using namespace ranges::v3;

    auto transform = [](auto f) {
        return view::transform(lift_with_expected_reply(f));
    };
    auto filter = [](auto f) {
        return view::filter(apply_with_expected_reply(f));
    };
    auto sink = [](auto f) {
        return view::transform([f](auto&& ws) {
            f(ws);
            return ws.expected_reply;
        });
    };
    auto test = [](const std::string& message,
                   const std::string& expected_reply) {
        return with_expected_reply<std::string>{message, expected_reply};
    };

    std::vector<with_expected_reply<std::string>> source{
        test("", ""),

        test("hello", "ERROR: Request not understood\n"),

        test("{}", "ERROR: Request not understood\n"),

        test("{\"FirstURL\" : \"http://www.iso.org/\",\"Text\" : \"ISO\"}",
             "ERROR: Not a C++-related link\n"),

        test("{\"FirstURL\" : \"http://isocpp.org/\",\"Text\" : \"ISO C++ -- "
             "Official site\"}",
             "OK: [ISO C++ -- Official site](http://isocpp.org/)\n")};

    auto pipeline =
        source |
        transform(trim)

        // Ignoring comments and empty messages
        | filter([](const std::string& message) {
              return message.length() > 0 && message[0] != '#';
          })

        // Trying to parse the input
        | transform([](const std::string& message) {
              return mtry([&] { return json::parse(message); });
          })

        // Converting the result into the bookmark
        | transform(
              [](const auto& exp) { return mbind(exp, bookmark_from_json); })

        | sink([](const auto& message) {
              const auto exp_bookmark = message.value;

              if (!exp_bookmark) {
                  message.reply("ERROR: Request not understood\n");
                  return;
              }

              if (exp_bookmark->text.find("C++") != std::string::npos) {
                  message.reply("OK: " + to_string(exp_bookmark.get()) + "\n");
              } else {
                  message.reply("ERROR: Not a C++-related link\n");
              }
          });

    auto tmp = source | sink([](const auto& message) {});
    // Forcing the range to evaluate its elements
    pipeline | to_vector;
}