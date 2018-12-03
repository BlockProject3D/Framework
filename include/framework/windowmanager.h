#ifndef WINDOWMANAGER_H_
# define WINDOWMANAGER_H_

# define EVENT_MOUSE 0
# define EVENT_KEYBOARD 1
# define EVENT_CHAR 2
# define EVENT_WINDOWCLOSE 3
# define EVENT_WINDOWMOVE 4
# define EVENT_MOUSEWHEEL 5

namespace Framework
{
    class ENGINE_API FVideoMode
    {
    public:
        /**
         * Mode width
         */
        int Width;
        
        /**
         * Mode height
         */
        int Height;
        
        /**
         * Fullscreen
         */
        bool Fullscreen;

        inline FVideoMode()
            : Width(1024), Height(768), Fullscreen(false)
	{
	}
        inline FVideoMode(const int w, const int h, const bool f = false)
            : Width(w), Height(h), Fullscreen(f)
	{
	}
    };

    struct ENGINE_API FKeyEvent
    {
        int Type;
        FKey Key;
        bool Pressed;
    };

    struct ENGINE_API FWindowEvent
    {
        int Type;
        int WinX;
        int WinY;
    };

    struct ENGINE_API FCharEvent
    {
        int Type;
        uint32 Unicode;
    };

    struct ENGINE_API FMouseEvent
    {
        int Type;
        int MouseX;
        int MouseY;
        int DeltaX;
        int DeltaY;
    };

    struct ENGINE_API FMouseWheelEvent
    {
        int Type;
        int Delta;
    };

    union ENGINE_API FEvent
    {
        int Type;
        FKeyEvent KeyEvent;
        FCharEvent CharEvent;
        FMouseEvent MouseEvent;
        FWindowEvent WindowEvent;
        FMouseWheelEvent MouseWheelEvent;
        FEvent() {}
    };

    class ENGINE_API IWindow
    {
    public:
        virtual ~IWindow() {}
        virtual void SetTitle(const char *newtitle) = 0;
        virtual void EnterFullscreen() = 0;
        virtual void ExitFullscreen() = 0;
        virtual void ChangeVideoMode(const FVideoMode &newmode) = 0;
        virtual FVideoMode GetVideoMode() = 0;
        virtual bool PollNextEvent(FEvent &ev) = 0;
        virtual void HideCursor(const bool b) = 0;
        virtual void UpdateDisplay() = 0;
        virtual void SetMouseGrabbed(const bool b) = 0;
    };

    class ENGINE_API IWindowManager
    {
    public:
        virtual ~IWindowManager() {}
        virtual IWindow *CreateWindow(const char *name, const FVideoMode &mode) = 0;
        virtual void DestroyWindow(IWindow *w) = 0;
        virtual void GetAvailableVideoModes(FList<FVideoMode> &modes) = 0;
        virtual void SetMousePos(const int x, const int y) = 0;
        virtual void GetMousePos(int &x, int &y) = 0;
        virtual void GetScreenRes(int &w, int &h) = 0;
        virtual void SetVSync(const bool b) = 0;
    };
}

#endif /* !WINDOWMANAGER_H_ */
