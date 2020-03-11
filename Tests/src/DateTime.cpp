// Copyright (c) 2020, BlockProject
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
#include <Framework/System/DateTime.hpp>
#include <Framework/System/Stringifier.DateTime.hpp>

TEST(DateTime, Creation)
{
    bpf::system::DateTime dt(3, 3, 2020, 1, 50, 0);
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Tue Mar 3 2020 1:50:0");
    bpf::system::DateTime dt1(3, 3, 2020);
    EXPECT_STREQ(*bpf::String::ValueOf(dt1), "Tue Mar 3 2020 0:0:0");
}

TEST(DateTime, UTCTime)
{
    bpf::system::DateTime dt(3, 3, 2020, 1, 50, 0);
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Tue Mar 3 2020 1:50:0");
    EXPECT_STREQ(*bpf::String::ValueOf(dt.ToUTCTime().ToLocalTime()), "Tue Mar 3 2020 1:50:0");
}

TEST(DateTime, UTCNow)
{
    bpf::system::DateTime now = bpf::system::DateTime::Now();
    bpf::system::DateTime utcnow = bpf::system::DateTime::UTCNow();

    EXPECT_EQ(now.ToUTCTime(), utcnow);
    EXPECT_EQ(utcnow.ToLocalTime(), now);
}

TEST(DateTime, Compare)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Now();
    bpf::system::DateTime dt1 = dt + bpf::system::TimeSpan(3600) + bpf::system::TimeSpan(60);
    EXPECT_TRUE(dt < dt1);
    EXPECT_TRUE(dt1 > dt);
    EXPECT_TRUE(dt == dt);
    EXPECT_TRUE(dt1 == dt1);
    EXPECT_TRUE(dt == dt);
    EXPECT_TRUE(dt != dt1);
    EXPECT_TRUE(dt1 != dt);
    EXPECT_FALSE(dt == dt1);
    EXPECT_FALSE(dt1 == dt);

}

TEST(DateTime, Diff)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Now();
    bpf::system::DateTime dt1 = dt + bpf::system::TimeSpan(3600) + bpf::system::TimeSpan(60);
    bpf::system::TimeSpan diff = dt1 - dt;
    EXPECT_EQ(diff.Hours, 1);
    EXPECT_EQ(diff.Minutes, 1);
    EXPECT_EQ(diff.Seconds, 0);
    dt1 = dt + bpf::system::TimeSpan(3600) + bpf::system::TimeSpan(61);
    diff = dt1 - dt;
    EXPECT_EQ(diff.Hours, 1);
    EXPECT_EQ(diff.Minutes, 1);
    EXPECT_EQ(diff.Seconds, 1);
}

TEST(DateTime, Subtract_1)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Now();
    bpf::system::DateTime dt1 = dt + bpf::system::TimeSpan(3600) + bpf::system::TimeSpan(61);
    dt1 = dt1 - bpf::system::TimeSpan(60);
    dt1 = dt1 - bpf::system::TimeSpan(0);
    bpf::system::TimeSpan diff = dt1 - dt;
    EXPECT_EQ(diff.Hours, 1);
    EXPECT_EQ(diff.Minutes, 0);
    EXPECT_EQ(diff.Seconds, 1);
}

TEST(DateTime, Subtract_2)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Now();
    bpf::system::DateTime dt1 = dt + bpf::system::TimeSpan(3600) + bpf::system::TimeSpan(61);
    dt1 = dt1 - bpf::system::TimeSpan(60);
    dt1 = dt1 - bpf::system::TimeSpan(0);
    dt1 = dt1 - bpf::system::TimeSpan(1);
    dt1 = dt1 - bpf::system::TimeSpan(3600);
    bpf::system::TimeSpan diff = dt1 - dt;
    EXPECT_EQ(diff.Hours, 0);
    EXPECT_EQ(diff.Minutes, 0);
    EXPECT_EQ(diff.Seconds, 0);
    EXPECT_EQ(diff.TotalSeconds, 0);
    EXPECT_EQ(dt, dt1);
}

TEST(DateTime, Parse_1)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Parse("Tue Mar 3 2020 1:50:0");
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Tue Mar 3 2020 1:50:0");
    dt = bpf::system::DateTime();
    EXPECT_TRUE(bpf::system::DateTime::TryParse("Tue Mar 3 2020 1:50:0", dt));
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Tue Mar 3 2020 1:50:0");
}

TEST(DateTime, Parse_2)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Parse("Tue Mar 3 2020");
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Tue Mar 3 2020 0:0:0");
    dt = bpf::system::DateTime();
    EXPECT_TRUE(bpf::system::DateTime::TryParse("Tue Mar 3 2020", dt));
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Tue Mar 3 2020 0:0:0");
}

