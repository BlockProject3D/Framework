#ifndef FACTORY_H_
# define FACTORY_H_

namespace Framework
{
    /**
     * Constructor abstraction
     * @tparam T the type of parent to generate
     * @tparam Args the arguments to the constructor
     */
    template <class /* ? extends */ T, typename ...Args>
    class ENGINE_API IConstructor
    {
    public:
        virtual ~IConstructor() {}

        /**
         * Creates a new instance of the given type
         * @param args arguments to the constructor
         */
        virtual T *NewInstance(Args... args) = 0;
    };
    
    template <class /* ? extends */ T, typename ...Args>
    class ENGINE_API FFactory
    {
    private:
        bpf::Map<FString, IConstructor<T, Args...> *> Registry;
    public:
        inline void AddClass(const FString &name, IConstructor<T, Args...> *c)
        {
            Registry[name] = c;
        }
        inline T *New(const FString &name, Args&&... args)
        {
            if (!Registry.HasKey(name))
                return (Null);
            return (Registry[name]->NewInstance(std::forward<Args>(args)...));
        }
    };
};

#endif /* !FACTORY_H_ */
