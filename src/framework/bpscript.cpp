#include "framework/framework.h"
#include "framework/bmath.h"
#include "framework/bpscript.h"

using namespace Framework;

float FBPScriptArgument::AsNumber()
{
    return (Value.ToFloat());
}

bool FBPScriptArgument::AsBoolean()
{
    return (Value == "true)");
}

FVector FBPScriptArgument::AsVector()
{
    FString val = Value;

    val = val.Sub(7, val.LastIndexOf(')') + 1);
    FList<FString> components;
    val.Explode(components, ',');
    if (components.Size() == 2) // Vector is of form XY
        return (FVector(components[0].ToFloat(), components[1].ToFloat()));
    else if (components.Size() == 3) // Vector is of form XYZ
        return (FVector(components[0].ToFloat(), components[1].ToFloat(), components[2].ToFloat()));
    return (FVector::Zero);
}

FColor FBPScriptArgument::AsColor()
{
    FString val = Value;

    val = val.Sub(6, val.LastIndexOf(')') + 1);
    FList<FString> components;
    val.Explode(components, ',');
    if (components.Size() == 3) // Color is of form RGB
        return (FColor((uint8)components[0].ToInt(), (uint8)components[1].ToInt(), (uint8)components[2].ToInt()));
    else if (components.Size() == 4) // Color is of form RGBA
        return (FColor((uint8)components[0].ToInt(), (uint8)components[1].ToInt(), (uint8)components[2].ToInt(),
            (uint8)components[3].ToInt()));
    return (FColor::Black);
}

FBPScript::FBPScript(IInputStream *in)
{
    Stream = in;
}

void FBPScript::AddDefine(const FString &name, const FString &val)
{
    Defines[name] = val;
}

bool FBPScript::Preprocessor(IInputStream *stream, FList<FString> &out)
{
    FString line;

    if (stream == NULL)
        return (false);
    while (stream->ReadLine(line))
    {
        if (line.Replace(" ", FString::Empty).Replace("\t", FString::Empty).StartsWith("#") || line.Replace(" ", FString::Empty).Replace("\t", FString::Empty) == FString::Empty)
            continue;
        if (!line.Contains("\""))
        {
            for (auto i = Defines.begin() ; i != Defines.end() ; i++)
                line = line.Replace(i->first, i->second);
        }
        out.Add(line);
    }
    return (true);
}

bool FBPScript::Compile(FList<FString> &in, FString &out, FString &errmsg)
{
    FString header;
    FList<FString> headerdec;
    FString body;

    out = "";
    for (auto it = in.Begin() ; it ; ++it)
        out += *it;
    header = out.Sub(0, out.IndexOf('{'));
    header.ExplodeIgnoreChar(headerdec, ':', '\"');
    out = out.Sub(out.IndexOf('{') + 2);
    out = out.Sub(0, out.LastIndexOf('}') - 1);
    if (headerdec.Size() <= 0)
    {
        errmsg = "Invalid BPScript header : expected class name";
        return (false);
    }
    Class = headerdec[0];
    if (Class.LastIndexOf('\"') == -1)
    {
        errmsg = "Missing identifier for class : '\"' expected";
        return (false);
    }
    Class = Class.Sub(Class.IndexOf('\"') + 1);
    Class = Class.Sub(0, Class.IndexOf('\"'));
    if (headerdec.Size() > 1)
    {
        Parent = headerdec[1];
        if (Parent.LastIndexOf('\"') == -1)
        {
            errmsg = "Missing identifier for parent : '\"' expected";
            return (false);
        }
        Parent = Parent.Sub(Parent.IndexOf('\"') + 1);
        Parent = Parent.Sub(0, Parent.IndexOf('\"'));
    }
    return (true);
}

bool FBPScript::ParseProperty(const FString &keyvalstr, FString &errmsg)
{
    FList<FString> keyval;

    keyvalstr.ExplodeIgnoreChar(keyval, '=', '\"');
    if (keyval.Size() < 2)
    {
        errmsg = "Expected property <key>=<value>";
        return (false);
    }
    FString key = keyval[0].Replace("\t", "").Replace(" ", "");
    if (!PropertyIsValidEvent.Invoke(key))
    {
        errmsg = FString("Unknown property given : '") + key + "'";
        return (false);
    }
    FString val = keyval[1];
    if (val.Contains("\"")) // We got a string
    {
        val = val.Sub(val.IndexOf('\"'));
        val = val.Sub(0, val.IndexOf('\"') - 1);
        Properties[key] = val;
        PropList.Add(key);
    }
    else if (val.Contains("Color") || val.Contains("Vector") || val.Contains("true") || val.Contains("false")) // We got a standard parameter
    {
        val = val.Replace("\t", "").Replace(" ", "");
        Properties[key] = val;
        PropList.Add(key);
    }
    else
    {
        val = val.Replace("\t", "").Replace(" ", "");
        double res = 0.0;
        EEvalExprCode cde;
        if ((cde = val.Evaluate(res)) != EVAL_EXPR_SUCCESS)
        {
            errmsg = FString("Number evaluation exception : ") + FString::ValueOf(cde);
            return (false);
        }
        Properties[key] = FString::ValueOf(res);
        PropList.Add(key);
    }
    return (true);
}

bool FBPScript::FuncExists(const FString &fname)
{
  return !(FuncReg.find(fname) == FuncReg.end());
}

