#ifndef BPSCRIPT_RULE_H_
#define BPSCRIPT_RULE_H_

namespace Framework
{
    namespace BPScript
    {
        class Script;

        class IParseRule
        {
        public:
            virtual ~IParseRule() {}
            virtual bool Run(Script &cript, uint32 line, FString &data) = 0;
        };
        
        class VariableParser
        {
        private:
            Script &Src;
            uint32 Line;
            uint32 Pos;

            void ParseString(const FString &data);
            void ParseColor(const FString &data);
            void ParseVector(const FString &data);
            void ParseBool(const FString &data);
            void ParseList(const FString &data);

        public:
            VariableParser(const FString &var, Script &src, uint32 line);
            inline uint32 GetMemAddr() const
            {
                return (Pos);
            }
        };

        class ParseRuleGlobal final : public IParseRule
        {
        public:
            bool Run(Script &cript, uint32 line, FString &data);
        };

        class ParseRuleFunction final : public IParseRule
        {
        public:
            bool Run(Script &cript, uint32 line, FString &data);
        };
    }
}


#endif /* !BPSCRIPT_RULE_H_ */