TEST(DateTime, Parse_3)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Parse("Tue Mar 3");
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Tue Mar 3 2020 0:0:0");
    dt = bpf::system::DateTime();
    EXPECT_TRUE(bpf::system::DateTime::TryParse("Tue Mar 3", dt));
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Tue Mar 3 2020 0:0:0");
}

TEST(DateTime, Parse_4)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Parse("03/04/2020 05:53:3");
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Wed Mar 4 2020 5:53:3");
    dt = bpf::system::DateTime();
    EXPECT_TRUE(bpf::system::DateTime::TryParse("03/04/2020 05:53:3", dt));
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Wed Mar 4 2020 5:53:3");
}

TEST(DateTime, Parse_5)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Parse("03-04-2020 5:53:0");
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Wed Mar 4 2020 5:53:0");
    dt = bpf::system::DateTime();
    EXPECT_TRUE(bpf::system::DateTime::TryParse("03-04-2020 5:53:0", dt));
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Wed Mar 4 2020 5:53:0");
}

TEST(DateTime, Parse_6)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Parse("03/04/2020");
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Wed Mar 4 2020 0:0:0");
    dt = bpf::system::DateTime();
    EXPECT_TRUE(bpf::system::DateTime::TryParse("03/04/2020", dt));
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Wed Mar 4 2020 0:0:0");
}

TEST(DateTime, Parse_7)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Parse("03-04-2020");
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Wed Mar 4 2020 0:0:0");
    dt = bpf::system::DateTime();
    EXPECT_TRUE(bpf::system::DateTime::TryParse("03-04-2020", dt));
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Wed Mar 4 2020 0:0:0");
}

TEST(DateTime, Parse_8)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Parse("03/04");
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Wed Mar 4 2020 0:0:0");
    dt = bpf::system::DateTime();
    EXPECT_TRUE(bpf::system::DateTime::TryParse("03/04", dt));
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Wed Mar 4 2020 0:0:0");
}

TEST(DateTime, Parse_9)
{
    bpf::system::DateTime dt = bpf::system::DateTime::Parse("03-04");
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Wed Mar 4 2020 0:0:0");
    dt = bpf::system::DateTime();
    EXPECT_TRUE(bpf::system::DateTime::TryParse("03-04", dt));
    EXPECT_STREQ(*bpf::String::ValueOf(dt), "Wed Mar 4 2020 0:0:0");
}

TEST(DateTime, Parse_Err_1)
{
    bpf::system::DateTime dt;
    EXPECT_THROW(bpf::system::DateTime::Parse("Tue Dfg 3 2020 1:50:0"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse("Tue Dec 3 2020 :0"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse("Tue Dec 3 2020 0:0:x"), bpf::ParseException);
    EXPECT_FALSE(bpf::system::DateTime::TryParse("Tue Dfg 3 2020 1:50:0", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("Tue Dec 3 2020 :0", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("Tue Dec 3 2020 0:0:x", dt));
}

TEST(DateTime, Parse_Err_2)
{
    bpf::system::DateTime dt;
    EXPECT_THROW(bpf::system::DateTime::Parse("Tue Dec 3 2020 x:v:n"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse("1-x-v x:v:n"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse("1/x/4 1:0:1"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse("1/x-4 1:0:1"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse("1-2-3 x:v:n"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse("1-2-3 :v"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse("Tue Dfg 3"), bpf::ParseException);
    EXPECT_FALSE(bpf::system::DateTime::TryParse("Tue Dec 3 2020 x:v:n", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("Tue Dfg 3", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("1-x-v x:v:n", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("1-2-3 x:v:n", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("1-2-3 :v", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("1/x/4 1:0:1", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("1/x-4 1:0:1", dt));
}

TEST(DateTime, Parse_Err_3)
{
    bpf::system::DateTime dt;
    EXPECT_THROW(bpf::system::DateTime::Parse("020 0:1"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse("Tue 3 2020 1:4"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse("Dec 3 2020 1:2"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse(" 2020 x:v"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse("asjdoq979823"), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse(""), bpf::ParseException);
    EXPECT_THROW(bpf::system::DateTime::Parse("\n"), bpf::ParseException);
    EXPECT_FALSE(bpf::system::DateTime::TryParse("020 0:1", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("Tue 3 2020 1:4", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("Dec 3 2020 1:2", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse(" 2020 x:v", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("asjdoq979823", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("", dt));
    EXPECT_FALSE(bpf::system::DateTime::TryParse("\n", dt));
}