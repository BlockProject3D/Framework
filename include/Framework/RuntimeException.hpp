#ifndef RUNTIMEEXCEPTION_H_
#define RUNTIMEEXCEPTION_H_

namespace bpf
{
    class BPF_API RuntimeException : public Exception
    {
    private:
        String Type;
        String Message;

    public:
        virtual ~RuntimeException() {}
        RuntimeException(const String &type, const String &message) noexcept;

        /**
         * Returns the exception's message
         */
        inline const String &GetMessage() const
        {
            return (Message);
        }

        inline const char *GetType() const override
        {
            return (*Type);
        }
        //virtual void Log(Framework::FLogger &logger) const override;
    };
}

#endif /* !RUNTIMEEXCEPTION_H_ */
