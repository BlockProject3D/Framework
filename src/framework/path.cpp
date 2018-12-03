#include "Framework/Framework.hpp"

using namespace Framework;

FString FPaths::ShaderFolder = "NONE";
FString FPaths::ModulesFolder = "NULL";
FString FPaths::GameDataRoot = "";

FString FPaths::ShadersSrc()
{
    return (GameDataRoot + FString("Shaders/") + ShaderFolder + "/");
}

FString FPaths::Shaders()
{
    return (GameDataRoot + "Shaders/");
}

FString FPaths::Assets()
{
    return (GameDataRoot + FString("Assets/"));
}

FString FPaths::Models()
{
    return (GameDataRoot + "Assets/Models/");
}

FString FPaths::Textures()
{
    return (GameDataRoot + "Assets/Textures/");
}

FString FPaths::Modules()
{
    return (ModulesFolder + "/");
}

FString FPaths::GameRoot()
{
    return (GameDataRoot + "/");
}

void FPaths::SetGameRoot(const FString &folder, const FString &suffix)
{
    GameDataRoot = folder.Sub(0, folder.LastIndexOf('/') + 1);
    GameDataRoot += suffix;
}

void FPaths::SetShaderSubfolder(const Framework::FString &folder)
{
    ShaderFolder = folder;
}

void FPaths::SetModuleRoot(const FString &folder)
{
    ModulesFolder = folder.Sub(0, folder.LastIndexOf('/') + 1);
}
