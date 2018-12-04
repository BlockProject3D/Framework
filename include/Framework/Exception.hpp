#ifndef EXCEPTION_H_
# define EXCEPTION_H_

namespace bpf
{
    class BPF_API Exception
    {
    private:
    public:
        virtual ~Exception() {}
        Exception() noexcept {}

        /**
         * Returns exception type
         */
        inline virtual const char *GetType() const
        {
            return ("GenericException");
        }

        /**
         * Logs the exception to the given logger
         */
        //virtual void Log(Framework::FLogger &logger) const;
    };
};

#endif /* !EXCEPTION_H_ */
