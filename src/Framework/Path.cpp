#include "Framework/Framework.hpp"

using namespace Framework;

bpf::String FPaths::ShaderFolder = "NONE";
bpf::String FPaths::ModulesFolder = "NULL";
bpf::String FPaths::GameDataRoot = "";

bpf::String FPaths::ShadersSrc()
{
    return (GameDataRoot + bpf::String("Shaders/") + ShaderFolder + "/");
}

bpf::String FPaths::Shaders()
{
    return (GameDataRoot + "Shaders/");
}

bpf::String FPaths::Assets()
{
    return (GameDataRoot + bpf::String("Assets/"));
}

bpf::String FPaths::Models()
{
    return (GameDataRoot + "Assets/Models/");
}

bpf::String FPaths::Textures()
{
    return (GameDataRoot + "Assets/Textures/");
}

bpf::String FPaths::Modules()
{
    return (ModulesFolder + "/");
}

bpf::String FPaths::GameRoot()
{
    return (GameDataRoot + "/");
}

void FPaths::SetGameRoot(const bpf::String &folder, const bpf::String &suffix)
{
    GameDataRoot = folder.Sub(0, folder.LastIndexOf('/') + 1);
    GameDataRoot += suffix;
}

void FPaths::SetShaderSubfolder(const bpf::String &folder)
{
    ShaderFolder = folder;
}

void FPaths::SetModuleRoot(const bpf::String &folder)
{
    ModulesFolder = folder.Sub(0, folder.LastIndexOf('/') + 1);
}
