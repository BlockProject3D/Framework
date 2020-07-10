// Copyright (c) 2020, BlockProject 3D
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
//     * Neither the name of BlockProject 3D nor the names of its contributors
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

#include <Framework/Memory/ObjectConstructor.hpp>
#include <gtest/gtest.h>

class MyClass
{
public:
    int Val;

    MyClass(int a)
        : Val(a)
    {
    }

    MyClass()
        : Val(0)
    {
    }

    BP_USE_CONSTRUCTOR(MyClass, MyClass);
};

BP_MAP_CONSTRUCTOR(MyClass, MyClass, int)
BP_MAP_CONSTRUCTOR(MyClass, MyClass)

TEST(ObjectConstructor, Basic)
{
    EXPECT_NE(MyClass::GetConstructor(), Null);
    EXPECT_NE(MyClass::GetConstructor<int>(), Null);
}

TEST(ObjectConstructor, Instantiate_1)
{
    auto ptr = MyClass::GetConstructor()->MakeUnique();
    EXPECT_EQ(ptr->Val, 0);
    EXPECT_EQ((*ptr).Val, 0);
}

TEST(ObjectConstructor, Instantiate_2)
{
    auto ptr = MyClass::GetConstructor<int>()->MakeUnique(42);
    EXPECT_EQ(ptr->Val, 42);
    EXPECT_EQ((*ptr).Val, 42);
}
