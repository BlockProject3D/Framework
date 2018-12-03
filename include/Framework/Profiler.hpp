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
        FString Name;
        long long Time; //In micro seconds
        int Pos;
        uint32 CreationID;
    };

    class ENGINE_API FProfiler
    {
    private:
        uint32 CurCreationID;
        FMap<FString, FProfilerSection> ProfilerMap;
        FStack<FProfilerSection> Stack = FStack<FProfilerSection>(32);
        FArray<FProfilerSection> Sections = FArray<FProfilerSection>(0);

        void Push(const FString &name);
        void Pop();
        FProfiler();
    public:
        static FProfiler *Instance();
        FArray<FProfilerSection> &GenDisplayList();
        inline static void PushSection(const FString &name)
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
