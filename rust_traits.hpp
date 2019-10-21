//
// Created by jmeny on 21.10.19.
//

#pragma once

#include "generate_vtable.hpp"


template<class L>
class RustTrait
{
public:

    RustTrait() = default;

    template<class F>
    explicit RustTrait(F& f): p(std::addressof(f))
    {
        using I = mp_find<L, std::remove_reference_t<F>>;
        static_assert(I() < mp_size<L>());
        caller = vtable[I()];
    }

    void execute() const
    {
        caller(p);
    }

private:
    void* p;
    void (*caller)(void*);
    
    static constexpr auto vtable = initPtrs<L>();
};
