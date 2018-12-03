#ifndef PLATFORM_H_
# define PLATFORM_H_

# include <initializer_list>

namespace Framework
{
    class IFileSystem;
    class ISystemNetwork;
    class ISystemManager;
    class FString;

    enum EPlatformEndianess
    {
        PLATFORM_BIGENDIAN,
        PLATFORM_LITTLEENDIAN
    };

    template <typename T>
    union FTypeExpander
    {
        T		Data;
        uint8	Bytes[sizeof(T)];
        inline FTypeExpander(T d)
            : Data(d)
	{
	}

        inline FTypeExpander(std::initializer_list<uint8> d)
        {
            int i = 0;

            for (auto it = d.begin() ; it != d.end() ; ++it)
                Bytes[i++] = *it;
        }

        inline FTypeExpander()
            : Data(0)
	{
	}
    };

    class ENGINE_API FPlatform
    {
    private:
        static bool Exit;
        static IFileSystem *FileSys;
        static ISystemManager *BaseSys;
        static FMap<const char *, FString> PropMap;
    public:
        static void RequestExit(bool force=false);
        static bool IsExiting();
        DEPRECATED(static void *OpenDLL(const char *file));
        DEPRECATED(static void CloseDLL(void *hdl));
        DEPRECATED(static const char *GetDLLExt());
        DEPRECATED(static void *GetDLLSymbol(void *hdl, const char *name));
        DEPRECATED(static const char *GetLastError());
        static IFileSystem *GetFileSystem();
        static ISystemManager *GetBaseSystem();
        static EPlatformEndianess GetPlatformEndianess();
        static void ReverseBuffer(uint8 *buf, const uint32 size);

        /**
         * Reverse buffer buf groupsize per groupsize instead of byte per byte
         */
        static void ReverseBuffer(uint8 *buf, const uint32 groupsize, const uint32 size);
        static void Initialize();
        static void Shutdown();
        static const FString &GetProperty(const char *vname);
        static void SetProperty(const char *vname, const FString &prop);
    };
};

#endif /* !PLATFORM_H_ */
