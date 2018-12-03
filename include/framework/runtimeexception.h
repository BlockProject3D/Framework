#ifndef RUNTIMEEXCEPTION_H_
#define RUNTIMEEXCEPTION_H_

namespace Framework
{
    class ENGINE_API FRuntimeException : public FException
    {
    private:
        FString Type;
        FString Message;
    public:
        virtual ~FRuntimeException() {}
        FRuntimeException(const FString &type, const FString &message) noexcept;

        /**
         * Returns the exception's message
         */
        inline const FString &GetMessage() const
        {
            return (Message);
        }

        inline const char *GetType() const override
        {
            return (*Type);
        }
        virtual void Log(FLogger &logger) const override;
    };

    class ENGINE_API FIndexException : public FRuntimeException
    {
    private:
        int ID;
    public:
        inline FIndexException(const int id) noexcept
            : FRuntimeException("Index", ""), ID(id)
        {
        }
        void Log(FLogger &logger) const;
    };
}

#endif /* !RUNTIMEEXCEPTION_H_ */
