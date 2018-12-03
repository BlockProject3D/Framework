#ifndef PROFILER_H_
# define PROFILER_H_

# ifdef BUILD_DEBUG
#  define PROFILER_PUSH_SECTION(name) Framework::FProfiler::PushSection(name)
#  define PROFILER_POP_SECTION() Framework::FProfiler::PopSection()
# else
#  define PROFILER_PUSH_SECTION(name)
#  define PROFILER_POP_SECTION()
# endif

# include <map>

namespace Framework
{
    struct ENGINE_API FProfilerSection
    {
        bpf::String Name;
        long long Time; //In micro seconds
        int Pos;
        uint32 CreationID;
    };

    class ENGINE_API FProfiler
    {
    private:
        uint32 CurCreationID;
        bpf::Map<bpf::String, FProfilerSection> ProfilerMap;
        bpf::Stack<FProfilerSection> Stack = bpf::Stack<FProfilerSection>(32);
        bpf::Array<FProfilerSection> Sections = bpf::Array<FProfilerSection>(0);

        void Push(const bpf::String &name);
        void Pop();
        FProfiler();
    public:
        static FProfiler *Instance();
        bpf::Array<FProfilerSection> &GenDisplayList();
        inline static void PushSection(const bpf::String &name)
        {
            Instance()->Push(name);
        }
        inline static void PopSection()
        {
            Instance()->Pop();
        }
    };
};

DEFINE_DEFAULT(Framework::FProfilerSection, Framework::FProfilerSection());

#endif /* !PROFILER_H_ */
