#ifndef PATH_H_
# define PATH_H_

namespace Framework
{
    class ENGINE_API FPaths
    {
    private:
        static FString GameDataRoot;
        static FString ShaderFolder;
        static FString ModulesFolder;
    public:
        static FString Shaders();
        static FString ShadersSrc();
        static FString Models();
        static FString Textures();
        static FString Modules();
        static FString Assets();
        static FString GameRoot();
        static void SetShaderSubfolder(const FString &folder);
        static void SetGameRoot(const FString &folder, const FString &suffix = "");
        static void SetModuleRoot(const FString &folder);
    };
};

#endif /* !PATH_H_ */
