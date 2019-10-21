//
// Created by jmeny on 21.10.19.
//

#pragma once

#include "generate_vtable.hpp"


template<class L>
class RustTrait
{
public:
    template<class B>
    explicit RustTrait(B& box): p(std::addressof(*box)){}

    void execute() const
    {
        using I = mp_find<L, std::remove_reference_t<decltype(*std::)>>;
        static_assert(I() < mp_size<L>());
        vtable[I()](p);
    }

private:
    void* p;
    
    static constexpr auto vtable = initPtrs<L>();
};
