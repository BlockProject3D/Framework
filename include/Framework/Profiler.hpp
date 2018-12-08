#pragma once

# ifdef BUILD_DEBUG
#  define PROFILER_PUSH_SECTION(name) Framework::FProfiler::PushSection(name)
#  define PROFILER_POP_SECTION() Framework::FProfiler::PopSection()
# else
#  define PROFILER_PUSH_SECTION(name)
#  define PROFILER_POP_SECTION()
# endif

namespace bpf
{
    struct BPF_API ProfilerSection
    {
        String Name;
        long long Time; //In micro seconds
        int Pos;
        uint32 CreationID;
    };

    class BPF_API Profiler
    {
    private:
        uint32 CurCreationID;
        Map<String, ProfilerSection> _map;
        Stack<ProfilerSection> _stack = Stack<ProfilerSection>(32);

        void Push(const String &name);
        void Pop();
        Profiler();

    public:
        static Profiler &Instance();
        Array<ProfilerSection> GenDisplayList();
        
        inline static void PushSection(const String &name)
        {
            Instance().Push(name);
        }

        inline static void PopSection()
        {
            Instance().Pop();
        }
    };
};

DEFINE_DEFAULT(bpf::ProfilerSection, bpf::ProfilerSection());
