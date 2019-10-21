//
// Created by jmeny on 21.10.19.
//

#pragma once


#include <boost/mp11.hpp>
using namespace boost::mp11;

#include <array>

template<class L, std::size_t... I>
constexpr auto initPtrsImpl(std::index_sequence<I...>)
{
    using F = void (*)(void*);
    return std::array<F, sizeof...(I)>{+[](void* t){
        return (*reinterpret_cast<mp_at_c<L, I>*>(t)).execute();}...};
}

template<class L>
constexpr auto initPtrs()
{
    constexpr auto N = mp_size<L>();
    return initPtrsImpl<L>(std::make_index_sequence<N>());
}
