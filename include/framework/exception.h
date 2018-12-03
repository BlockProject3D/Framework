#ifndef EXCEPTION_H_
# define EXCEPTION_H_

namespace Framework
{
    class FLogger;

    class ENGINE_API FException
    {
    private:
    public:
        virtual ~FException() {}
        FException() noexcept {}

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
        virtual void Log(FLogger &logger) const;
    };
};

#endif /* !EXCEPTION_H_ */
