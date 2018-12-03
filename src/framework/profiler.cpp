#include <chrono>
#include "framework/framework.h"

using namespace Framework;

FProfiler::FProfiler()
    : CurCreationID(0)
{
}

void FProfiler::Push(const FString &name)
{
    FProfilerSection section;

    section.Name = name;
    section.Time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    section.Pos = Stack.Size();
    if (!ProfilerMap.HasKey(name))
    {
        section.CreationID = CurCreationID++;
        ProfilerMap[name] = section;
    }
    else
        section.CreationID = ProfilerMap[name].CreationID;
    Stack.Push(section);
}

FArray<FProfilerSection> &FProfiler::GenDisplayList()
{
    if (Sections.Length() != CurCreationID)
        Sections = FArray<FProfilerSection>(CurCreationID);
    for (auto it = ProfilerMap.Begin() ; it ; ++it)
        Sections[(*it).Value.CreationID] = (*it).Value;
    return (Sections);
}

void FProfiler::Pop()
{
    long long curt = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    FProfilerSection section = Stack.Pop();

    section.Time = curt - section.Time;
    ProfilerMap[section.Name] = section;
}

FProfiler *FProfiler::Instance()
{
    static FProfiler Inst;

    return (&Inst);
}
