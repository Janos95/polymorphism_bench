//
// Author Janos Meny (on 10/7/19). Email janos.meny@googlemail.com.
//

#pragma once

#include <cstdint>
#include <new>
#include <algorithm>

#include <boost/mp11.hpp>
using namespace boost::mp11;


template<class L, std::size_t... I>
constexpr auto initPtrsImpl(std::index_sequence<I...>)
{
    using F = void (*)(void*);
    return std::array<F, sizeof...(I)>{+[](void* t){
            return (*reinterpret_cast<mp_at_c<L, I>*>(t)).execute();
        }...};
}

template<class L>
constexpr auto initPtrs()
{
    constexpr auto N = mp_size<L>();
    return initPtrsImpl<L>(std::make_index_sequence<N>());
}


template<class L>
class SmallTaskRef
{
public:

    SmallTaskRef() = default;

    template<class F>
    explicit SmallTaskRef(F& f)
    {
        using I = mp_find<L, F>;
        using S = mp_size<L>;
        static_assert(I() < S());
        set(std::addressof(f), I());
    }

    auto execute() const
    {
        return ptrs_[index()](ptr());
    }

private:

    [[nodiscard]] void* ptr() const {
        return reinterpret_cast<void*>(data_ & ((1ull << 48ull) - 1ull));
    }

    [[nodiscard]] std::uint16_t index() const {
        return data_ >> 48ull;
    }

    void set(void* p, std::uint16_t v) {
        auto ip = reinterpret_cast<std::uintptr_t>(p);
        assert(!(ip >> 48ull));
        ip |= static_cast<std::uintptr_t>(v) << 48ull;
        data_ = ip;
    }

    static constexpr auto ptrs_ = initPtrs<L>();

    std::uintptr_t data_;
};



class TaskRef
{
public:

    TaskRef() = default;

    template<class F>
    explicit TaskRef(F& f) : f_(std::addressof(f))
    {
        caller_ = +[](void* p){return (*reinterpret_cast<F*>(p)).execute();};
    }

    auto execute() const
    {
        caller_(f_);
    }

private:

    void (*caller_)(void*);
    void* f_;
};


