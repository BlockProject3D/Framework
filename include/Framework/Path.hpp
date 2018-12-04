#ifndef PATH_H_
# define PATH_H_

namespace bpf
{
    class BPF_API Paths
    {
    private:
        static String AppDataRoot;
        static String ModulesFolder;
    public:
        static String Modules();
        static String Assets();
        static String AppRoot();
        static void SetAppRoot(const String &folder);
        static void SetModuleRoot(const String &folder);
    };
};

#endif /* !PATH_H_ */
