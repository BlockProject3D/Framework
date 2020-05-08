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

#ifndef STREAM_H_
# define STREAM_H_

#include "Framework/System/Platform.hpp"

namespace Framework
{
    class ENGINE_API IInputStream
    {
    public:
        virtual ~IInputStream() {}
        virtual bool ReadLine(bpf::String &line) = 0;
        virtual uint8 ReadByte() = 0;
        virtual int16 ReadShort() = 0;
        virtual uint16 ReadUShort() = 0;
        virtual float ReadFloat() = 0;
        virtual int ReadInt() = 0;
        virtual uint32 ReadUInt() = 0;
        virtual uint32 Read(uint8 *buffer, const uint32 sizebytes) = 0;
    };

    class ENGINE_API IOutputStream
    {
    public:
        virtual ~IOutputStream() {}
        virtual void WriteString(const bpf::String &str) = 0;
        virtual void WriteByte(const uint8 b) = 0;
        virtual void WriteShort(const int16 s) = 0;
        virtual void WriteUShort(const uint16 s) = 0;
        virtual void WriteFloat(const float f) = 0;
        virtual void WriteInt(const int i) = 0;
        virtual void WriteUInt(const uint32 u) = 0;
        virtual void Write(const uint8 *buffer, const uint32 sizebytes) = 0;
    };

    class ENGINE_API FByteBuf : public IOutputStream, public IInputStream
    {
    protected:
        uint32 DataPtr;
        uint32 CurSize;
        uint32 Size;
        uint8 *Data;
        bool Del;
        EPlatformEndianess Order;
        void OrderData(uint8 *buf, const uint32 bsize);
    public:
        FByteBuf(const uint32 size, const EPlatformEndianess order = PLATFORM_LITTLEENDIAN);
        FByteBuf(uint8 *bytes, const uint32 size, const EPlatformEndianess order = PLATFORM_LITTLEENDIAN);
        FByteBuf();
        virtual ~FByteBuf();
        bool ReadLine(bpf::String &line);
        uint8 ReadByte();
        uint16 ReadUShort();
        inline int16 ReadShort()
        {
            return ((int16)ReadUShort());
        }
        float ReadFloat();
        inline int ReadInt()
        {
            return ((int)ReadUInt());
        }
        uint32 ReadUInt();
        uint32 Read(uint8 *buffer, const uint32 sizebytes);
        void WriteString(const bpf::String &str);
        virtual void WriteByte(const uint8 b); //Virtual for FDynamicByteBuf
        inline void WriteShort(const int16 s)
        {
            WriteUShort((uint16)s);
        }
        void WriteUShort(const uint16 s);
        void WriteFloat(const float f);
        inline void WriteInt(const int i)
        {
            WriteUInt((uint32)i);
        }
        void WriteUInt(const uint32 u);
        void Write(const uint8 *buffer, const uint32 sizebytes);
        void Seek(const uint32 newpos);

        inline void Clear()
        {
            DataPtr = 0;
            CurSize = 0;
        }

        /**
         * Returns a pointer to the raw data in this buffer
         */
        inline uint8 *GetRawData()
        {
            return (Data);
        }

        /**
         * Returns the current written size of the buffer in bytes
         */
        inline uint32 GetCurSize() const
        {
            return (CurSize);
        }

        /**
         * Returns the position of the cursor in the buffer
         */
        inline uint32 GetCursorPos() const
        {
            return (DataPtr);
        }

        /**
         * Returns the current maximum allocated size of this buffer
         */
        inline uint32 GetMaxSize() const
        {
            return (Size);
        }

        /**
         * Returns a byte at specified pos
         * @param pos the position of the byte to return
         */
        inline uint8 operator[](const uint32 pos) const
        {
            return (pos >= GetMaxSize() ? (uint8)0 : Data[pos]);
        }

        /**
         * Shifts the buffer to the left of count bytes
         */
        void Shift(uint32 count);
    };

    class ENGINE_API FDynamicByteBuf : public FByteBuf
    {
    private:
        uint32 SizeInc;
    public:
        FDynamicByteBuf(const uint32 initsize, const EPlatformEndianess order = PLATFORM_LITTLEENDIAN);
        virtual ~FDynamicByteBuf();
        void WriteByte(const uint8 b);
    };
};

#endif /* !STREAM_H_ */
