#include "framework/framework.h"
#include "framework/bpscript/bpscript.h"

using namespace Framework;
using namespace Framework::BPScript;

Script::Script(const FString &name, IInputStream &stream)
    : Name(name), StaticMem(STATIC_MEM_INIT_SIZE)
{
    FString data = Preprocess(stream);
    Rules.Add(NewObject<FUnique, ParseRuleGlobal>());
    Rules.Add(NewObject<FUnique, ParseRuleFunction>());
    Compile(data);
}

FString Script::Preprocess(IInputStream &stream)
{
    FString data;
    FString line;
    
    while (stream.ReadLine(line))
    {
        line = line.Replace(" ", "").Replace("\t", "");
        if (line == FString::Empty || line.StartsWith("//"))
            continue;
        data += line + "\n";
    }
    return (data);
}

void Script::Compile(FString &data)
{
    uint32 line = 1;
    Instructions.Clear();
    while (data.Len() > 0)
    {
        if (data[0] == '\n')
            ++line;
        bool flag = false;
        for (auto it = Rules.Begin() ; it ; ++it)
        {
            if (it->Run(*this, line, data))
            {
                flag = true;
                break;
            }
        }
        if (!flag)
            Script::Throw(*this, line, "Parse error");
    }
}
