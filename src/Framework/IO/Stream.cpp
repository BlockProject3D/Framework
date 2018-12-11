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

#include <cstring>
#include "Framework/IO/Stream.hpp"

using namespace Framework;

FByteBuf::FByteBuf(const uint32 size, const EPlatformEndianess order)
    : DataPtr(0), CurSize(0), Size(size), Data(new uint8[size]), Del(true), Order(order)
{
}

FByteBuf::FByteBuf()
    : DataPtr(0), CurSize(0), Size(0), Data(Null), Del(false), Order(PLATFORM_LITTLEENDIAN)
{
}

FByteBuf::FByteBuf(uint8 *bytes, const uint32 size, const EPlatformEndianess order)
    : DataPtr(0), CurSize(0), Size(size), Data(bytes), Del(false), Order(order)
{
}

FByteBuf::~FByteBuf()
{
  if (Del)
    delete[] Data;
}

void FByteBuf::OrderData(uint8 *buf, const uint32 bsize)
{
    if (Order == PLATFORM_LITTLEENDIAN && FPlatform::GetPlatformEndianess() != PLATFORM_LITTLEENDIAN)
        FPlatform::ReverseBuffer(buf, bsize);
    else if (Order == PLATFORM_BIGENDIAN && FPlatform::GetPlatformEndianess() != PLATFORM_BIGENDIAN)
        FPlatform::ReverseBuffer(buf, bsize);
}

void FByteBuf::Seek(const uint32 newpos)
{
    if (newpos >= Size)
        return;
    DataPtr = newpos;
}

bool FByteBuf::ReadLine(bpf::String &line)
{
    if (DataPtr >= Size)
        return (false);
    line = bpf::String::Empty;
    while (DataPtr < Size && Data[DataPtr] != '\n')
    {
        line += (char)Data[DataPtr];
        DataPtr++;
    }
    return (true);
}

uint8 FByteBuf::ReadByte()
{
    if (DataPtr >= Size)
        return (0);
    return (Data[DataPtr++]);
}

uint16 FByteBuf::ReadUShort()
{
    uint8 buf[2];
    uint16 *ptr;

    buf[0] = ReadByte();
    buf[1] = ReadByte();
    OrderData(buf, 2);
    ptr = (uint16 *)buf;
    return (*ptr);
}

float FByteBuf::ReadFloat()
{
    uint8 buf[4];
    float *ptr;

    buf[0] = ReadByte();
    buf[1] = ReadByte();
    buf[2] = ReadByte();
    buf[3] = ReadByte();
    OrderData(buf, 4);
    ptr = (float *)buf;
    return (*ptr);
}

uint32 FByteBuf::ReadUInt()
{
    uint8 buf[4];
    uint32 *ptr;

    buf[0] = ReadByte();
    buf[1] = ReadByte();
    buf[2] = ReadByte();
    buf[3] = ReadByte();
    OrderData(buf, 4);
    ptr = (uint32 *)buf;
    return (*ptr);
}

uint32 FByteBuf::Read(uint8 *buffer, uint32 sizebytes)
{
    for (uint32 i = 0 ; i < sizebytes ; i++)
        buffer[i] = ReadByte();
    return (sizebytes);
}

void FByteBuf::WriteString(const bpf::String &str)
{
    for (int i = 0 ; i < str.Len() ; i++)
        WriteByte((uint8)str[i]);
}

void FByteBuf::WriteByte(const uint8 b)
{
    if (DataPtr >= Size)
        return;
    if (DataPtr >= CurSize)
        ++CurSize;
    Data[DataPtr++] = b;
}

void FByteBuf::WriteUShort(const uint16 s)
{
    uint8 buf[2];

    buf[0] = (uint8)s;
    buf[1] = (uint8)(s >> 8);
    OrderData(buf, 2);
    Write(buf, 2);
}

void FByteBuf::WriteFloat(const float f)
{
    uint8 buf[4];
    uint32 tmp;

    tmp = *(uint32 *)&f;
    buf[0] = (uint8)tmp;
    buf[1] = (uint8)(tmp >> 8);
    buf[2] = (uint8)(tmp >> 16);
    buf[3] = (uint8)(tmp >> 24);
    OrderData(buf, 4);
    Write(buf, 4);
}

void FByteBuf::WriteUInt(const uint32 u)
{
    uint8 buf[4];
    uint32 tmp;

    tmp = *(uint32 *)&u;
    buf[0] = (uint8)tmp;
    buf[1] = (uint8)(tmp >> 8);
    buf[2] = (uint8)(tmp >> 16);
    buf[3] = (uint8)(tmp >> 24);
    OrderData(buf, 4);
    Write(buf, 4);
}

void FByteBuf::Write(const uint8 *buffer, uint32 sizebytes)
{
    for (uint32 i = 0 ; i < sizebytes ; i++)
        WriteByte(buffer[i]);
}

void FByteBuf::Shift(uint32 count)
{
    if (DataPtr < count)
        count = DataPtr;
    memmove(Data, Data + count, CurSize - count);
    DataPtr -= count;
    CurSize -= count;
}

FDynamicByteBuf::FDynamicByteBuf(const uint32 initsize, const EPlatformEndianess order)
    : FByteBuf(), SizeInc(initsize)
{
    Data = (uint8 *)malloc(initsize);
    Size = initsize;
    DataPtr = 0;
    Order = order;
    memset(Data, 0, Size);
}

FDynamicByteBuf::~FDynamicByteBuf()
{
    free(Data);
}

void FDynamicByteBuf::WriteByte(uint8 b)
{
    if (DataPtr >= Size)
    {
        Size += SizeInc;
        Data = (uint8 *)realloc(Data, Size);
        memset(Data + (Size - SizeInc), 0, SizeInc);
    }
    if (DataPtr >= CurSize)
        ++CurSize;
    Data[DataPtr++] = b;
}