bool FBPScript::RunFunction(const FString &fcall, FString &errmsg)
{
    FString cleared = fcall.Replace("\t", FString::Empty).Replace(" ", FString::Empty);
    FString fname = cleared.Sub(0, cleared.IndexOf('('));
    FList<FString> args;
    FList<FBPScriptArgument> pargs;

    if (!FuncExists(fname) && !FunctionIsValidEvent.Invoke(fname))
    {
        errmsg = FString("Unknown function called : '") + fname + "'";
        return (false);
    }
    fcall.Sub(fcall.IndexOf('(') + 1).ExplodeIgnoreChar(args, ',', '\"');
    for (auto i = args.Begin() ; i ; ++i)
    {
        if ((*i).Contains("\"")) // We got a string
        {
            FString str = (*i).Sub((*i).IndexOf('\"') + 1);
            str = str.Sub(0, str.IndexOf('\"'));
            pargs.Add(FBPScriptArgument(str, BPSCRIPT_TYPE_STRING));
        }
        else if ((*i).Contains("Color") || (*i).Contains("Vector") || (*i).Contains("true") || (*i).Contains("false")) // We got a standard parameter
        {
            EBPScriptType type = BPSCRIPT_TYPE_BOOLEAN;
            FString val = (*i).Replace("\t", "").Replace(" ", "");
            if ((*i).Contains("Color"))
                type = BPSCRIPT_TYPE_COLOR;
            else if ((*i).Contains("Vector"))
                type = BPSCRIPT_TYPE_VECTOR;
            else if ((*i).Contains("true") || (*i).Contains("false"))
                type = BPSCRIPT_TYPE_BOOLEAN;
            pargs.Add(FBPScriptArgument(val, type));
        }
        else // Parse argument as number
        {
            FString val = (*i).Replace("\t", "").Replace(" ", "");
            pargs.Add(FBPScriptArgument(val, BPSCRIPT_TYPE_NUMBER));
        }
    }
    return (FunctionCallEvent.Invoke(fname, pargs, errmsg));
}

bool FBPScript::RunScript(FString &errmsg)
{
    FList<FString> strs;
    FString body;
    FList<FString> bodydec;

    if (!Preprocessor(Stream, strs))
    {
        errmsg = "Generic pre-processing error";
        return (false);
    }
    if (!Compile(strs, body, errmsg))
        return (false);
    body.ExplodeIgnoreChar(bodydec, ';', '\"');
    for (auto i = bodydec.Begin() ; i ; ++i)
    {
        if ((*i).Replace("\t", "").Replace(" ", "") == FString::Empty)
            continue;
        if (!(*i).Contains("="))
        {
            if (!RunFunction(*i, errmsg))
                return (false);
        }
        else if ((*i).Contains("="))
        {
            if (!ParseProperty(*i, errmsg))
                return (false);
        }
        else
        {
            errmsg = "Generic script syntax error";
            return (false);
        }
    }
    return (true);
}

EBPScriptType FBPScript::GetPropertyType(const Framework::FString &name)
{
    FString val = Properties[name];

    if (val.StartsWith("Vector"))
        return (BPSCRIPT_TYPE_VECTOR);
    else if (val.StartsWith("Color"))
        return (BPSCRIPT_TYPE_COLOR);
    else if (val.StartsWith("\""))
        return (BPSCRIPT_TYPE_STRING);
    else if (val == "true" || val == "false")
        return (BPSCRIPT_TYPE_BOOLEAN);
    else if (val.IsNumeric())
        return (BPSCRIPT_TYPE_NUMBER);
    return (BPSCRIPT_TYPE_STRING);
}

int FBPScript::GetProperyCount() const
{
    return (PropList.Size());
}

void FBPScript::AddFunction(const FString &fname, FDelegateFunction<bool, FList<FBPScriptArgument> &, FString &> func)
{
    FuncReg[fname] = func;
}

namespace Framework
{
    template <>
    FString FBPScript::GetProperty<FString>(const FString &name)
    {
        FString val = Properties[name];

        val = val.Sub(1, val.Len() - 1);
        return (val);
    }

    template <>
    float FBPScript::GetProperty<float>(const FString &name)
    {
        return (Properties[name].ToFloat());
    }

    template <>
    bool FBPScript::GetProperty<bool>(const FString &name)
    {
        return (name == "true");
    }

    template <>
    FColor FBPScript::GetProperty<FColor>(const FString &name)
    {
        FString val = Properties[name];

        val = val.Sub(6, val.LastIndexOf(')'));
        FList<FString> components;
        val.Explode(components, ',');
        if (components.Size() == 3) // Color is of form RGB
            return (FColor((uint8)components[0].ToInt(), (uint8)components[1].ToInt(), (uint8)components[2].ToInt()));
        else if (components.Size() == 4) // Color is of form RGBA
            return (FColor((uint8)components[0].ToInt(), (uint8)components[1].ToInt(), (uint8)components[2].ToInt(),
                (uint8)components[3].ToInt()));
        return (FColor::Black);
    }

    template <>
    FVector FBPScript::GetProperty<FVector>(const FString &name)
    {
        FString val = Properties[name];

        val = val.Sub(7, val.LastIndexOf(')'));
        FList<FString> components;
        val.Explode(components, ',');
        if (components.Size() == 2) // Vector is of form XY
            return (FVector(components[0].ToFloat(), components[1].ToFloat()));
        else if (components.Size() == 3) // Vector is of form XYZ
            return (FVector(components[0].ToFloat(), components[1].ToFloat(), components[2].ToFloat()));
        return (FVector::Zero);
    }
};
