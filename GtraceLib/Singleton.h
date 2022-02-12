#pragma once

namespace gtrace {

template<typename T>
class Singleton {
public:
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static T& getInstance() {
        static T instace;
        return instace;
    }
};
}
