#ifndef BPSCRIPT_H_
# define BPSCRIPT_H_

# include <map>

namespace Framework
{
    enum ENGINE_API EBPScriptType
    {
        BPSCRIPT_TYPE_NUMBER,
        BPSCRIPT_TYPE_BOOLEAN,
        BPSCRIPT_TYPE_STRING,
        BPSCRIPT_TYPE_COLOR,
        BPSCRIPT_TYPE_VECTOR
    };

    struct ENGINE_API FBPScriptArgument
    {
        FString Value;
        EBPScriptType Type;
        inline FBPScriptArgument(const FString &val, const EBPScriptType type)
            : Value(val), Type(type)
        {
        }
        float			AsNumber();
        bool			AsBoolean();
        FVector			AsVector();
        FColor			AsColor();
        inline const FString	&AsString() const
        {
            return (Value);
        }
    };

    class ENGINE_API IBPScriptHandler
    {
    public:
        virtual ~IBPScriptHandler() {}
        virtual bool FunctionExists(const FString &fname) const = 0;
        virtual bool PropertyExists(const FString &name) const = 0;
        virtual bool FunctionCall(const FString &fname, FList<FBPScriptArgument> &args, FString &errmsg) = 0;
    };

    class ENGINE_API FBPScript
    {
    private:
        std::map<FString, FDelegateFunction<bool, FList<FBPScriptArgument> &, FString &>> FuncReg;

        std::map<FString, FString> Defines;
        std::map<FString, FString> Properties;
        FList<FString> PropList;
        FString Class;
        FString Parent;
        IInputStream *Stream;
        bool Preprocessor(IInputStream *stream, FList<FString> &out);
        bool Compile(FList<FString> &in, FString &out, FString &errmsg);
        bool ParseProperty(const FString &keyvalstr, FString &errmsg);
        bool RunFunction(const FString &fcall, FString &errmsg);
        bool FuncExists(const FString &fname);
    public:
        //Events
        FDelegate<bool, const FString &> FunctionIsValidEvent;
        FDelegate<bool, const FString &> PropertyIsValidEvent;
        FDelegate<bool, const FString &, FList<FBPScriptArgument> &, FString &>	FunctionCallEvent;
        //End

        FBPScript(IInputStream *in);
        ~FBPScript() {}
        void AddFunction(const FString &fname, FDelegateFunction<bool, FList<FBPScriptArgument> &, FString &> func);
        void AddDefine(const FString &name, const FString &val);
        bool RunScript(FString &errmsg);
        EBPScriptType GetPropertyType(const FString &name);
        template <typename T>
        T GetProperty(const FString &name);
        int GetProperyCount() const;
        inline const FString &GetClass() const
        {
            return (Class);
        }
        inline const FString &GetParent() const
        {
            return (Parent);
        }

        inline FList<FString>::Iterator PropertyIterator()
        {
            return (PropList.Begin());
        }
    };
};

#endif /* !BPSCRIPT_H_ */
