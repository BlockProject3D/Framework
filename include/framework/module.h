#ifndef MODULE_H_
# define MODULE_H_

namespace Framework
{
    class ENGINE_API FModuleException : public FRuntimeException
    {
    public:
        inline FModuleException(const FString &msg) noexcept
            : FRuntimeException("Module", msg)
        {
        }
    };

    class ENGINE_API FModule
    {
    private:
        FString Path;
        void *Handle;
    public:
        /**
         * Opens a module binary at a specified path, file extension omitted.
         * @param path the path to the binary module without file extension
         */
        FModule(const FString &path);
        ~FModule();

        /**
         * Loads a symbol from the module
         * @param name the symbol name
         */
        void *LoadSymbol(const FString &name);

        /**
         * Returns the path to this module
         */
        inline const FString &GetPath() const
        {
            return (Path);
        }
    };
}

#endif /* !MODULE_H_ */
