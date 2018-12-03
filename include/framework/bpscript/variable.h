#ifndef BPSCRIPT_VARIABLE_H_
#define BPSCRIPT_VARIABLE_H_

#include "framework/bmath.h"

namespace Framework
{
    namespace BPScript
    {
        class Script;
        
        enum EVarType
        {
            BPSCRIPT_VTYPE_NUMBER,
            BPSCRIPT_VTYPE_STRING,
            BPSCRIPT_VTYPE_VECTOR,
            BPSCRIPT_VTYPE_COLOR,
            BPSCRIPT_VTYPE_BOOLEAN,
            BPSCRIPT_VTYPE_LIST
        };

        class ENGINE_API IVariable
        {
        public:
            virtual ~IVariable() {}
            virtual uint8 GetTypeByte() const = 0;
            virtual void Write(FDynamicByteBuf &buf) = 0;
            virtual void Read(FDynamicByteBuf &buf) = 0;
        };
        
        class ENGINE_API VNumber final : public IVariable
        {
        private:
            float Val;
        public:
            inline VNumber(float v)
                : Val(v)
            {
            }
            inline float GetValue() const
            {
                return (Val);
            }
            inline uint8 GetTypeByte() const
            {
                return ((uint8)BPSCRIPT_VTYPE_NUMBER);
            }
            inline void Write(FDynamicByteBuf &buf)
            {
                buf.WriteFloat(Val);
            }
            inline void Read(FDynamicByteBuf &buf)
            {
                Val = buf.ReadFloat();
            }
        };
        
        class ENGINE_API VString final : public IVariable
        {
        private:
            FString Val;
        public:
            inline VString(const FString &v)
                : Val(v)
            {
            }
            inline const FString &GetValue() const
            {
                return (Val);
            }
            inline uint8 GetTypeByte() const
            {
                return ((uint8)BPSCRIPT_VTYPE_STRING);
            }
            inline void Write(FDynamicByteBuf &buf)
            {
                buf.WriteString(Val + '\n');
            }
            inline void Read(FDynamicByteBuf &buf)
            {
                buf.ReadLine(Val);
            }
        };

        class ENGINE_API VVector final : public IVariable
        {
        private:
            FVector Val;
        public:
            inline VVector(const FVector &v)
                : Val(v)
            {
            }
            inline FVector GetValue() const
            {
                return (Val);
            }
            inline uint8 GetTypeByte() const
            {
                return ((uint8)BPSCRIPT_VTYPE_VECTOR);
            }
            inline void Write(FDynamicByteBuf &buf)
            {
                buf.WriteFloat(Val.X);
                buf.WriteFloat(Val.Y);
                buf.WriteFloat(Val.Z);
            }
            inline void Read(FDynamicByteBuf &buf)
            {
                Val.X = buf.ReadFloat();
                Val.Y = buf.ReadFloat();
                Val.Z = buf.ReadFloat();
            }
        };

        class ENGINE_API VColor final : public IVariable
        {
        private:
            FColor Val;
        public:
            inline VColor(const FColor &v)
                : Val(v)
            {
            }
            inline FColor GetValue() const
            {
                return (Val);
            }
            inline uint8 GetTypeByte() const
            {
                return ((uint8)BPSCRIPT_VTYPE_COLOR);
            }
            inline void Write(FDynamicByteBuf &buf)
            {
                buf.WriteByte(Val.R);
                buf.WriteByte(Val.G);
                buf.WriteByte(Val.B);
                buf.WriteByte(Val.A);
            }
            inline void Read(FDynamicByteBuf &buf)
            {
                Val.R = buf.ReadByte();
                Val.G = buf.ReadByte();
                Val.B = buf.ReadByte();
                Val.A = buf.ReadByte();
            }
        };

        class ENGINE_API VBool final : public IVariable
        {
        private:
            bool Val;
        public:
            inline VBool(const bool v)
                : Val(v)
            {
            }
            inline bool GetValue() const
            {
                return (Val);
            }
            inline uint8 GetTypeByte() const
            {
                return ((uint8)BPSCRIPT_VTYPE_BOOLEAN);
            }
            inline void Write(FDynamicByteBuf &buf)
            {
                buf.WriteByte(Val == true ? 1 : 0);
            }
            inline void Read(FDynamicByteBuf &buf)
            {
                Val = buf.ReadByte() == 1 ? true : false;
            }
        };

        class ENGINE_API VList final : public IVariable
        {
        private:
            FList<uint32> Vars;
            Script &Src;
        public:
            inline VList(Script &s)
                : Src(s)
            {
            }
            inline void Remove(const uint32 id)
            {
                Vars.RemoveAt(id);
            }
            inline uint8 GetTypeByte() const
            {
                return ((uint8)BPSCRIPT_VTYPE_LIST);
            }
            void Add(IVariable &var);
            void Get(IVariable &var, const uint32 id);
            void Write(FDynamicByteBuf &buf);
            void Read(FDynamicByteBuf &buf);
        };
    }
}

#endif /* !BPSCRIPT_VARIABLE_H_ */
