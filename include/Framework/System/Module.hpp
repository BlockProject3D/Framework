#ifndef MODULE_H_
# define MODULE_H_

#include "Framework/Framework.hpp"

namespace Framework
{
    class ENGINE_API FModuleException : public bpf::RuntimeException
    {
    public:
        inline FModuleException(const bpf::String &msg) noexcept
            : bpf::RuntimeException("Module", msg)
        {
        }
    };

    class ENGINE_API FModule
    {
    private:
        bpf::String Path;
        void *Handle;

    public:
        /**
         * Opens a module binary at a specified path, file extension omitted.
         * @param path the path to the binary module without file extension
         */
        FModule(const bpf::String &path);
        ~FModule();

        /**
         * Loads a symbol from the module
         * @param name the symbol name
         */
        void *LoadSymbol(const bpf::String &name);

        /**
         * Returns the path to this module
         */
        inline const bpf::String &GetPath() const
        {
            return (Path);
        }
    };
}

#endif /* !MODULE_H_ */
