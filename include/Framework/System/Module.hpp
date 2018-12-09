#ifndef MODULE_H_
# define MODULE_H_

#include "Framework/Framework.hpp"
#include "Framework/System/ModuleException.hpp"

namespace bpf
{
    class BPF_API Module
    {
    private:
        bpf::String Path;
        void *Handle;

#ifdef WINDOWS
        String ObtainErrorString();
#endif
    public:
        /**
         * Opens a module binary at a specified path, file extension omitted.
         * @param path the path to the binary module without file extension
         * @throws ModuleException
         */
        Module(const bpf::String &path);
        ~Module();

        /**
         * Loads a symbol from the module
         * @param name the symbol name
         * @throws ModuleException
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
