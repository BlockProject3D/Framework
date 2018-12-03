#include "framework/framework.h"
#include "framework/bpscript/variable.h"
#include "framework/bpscript/bpscript.h"

using namespace Framework;
using namespace Framework::BPScript;

void VList::Add(IVariable &var)
{
    Vars.Add(Src.WriteStaticMem(var));
}

void VList::Get(IVariable &var, const uint32 id)
{
    Src.ReadStaticMem(Vars[id], var);
}

void VList::Write(FDynamicByteBuf &buf)
{
    buf.WriteUInt(Vars.Size());
    for (auto it = Vars.Begin() ; it ; ++it)
        buf.WriteUInt(*it);
}

void VList::Read(FDynamicByteBuf &buf)
{
    uint32 count = buf.ReadUInt();

    while (count > 0)
    {
        Vars.Add(buf.ReadUInt());
        --count;
    }
}
