// Copyright (c) 2018, BlockProject
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//     * Neither the name of BlockProject nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <cassert>
#include <iostream>
#include <gtest/gtest.h>

/*int main()
{
    bpf::String str = "é This is a test !";
    bpf::Tuple<int, float, double> tp;
    bpf::List<bpf::ModuleEntry *> testlst;

    tp.Get<0>() = 42;
    tp.Get<1>() = 42.42f;
    tp.Get<2>() = 423.423;

    std::cout << "--------------" << std::endl;
    std::cout << tp.Size() << std::endl;
    std::cout << tp.Get<int>() << std::endl;
    std::cout << tp.Get<float>() << std::endl;
    std::cout << tp.Get<double>() << std::endl;
    std::cout << "--------------" << std::endl;

    std::cout << str.Size() << std::endl;
    std::cout << str.Len() << std::endl;
    std::cout << (str[0] == bpf::String::UTF32("é")) << std::endl;

    bpf::DateTime dt = bpf::DateTime::LocalTime();
    std::cout << *dt.ToString() << std::endl;
    bpf::DateTime dt1 = dt + bpf::DateTime(3600) + bpf::DateTime(61);
    std::cout << *dt1.ToString() << std::endl;

    assert(dt < dt1);
    assert(dt1 > dt);
    assert(dt == dt);
    assert(dt1 == dt1);
    assert(dt != dt1);
    assert(dt1 != dt);

    system("PAUSE");
}*/

#include <Framework/String.hpp>

int main(int ac, char **av)
{
    ::testing::InitGoogleTest(&ac, av);
    return (RUN_ALL_TESTS());
}
