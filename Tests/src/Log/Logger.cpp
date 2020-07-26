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

#include <Framework/Log/Logger.hpp>
#include <Framework/Memory/Utility.hpp>
#include <gtest/gtest.h>

class MemoryLog final : public bpf::log::ILogAdapter
{
private:
    bpf::collection::List<bpf::String> &_log;

public:
    explicit MemoryLog(bpf::collection::List<bpf::String> &log)
        : _log(log)
    {
    }

    void LogMessage(bpf::log::ELogLevel level, const bpf::String &category, const bpf::String &msg) final
    {
        _log.Add(bpf::String('[') + category + "] " + bpf::String::ValueOf((int)level) + ' ' + msg);
    }
};

TEST(Logger, Basic)
{
    bpf::collection::List<bpf::String> log;
    auto lg = bpf::log::Logger("UT");

    lg.AddHandler(bpf::memory::MakeUnique<MemoryLog>(log));
    lg.SetLevel(bpf::log::ELogLevel::DEBUG);
    lg.Debug("Test");
    EXPECT_STREQ(*log.Last(), "[UT] 3 Test");
    lg.Info("Test");
    EXPECT_STREQ(*log.Last(), "[UT] 2 Test");
    lg.Warning("Test");
    EXPECT_STREQ(*log.Last(), "[UT] 1 Test");
    lg.Error("Test");
    EXPECT_STREQ(*log.Last(), "[UT] 0 Test");
}

TEST(Logger, Move_1)
{
    bpf::collection::List<bpf::String> log;
    auto lg = bpf::log::Logger("UT");
    auto lg1 = bpf::log::Logger("UT1");

    lg.AddHandler(bpf::memory::MakeUnique<MemoryLog>(log));
    lg1.SetLevel(bpf::log::ELogLevel::DEBUG);
    lg1.AddHandler(bpf::memory::MakeUnique<MemoryLog>(log));
    lg = std::move(lg1);
    lg.Debug("Test");
    EXPECT_STREQ(*log.Last(), "[UT1] 3 Test");
    lg.Info("Test");
    EXPECT_STREQ(*log.Last(), "[UT1] 2 Test");
    lg.Warning("Test");
    EXPECT_STREQ(*log.Last(), "[UT1] 1 Test");
    lg.Error("Test");
    EXPECT_STREQ(*log.Last(), "[UT1] 0 Test");
}

TEST(Logger, Move_2)
{
    bpf::collection::List<bpf::String> log;
    auto lg1 = bpf::log::Logger("UT");
    auto lg = std::move(lg1);

    lg.AddHandler(bpf::memory::MakeUnique<MemoryLog>(log));
    lg.SetLevel(bpf::log::ELogLevel::DEBUG);
    lg.Debug("Test");
    EXPECT_STREQ(*log.Last(), "[UT] 3 Test");
    lg.Info("Test");
    EXPECT_STREQ(*log.Last(), "[UT] 2 Test");
    lg.Warning("Test");
    EXPECT_STREQ(*log.Last(), "[UT] 1 Test");
    lg.Error("Test");
    EXPECT_STREQ(*log.Last(), "[UT] 0 Test");
}

TEST(Logger, MinLevel_1)
{
    bpf::collection::List<bpf::String> log;
    auto lg = bpf::log::Logger("UT");

    lg.AddHandler(bpf::memory::MakeUnique<MemoryLog>(log));
    lg.SetLevel(bpf::log::ELogLevel::ERROR);
    lg.Debug("Test");
    EXPECT_EQ(log.Size(), 0u);
    lg.Info("Test");
    EXPECT_EQ(log.Size(), 0u);
    lg.Warning("Test");
    EXPECT_EQ(log.Size(), 0u);
    lg.Error("Test");
    EXPECT_STREQ(*log.Last(), "[UT] 0 Test");
}

TEST(Logger, MinLevel_2)
{
    bpf::collection::List<bpf::String> log;
    auto lg = bpf::log::Logger("UT");

    lg.AddHandler(bpf::memory::MakeUnique<MemoryLog>(log));
    lg.SetLevel(bpf::log::ELogLevel::INFO);
    lg.Debug("Test");
    EXPECT_EQ(log.Size(), 0u);
    lg.Info("Test");
    EXPECT_STREQ(*log.Last(), "[UT] 2 Test");
    lg.Warning("Test");
    EXPECT_STREQ(*log.Last(), "[UT] 1 Test");
    lg.Error("Test");
    EXPECT_STREQ(*log.Last(), "[UT] 0 Test");
}
