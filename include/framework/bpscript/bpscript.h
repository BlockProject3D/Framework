#ifndef BPSCRIPT_BPSCRIPT_H_
#define BPSCRIPT_BPSCRIPT_H_

#include "framework/bpscript/rule.h"
#include "framework/bpscript/variable.h"

namespace Framework
{
    namespace BPScript
    {
        constexpr uint32 STATIC_MEM_INIT_SIZE = 16;
        
        class ENGINE_API Exception : public FRuntimeException
        {
        public:
            inline Exception(const FString &fname, const FString &msg)
                : FRuntimeException("Script", fname + " : " + msg)
            {
            }
        };

        /*
         * Types :
         *      BPSCRIPT_VTYPE_NUMBER
         *      BPSCRIPT_VTYPE_STRING
         *      BPSCRIPT_VTYPE_VECTOR
         *      BPSCRIPT_VTYPE_COLOR
         *      BPSCRIPT_VTYPE_BOOLEAN
         *      BPSCRIPT_VTYPE_LIST
         */

        enum EInstruction
        {
            BPSCRIPT_INST_NOOP,
            BPSCRIPT_INST_SETGLOB,
            BPSCRIPT_INST_GETGLOB,
            BPSCRIPT_INST_PUSH,
            BPSCRIPT_INST_CALL
        };
        
        class ENGINE_API Instruction
        {
        private:
            EInstruction Type;
            uint32 Data;
        public:
            inline Instruction(const EInstruction inst, const uint32 data)
                : Type(inst), Data(data)
            {
            }
            inline EInstruction GetType() const
            {
                return (Type);
            }
            inline uint32 GetData() const
            {
                return (Data);
            }
        };

        class ENGINE_API Script
        {
        private:
            FString Name;
            FDynamicByteBuf StaticMem;
            FList<Instruction> Instructions;
            FList<FUniquePtr<IParseRule>> Rules;

            FString Preprocess(IInputStream &stream);
            void Compile(FString &data);
        public:
            Script(const FString &name, IInputStream &stream);
            inline void AddInstruction(Instruction &&inst)
            {
                Instructions.Add(inst);
            }
            inline uint32 WriteStaticMem(IVariable &var)
            {
                uint32 pos = StaticMem.GetCursorPos();
                StaticMem.WriteByte(var.GetTypeByte());
                var.Write(StaticMem);
                return (pos);
            }
            inline void ReadStaticMem(const uint32 pos, IVariable &var)
            {
                StaticMem.Seek(pos);
                var.Read(StaticMem);
            }
            inline const FString &GetName() const
            {
                return (Name);
            }
            
            inline static void Throw(Script &src, uint32 line, const FString &msg)
            {
                throw Exception(src.GetName(), msg + " at line " + line);
            }
        };
    }
}

#endif /* !BPSCRIPT_BPSCRIPT_H_ */
