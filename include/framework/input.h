#ifndef INPUT_H_
# define INPUT_H_

namespace Framework
{
    /**
     * Represents a key or a button
     * TODO : Remake Window manager API to remove the need of FKey as a structure and not a class
     */
    struct ENGINE_API FKey
    {
        const char *Name;
        int UniqueID;
        int State;
        FKey(const char *name, int uid);
        FKey();
        bool operator==(const FKey &other) const;
    };

    class ENGINE_API FInput
    {
    public:
        static FKey GetKeyFromCode(int code);
        static bool IsKeyDown(const FKey &key);
        static void SetKeyState(int code, int state);
    };

    /**
     * Key enumerations
     */
    namespace EKeys
    {
        static const Framework::FKey KEY_NULL = Framework::FKey("NULL", -1);

        static const Framework::FKey KEY_A = Framework::FKey("A", 65);
        static const Framework::FKey KEY_B = Framework::FKey("B", 66);
        static const Framework::FKey KEY_C = Framework::FKey("C", 67);
        static const Framework::FKey KEY_D = Framework::FKey("D", 68);
        static const Framework::FKey KEY_E = Framework::FKey("E", 69);
        static const Framework::FKey KEY_F = Framework::FKey("F", 70);
        static const Framework::FKey KEY_G = Framework::FKey("G", 71);
        static const Framework::FKey KEY_H = Framework::FKey("H", 72);
        static const Framework::FKey KEY_I = Framework::FKey("I", 73);
        static const Framework::FKey KEY_J = Framework::FKey("J", 74);
        static const Framework::FKey KEY_K = Framework::FKey("K", 75);
        static const Framework::FKey KEY_L = Framework::FKey("L", 76);
        static const Framework::FKey KEY_M = Framework::FKey("M", 77);
        static const Framework::FKey KEY_N = Framework::FKey("N", 78);
        static const Framework::FKey KEY_O = Framework::FKey("O", 79);
        static const Framework::FKey KEY_P = Framework::FKey("P", 80);
        static const Framework::FKey KEY_Q = Framework::FKey("Q", 81);
        static const Framework::FKey KEY_R = Framework::FKey("R", 82);
        static const Framework::FKey KEY_S = Framework::FKey("S", 83);
        static const Framework::FKey KEY_T = Framework::FKey("T", 84);
        static const Framework::FKey KEY_U = Framework::FKey("U", 85);
        static const Framework::FKey KEY_V = Framework::FKey("V", 86);
        static const Framework::FKey KEY_W = Framework::FKey("W", 87);
        static const Framework::FKey KEY_X = Framework::FKey("X", 88);
        static const Framework::FKey KEY_Y = Framework::FKey("Y", 89);
        static const Framework::FKey KEY_Z = Framework::FKey("Z", 90);
        static const Framework::FKey KEY_SPACE = Framework::FKey("SPACE", 32);
        static const Framework::FKey KEY_ENTER = Framework::FKey("ENTER", 257);

        static const Framework::FKey KEY_1 = Framework::FKey("1", 49);
        static const Framework::FKey KEY_2 = Framework::FKey("2", 50);
        static const Framework::FKey KEY_3 = Framework::FKey("3", 51);
        static const Framework::FKey KEY_4 = Framework::FKey("4", 52);
        static const Framework::FKey KEY_5 = Framework::FKey("5", 53);
        static const Framework::FKey KEY_6 = Framework::FKey("6", 54);
        static const Framework::FKey KEY_7 = Framework::FKey("7", 55);
        static const Framework::FKey KEY_8 = Framework::FKey("8", 56);
        static const Framework::FKey KEY_9 = Framework::FKey("9", 57);
        static const Framework::FKey KEY_0 = Framework::FKey("0", 48);
        static const Framework::FKey KEY_PLUS = Framework::FKey("PLUS", 61);
        static const Framework::FKey KEY_DEGREE = Framework::FKey("DEGREE", 93);

        static const Framework::FKey KEY_ESCAPE = Framework::FKey("ESCAPE", 256);
        static const Framework::FKey KEY_F1 = Framework::FKey("F1", 290);
        static const Framework::FKey KEY_F2 = Framework::FKey("F2", 291);
        static const Framework::FKey KEY_F3 = Framework::FKey("F3", 292);
        static const Framework::FKey KEY_F4 = Framework::FKey("F4", 293);
        static const Framework::FKey KEY_F5 = Framework::FKey("F5", 294);
        static const Framework::FKey KEY_F6 = Framework::FKey("F6", 295);
        static const Framework::FKey KEY_F7 = Framework::FKey("F7", 296);
        static const Framework::FKey KEY_F8 = Framework::FKey("F8", 297);
        static const Framework::FKey KEY_F9 = Framework::FKey("F9", 298);
        static const Framework::FKey KEY_F10 = Framework::FKey("F10", 299);
        static const Framework::FKey KEY_F11 = Framework::FKey("F11", 300);
        static const Framework::FKey KEY_F12 = Framework::FKey("F12", 301);

        static const Framework::FKey KEY_TAB = Framework::FKey("TAB", 258);
        static const Framework::FKey KEY_LSHIFT = Framework::FKey("LSHIFT", 340);
        static const Framework::FKey KEY_RSHIFT = Framework::FKey("RSHIFT", 344);
        static const Framework::FKey KEY_LCTRL = Framework::FKey("LCTRL", 341);
        static const Framework::FKey KEY_RCTRL = Framework::FKey("RCTRL", 345);
        static const Framework::FKey KEY_LALT = Framework::FKey("LALT", 342);
        static const Framework::FKey KEY_RALT = Framework::FKey("RALT", 346);
        static const Framework::FKey KEY_PAGEUP = Framework::FKey("PAGEUP", 266);
        static const Framework::FKey KEY_PAGEDOWN = Framework::FKey("PAGEDOWN", 267);
        static const Framework::FKey KEY_START = Framework::FKey("START", 268);
        static const Framework::FKey KEY_END = Framework::FKey("END", 269);
        static const Framework::FKey KEY_DELETE = Framework::FKey("DELETE", 261);
        static const Framework::FKey KEY_BACK = Framework::FKey("BACK", 259);

        static const Framework::FKey KEY_ARROW_LEFT = Framework::FKey("LEFTARROW", 263);
        static const Framework::FKey KEY_ARROW_RIGHT = Framework::FKey("RIGHTARROW", 262);
        static const Framework::FKey KEY_ARROW_UP = Framework::FKey("UPARROW", 265);
        static const Framework::FKey KEY_ARROW_DOWN = Framework::FKey("DOWNARROW", 264);

        static const Framework::FKey KEY_MOUSE_LEFT = Framework::FKey("MOUSELEFT", 0);
        static const Framework::FKey KEY_MOUSE_RIGHT = Framework::FKey("MOUSERIGHT", 1);
        static const Framework::FKey KEY_MOUSE_MIDDLE = Framework::FKey("MOUSEMIDDLE", 2);
    };
};

#endif /* !INPUT_H_ */
