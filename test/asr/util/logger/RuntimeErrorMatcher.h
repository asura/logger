#ifndef RUNTIMEERRORMATCHER_H
#define RUNTIMEERRORMATCHER_H

#include <catch2/catch.hpp>
#include <sstream>
#include <stdexcept>
#include <string>

class RuntimeErrorMatcher : public Catch::MatcherBase<std::runtime_error>
{
    const std::string m_error_message;

public:
    explicit RuntimeErrorMatcher(const std::string& the_error_message)
        : m_error_message(the_error_message)
    {
    }

    bool match(const std::runtime_error& e) const override
    {
        return e.what() == m_error_message;
    }

    std::string describe() const override
    {
        std::ostringstream oss;
        oss << "std::runtime_error has message : " << m_error_message;
        return oss.str();
    }
};

#endif  // RUNTIMEERRORMATCHER_H
