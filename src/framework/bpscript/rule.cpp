#include "framework/framework.h"
#include "framework/bpscript/rule.h"
#include "framework/bpscript/bpscript.h"

using namespace Framework;
using namespace Framework::BPScript;

VariableParser::VariableParser(const FString &var, Script &src, uint32 line)
    : Src(src), Line(line)
{
    if (var[0] == '"') //We have a string
    {
        if (var[var.Len() - 1] != '"')
            Script::Throw(Src, line, "Unmatched '\"'");
        ParseString(var);
    }
    else if (var.StartsWith("Color(") || var.StartsWith("Vector("))
    {
        if (var[var.Len() - 1] != ')')
            Script::Throw(Src, line, "Unmatched '('");
        if (var.StartsWith("Color(")) //We have a color
            ParseColor(var);
        else //We have a vector
            ParseVector(var);
    }
    else if (var[0] == '{') //We have a list
    {
        if (var[var.Len() - 1] != '}')
            Script::Throw(Src, line, "Unmatched '{'");
        ParseList(var);
    }
    else //Must be a boolean or it's an error
        ParseBool(var);
}

void VariableParser::ParseBool(const FString &data)
{
    bool b;

    if (data == "True")
        b = true;
    else if (data == "False")
        b = false;
    else
        Script::Throw(Src, Line, FString("Unexpected token '") + data + "'");
}

void VariableParser::ParseList(const FString &data)
{
    FList<FString> tokens;
    VList lst(Src);

    data.Sub(1, data.Len() - 1).ExplodeIgnoreChar(tokens, ',', '"');
}

void VariableParser::ParseColor(const FString &data)
{
    FList<FString> vars;
    FColor col;

    data.Sub(7, data.Len() - 1).Explode(vars, ',');
    if (vars.Size() < 3)
        Script::Throw(Src, Line, "Too few arguments for Color");
    if (vars.Size() == 3)
        col = FColor(vars[0].ToInt(), vars[1].ToInt(), vars[2].ToInt());
    else if (vars.Size() == 4)
        col = FColor(vars[0].ToInt(), vars[1].ToInt(), vars[2].ToInt(), vars[3].ToInt());
    else
        Script::Throw(Src, Line, "Too many arguments for Color");
    VColor c(col);
    Pos = Src.WriteStaticMem(c);
}

void VariableParser::ParseVector(const FString &data)
{
    FList<FString> vars;
    FVector vec;

    data.Sub(8, data.Len() - 1).Explode(vars, ',');
    if (vars.Size() < 3)
        Script::Throw(Src, Line, "Too few arguments for Vector");
    if (vars.Size() == 3)
        vec = FVector(vars[0].ToFloat(), vars[1].ToFloat(), vars[2].ToFloat());
    else
        Script::Throw(Src, Line, "Too many arguments for Vector");
    VVector v(vec);
    Pos = Src.WriteStaticMem(v);
}

void VariableParser::ParseString(const FString &data)
{
    VString str(data.Sub(0, data.Len() - 1));
    Pos = Src.WriteStaticMem(str);
}

bool ParseRuleGlobal::Run(Script &cript, uint32 line, FString &data)
{
    FList<FString> lst;
    FString part = data.Sub(0, data.IndexOf(';'));
    part.ExplodeIgnoreChar(lst, '=', '"');
    if (part.Size() != 2)
        return (false);
    //script.AddInstruction(Instruction(BPSCRIPT_INST_PUSH));
    data = data.Sub(data.IndexOf(';'));
    return (true);
}

bool ParseRuleFunction::Run(Script &cript, uint32 line, FString &data)
{
    return (false);
}
