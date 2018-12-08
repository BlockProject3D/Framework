#include <chrono>
#include "Framework/Framework.hpp"

using namespace bpf;

Profiler::Profiler()
    : CurCreationID(0)
{
}

void Profiler::Push(const bpf::String &name)
{
    ProfilerSection section;

    section.Name = name;
    section.Time = std::chrono::duration_cast<std::chrono::microseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    section.Pos = _stack.Size();
    if (!_map.HasKey(name))
    {
        section.CreationID = CurCreationID++;
        _map[name] = section;
    }
    else
        section.CreationID = _map[name].CreationID;
    _stack.Push(section);
}

Array<ProfilerSection> Profiler::GenDisplayList()
{
    Array<ProfilerSection> sections = Array<ProfilerSection>(CurCreationID);
    for (auto it = _map.Begin() ; it ; ++it)
        sections[(*it).Value.CreationID] = (*it).Value;
    return (sections);
}

void Profiler::Pop()
{
    long long curt = std::chrono::duration_cast<std::chrono::microseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    ProfilerSection section = _stack.Pop();

    section.Time = curt - section.Time;
    _map[section.Name] = section;
}

Profiler &Profiler::Instance()
{
    static Profiler inst;

    return (inst);
}
