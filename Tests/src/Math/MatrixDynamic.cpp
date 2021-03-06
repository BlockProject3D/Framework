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

#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include <Framework/Math/Matrix.hpp>
#include <Framework/Math/Stringifier.Matrix.hpp>

TEST(MatrixDynamic, Create_Test_1)
{
    bpf::math::Matrix<int> mat(2, 3);
    bpf::math::Matrix<int> mat1(2, 3);

    EXPECT_EQ(mat, mat1);
    mat(0, 0) = 1;
    mat(1, 1) = 1;
    mat(1, 2) = 1;
    mat(0, 2) = 3;
    mat1(0, 0) = 1;
    mat1(1, 1) = 1;
    mat1(1, 2) = 1;
    mat1(0, 2) = 3;
    EXPECT_EQ(mat, mat1);
}

TEST(MatrixDynamic, Create_Test_2)
{
    bpf::math::Matrix<int> mat = bpf::math::Matrix<int>::Identity(3);
    bpf::math::Matrix<int> mat1(3, 3, { 1, 0, 0,
                                  0, 1, 0,
                                  0, 0, 1 });

    EXPECT_EQ(mat, mat1);
}

TEST(MatrixDynamic, Transpose_Square_1)
{
    bpf::math::Matrix<int> mat = bpf::math::Matrix<int>::Identity(3);
    bpf::math::Matrix<int> mat1 = mat.Transpose();

    EXPECT_EQ(mat, mat1);
}

TEST(MatrixDynamic, Transpose_Square_2)
{
    bpf::math::Matrix<int> mat = bpf::math::Matrix<int>(2, 2, {
        2, 3,
        4, 1
    });
    bpf::math::Matrix<int> expected = bpf::math::Matrix<int>(2, 2, {
        2, 4,
        3, 1
    });

    EXPECT_EQ(mat.Transpose(), expected);
    EXPECT_NE(mat, expected);
    EXPECT_FALSE(mat == expected);
}

TEST(MatrixDynamic, Transpose_NonSquare)
{
    bpf::math::Matrix<int> mat(2, 3);
    bpf::math::Matrix<int> mat1(3, 2, {
        1, 0,
        0, 1,
        3, 1
    });

    mat(0, 0) = 1;
    mat(1, 1) = 1;
    mat(1, 2) = 1;
    mat(0, 2) = 3;
    EXPECT_EQ(mat.Transpose(), mat1);
    EXPECT_NE(mat, mat1);
    EXPECT_FALSE(mat == mat1);
}

TEST(MatrixDynamic, SwapRows_Square)
{
    bpf::math::Matrix<int> mat = bpf::math::Matrix<int>::Identity(3);
    bpf::math::Matrix<int> mat1(3, 3, {
        4, 0, 1,
        0, 1, 2,
        1, 0, 3
    });

    //Input
    //1 0 3
    //0 1 2
    //4 0 1
    //SwapRows 0, 2
    //4 0 1
    //0 1 2
    //1 0 3
    mat(0, 2) = 3;
    mat(1, 2) = 2;
    mat(2, 0) = 4;
    mat.SwapRows(0, 2);
    EXPECT_EQ(mat, mat1);
}

TEST(MatrixDynamic, SwapRows_NonSquare)
{
    bpf::math::Matrix<int> mat(2, 3);
    bpf::math::Matrix<int> mat1(2, 3, {
        0, 1, 2,
        1, 0, 3
    });

    //Input
    //1 0 3
    //0 1 2
    //SwapRows 0, 1
    //0 1 2
    //1 0 3
    mat(0, 0) = 1;
    mat(1, 1) = 1;
    mat(0, 2) = 3;
    mat(1, 2) = 2;
    mat.SwapRows(0, 1);
    EXPECT_EQ(mat, mat1);
}

TEST(MatrixDynamic, SwapColumns_Square)
{
    bpf::math::Matrix<int> mat = bpf::math::Matrix<int>::Identity(3);
    bpf::math::Matrix<int> mat1(3, 3, {
        3, 0, 1,
        2, 1, 0,
        1, 0, 4
    });

    //Input
    //1 0 3
    //0 1 2
    //4 0 1
    //SwapColumns 0, 2
    //3 0 1
    //2 1 0
    //1 0 4
    mat(0, 2) = 3;
    mat(1, 2) = 2;
    mat(2, 0) = 4;
    mat.SwapColumns(0, 2);
    EXPECT_EQ(mat, mat1);
}

