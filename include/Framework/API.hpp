#ifndef API_H_
# define API_H_

# ifdef __GNUC__
#  define DEPRECATED(func) func __attribute__((deprecated))
# elif defined(_MSC_VER)
#  define DEPRECATED(func) __declspec(deprecated) func
# endif

#define ENGINE_API BPF_API

#endif /* !API_H_ */
