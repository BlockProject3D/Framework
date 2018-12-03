#ifndef CLASS_H
# define CLASS_H

# include "framework/factory.h"

namespace Framework
{
    /**
     * Provides a template to an abstract factory for type T
     * @tparam T the parent type
     * @tparam Args the constructor arguments
     */
    template <class /* ? extends */ T, typename ...Args>
    class FClass
    {
    private:
        IFactory<T, Args...> *Factory;
    public:
        inline FClass()
            : Factory(Null)
        {
        }

        /**
         * Initializes a FClass using a given concrete type factory
         * @param factory the factory to store
         */
        inline FClass(IFactory<T, Args...> *factory)
            : Factory(factory)
        {
        }

        /**
         * Returns a new instance of a T using the stored factory, Null if failed
         * @param args arguments to pass to the constructor
         */
        inline T *NewInstance(Args... args)
        {
            if (Factory == NULL)
                return (NULL);
            return (Factory->Create(args...));
        }
    };
};

#endif /* !CLASS_H */