TEST(MatrixDynamic, SwapColumns_NonSquare)
{
    bpf::math::Matrix<int> mat(2, 3);
    bpf::math::Matrix<int> mat1(2, 3, {
        0, 1, 3,
        1, 0, 2
    });

    //Input
    //1 0 3
    //0 1 2
    //SwapColumns 0, 1
    //0 1 3
    //1 0 2
    mat(0, 0) = 1;
    mat(1, 1) = 1;
    mat(0, 2) = 3;
    mat(1, 2) = 2;
    mat.SwapColumns(0, 1);
    EXPECT_EQ(mat, mat1);
}

TEST(MatrixDynamic, Multiply_Square_Test1)
{
    bpf::math::Matrix<int> mat = bpf::math::Matrix<int>::Identity(3);
    bpf::math::Matrix<int> mat1 = bpf::math::Matrix<int>::Identity(3);

    EXPECT_EQ(mat, mat * mat1);
}

TEST(MatrixDynamic, Multiply_Square_Test2)
{
    bpf::math::Matrix<int> mat(3, 3, {
        2, 5, 6,
        9, 8, 7,
        3, 1, 0
    });
    bpf::math::Matrix<int> mat1 = bpf::math::Matrix<int>::Identity(3);

    EXPECT_EQ(mat, mat * mat1);
}

TEST(MatrixDynamic, Multiply_NonSquare_Test1)
{
    bpf::math::Matrix<int> mat(4, 3, {
        2, 5, 6,
        9, 8, 7,
        3, 1, 0,
        8, 4, 5,
    });
    bpf::math::Matrix<int> mat1(3, 4, {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
    });
    bpf::math::Matrix<int> res = mat * mat1;
    bpf::math::Matrix<int> expected(4, 4, {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    });

    EXPECT_EQ(res, expected);
}

TEST(MatrixDynamic, Multiply_NonSquare_Test2)
{
    bpf::math::Matrix<int> mat(4, 3, {
        2, 5, 6,
        9, 8, 7,
        3, 1, 0,
        8, 4, 5,
    });
    bpf::math::Matrix<int> mat1(3, 4, {
        2, 5, 6, 1,
        9, 8, 7, 1,
        3, 1, 0, 1,
    });
    bpf::math::Matrix<int> res = mat * mat1;
    bpf::math::Matrix<int> expected(4, 4, {
        67, 56, 47, 13,
        111, 116, 110, 24,
        15, 23, 25, 4,
        67, 77, 76, 17
    });

    EXPECT_EQ(res, expected);
}

TEST(MatrixDynamic, Multiply_NonSquare_Test3)
{
    bpf::math::Matrix<int> mat(4, 5, {
        2, 5, 6, 8, 9,
        9, 8, 7, 7, 1,
        3, 1, 0, 6, 8,
        8, 4, 5, 0, 2
    });
    bpf::math::Matrix<int> mat1(5, 6, {
        2, 5, 6, 1, 0, 6,
        9, 8, 7, 1, 0, 6,
        3, 1, 0, 1, 0, 6,
        3, 1, 0, 1, 0, 6,
        3, 1, 0, 1, 0, 6
    });
    bpf::math::Matrix<int> res = mat * mat1;
    bpf::math::Matrix<int> expected(4, 6, {
        118, 73, 47, 30, 0, 180,
        135, 124, 110, 32, 0, 192,
        57, 37, 25, 18, 0, 108,
        73, 79, 76, 19, 0, 114
    });

    EXPECT_EQ(res, expected);
    EXPECT_THROW(mat * expected, bpf::math::IncompatibleMatrixSizeException);
}

TEST(MatrixDynamic, Matrix_Inverse_1)
{
    bpf::math::Matrix<float> mat(2, 2, {
        0, 6,
        9, 5
    });
    bpf::math::Matrix<float> res = mat * mat.Inverse();
    bpf::math::Matrix<float> expected(2, 2, {
        1, 0,
        0, 1
    });
    
    EXPECT_EQ(res, expected);
}

TEST(MatrixDynamic, Matrix_Inverse_2)
{
    bpf::math::Matrix<float> mat(2, 2);
    bpf::math::Matrix<float> mat1(2, 3);

    EXPECT_THROW(mat.Inverse(), bpf::math::NonInvertibleMatrixException);
    EXPECT_THROW(mat1.Inverse(), bpf::math::NonSquareMatrixException);
    EXPECT_THROW(mat1.GetDeterminant(), bpf::math::NonSquareMatrixException);
}

TEST(MatrixDynamic, Stringifier)
{
    bpf::math::Matrix<int> mat(4, 4, {
        5, 9, 8, 5,
        7, 2, 6, 7,
        3, 4, 1, 6,
        4, 5, 8, 1
    });

    EXPECT_STREQ(*bpf::String::ValueOf(mat), "Matrix(\n\t5\t9\t8\t5\n\t7\t2\t6\t7\n\t3\t4\t1\t6\n\t4\t5\t8\t1\n)");
}
