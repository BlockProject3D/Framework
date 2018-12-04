#include "Framework/Framework.hpp"

using namespace bpf;

String Paths::ModulesFolder = "NULL";
String Paths::AppDataRoot = "";

String Paths::Assets()
{
    return (AppDataRoot + String("Assets/"));
}

String Paths::Modules()
{
    return (ModulesFolder + "/");
}

String Paths::AppRoot()
{
    return (AppDataRoot + "/");
}

void Paths::SetAppRoot(const String &folder)
{
    AppDataRoot = folder;
}

void Paths::SetModuleRoot(const bpf::String &folder)
{
    ModulesFolder = folder;
}
