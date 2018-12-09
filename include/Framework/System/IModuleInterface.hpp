#pragma once

namespace bpf
{
    class BPF_API IModuleInterface
    {
    public:
        virtual ~IModuleInterface() {}
        virtual void OnLoadModule() = 0;
        virtual void OnUnloadModule() = 0;
    };
}
