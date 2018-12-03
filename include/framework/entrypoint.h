#ifndef ENTRYPOINT_H_
# define ENTRYPOINT_H_

# ifdef WINDOWS

# else

#  define IMPLEMENT_ENTRY_POINT() \
int main(int ac, char **av) \
{ \
    Framework::FArray<Framework::FString> arr(ac); \
    \
    for (int i = 0 ; i < ac ; i++) \
        arr[i] = av[i]; \
    Framework::FPlatform::Initialize(); \
    Framework::FPaths::SetGameRoot(arr[0]); \
    Framework::FPaths::SetModuleRoot(Framework::FPaths::GameRoot() + "Modules/"); \
    int res = Main(arr); \
    Framework::FPlatform::Shutdown(); \
    return (res); \
} \

# endif

#endif /* !ENTRYPOINT_H_ */
