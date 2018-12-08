#pragma once

#include <initializer_list>

namespace bpf
{
    template <typename T>
    union TypeExpander
    {
        T Data;
        uint8 Bytes[sizeof(T)];

        inline TypeExpander(T d)
            : Data(d)
        {
        }

        inline TypeExpander(std::initializer_list<uint8> d)
        {
            int i = 0;

            for (auto it = d.begin(); it != d.end(); ++it)
                Bytes[i++] = *it;
        }

        inline TypeExpander()
            : Data(0)
        {
        }
    };
}
