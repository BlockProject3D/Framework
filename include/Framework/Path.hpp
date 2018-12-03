#ifndef PATH_H_
# define PATH_H_

namespace Framework
{
    class ENGINE_API FPaths
    {
    private:
        static bpf::String GameDataRoot;
        static bpf::String ShaderFolder;
        static bpf::String ModulesFolder;
    public:
        static bpf::String Shaders();
        static bpf::String ShadersSrc();
        static bpf::String Models();
        static bpf::String Textures();
        static bpf::String Modules();
        static bpf::String Assets();
        static bpf::String GameRoot();
        static void SetShaderSubfolder(const bpf::String &folder);
        static void SetGameRoot(const bpf::String &folder, const bpf::String &suffix = "");
        static void SetModuleRoot(const bpf::String &folder);
    };
};

#endif /* !PATH_H_ */
