#include "framework/framework.h"
#include "framework/input.h"

using namespace Framework;

Framework::FKey	*KeyCodes[512] = {};

FKey FInput::GetKeyFromCode(int code)
{
    if (code < 0 || code > 511 || KeyCodes[code] == NULL)
        return (EKeys::KEY_NULL);
    return (*KeyCodes[code]);
}

bool FInput::IsKeyDown(const FKey &key)
{
    int code = key.UniqueID;

    if (code < 0 || code > 511 || KeyCodes[code] == NULL)
        return (false);
    return (KeyCodes[code]->State == 1);
}

void FInput::SetKeyState(int code, int state)
{
    if (code < 0 || code > 511 || KeyCodes[code] == NULL)
        return;
    KeyCodes[code]->State = state;
}

FKey::FKey(const char *name, int uid)
{
    Name = name;
    UniqueID = uid;
    State = 0;
    if (UniqueID >= 0 && UniqueID < 512)
        KeyCodes[UniqueID] = this;
}

FKey::FKey()
{
    Name = "NULL";
    UniqueID = -1;
}

bool FKey::operator==(const FKey &other) const
{
    return (UniqueID == other.UniqueID);
}
