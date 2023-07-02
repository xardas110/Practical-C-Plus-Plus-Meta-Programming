//Template metaprogramming refresher

#include <iostream>
#include <string>
#include <chrono>
#include <type_traits>

template <typename T>
struct Echo
{
    using type = T;
};

template < >
struct Echo<int> { using type = unsigned int; };

template <int X>
struct IntIdentity
{
    static constexpr int value = X;
};

template <typename T, T Value>
struct ValueIdentity14 //c++ 14
{
    static constexpr int value = Value;
};

template <auto X> //c++ 17
struct ValueIdentity17
{
    static constexpr int value = X;
};

template <auto X> //c++ 17
inline constexpr auto ValueIdentity17_v = ValueIdentity17<X>::value;

template <typename T>
struct TypeIdentity
{
    using type = T;
};

template <typename T>
using TypeIdentity_t = typename TypeIdentity<T>::type;

int Sum(int x, int y)
{
    return x + y;
}

template<int X, int Y>
struct Sum17 //C++ 17
{
    static constexpr int value = X + Y;
    //inline static int value = X + Y;
};

template<auto X, auto Y>
struct Sum20 //C++ 20
{
    static constexpr auto value = X + Y; 
};

template <typename X, typename Y>
constexpr auto SumConstexpr(X x, Y y)
{
    return x + y;
}

template<class T, T v> //c++ 14
struct integral_constant
{
    static constexpr T value = v;

    using value_type = T;
    using type = integral_constant<T, v>;

    constexpr operator value_type() const noexcept
    {
        return value;
    }

    constexpr value_type operator()() const noexcept
    {
        return value;
    }
};

//primary type trait
template <typename T>
struct is_void : std::false_type {  };

template <>
struct is_void<void> : std::true_type {  };

template <>
struct is_void<const void> : std::true_type {  };

template <>
struct is_void<volatile void> : std::true_type {  };

template <>
struct is_void<volatile const void> : std::true_type {  };

//Transformation traits
//Do nothing, when there is no const
template<typename T> 
struct remove_const : TypeIdentity<T> {};

// Partial specialization, when const is detected
template<typename T> 
struct remove_const<T const> : TypeIdentity<T> {};

template<typename T> 
using remove_const_t = typename remove_const<T>::type;

template<typename T>
struct remove_volatile : TypeIdentity<T> {};

template<typename T>
struct remove_volatile<T volatile> : TypeIdentity<T> {};

template<typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

template<typename T>
using remove_cv = remove_const<remove_volatile_t<T>>;

template<typename T>
using remove_cv_t = typename remove_cv<T>::type;

//Conditional
template<bool Condition, typename T, typename F>
struct condidional : TypeIdentity<T> {};

template<typename T, typename F>
struct condidional<false, T, F> : TypeIdentity<F> {};

template<typename T1, typename T2>
struct is_same : std::false_type{};

template<typename T>
struct is_same<T, T> : std::true_type {};

template<typename T1, typename T2>
constexpr bool is_same_v = is_same<T1, T2>::value;

//Good use case for metaprogramming
unsigned int factorial(unsigned int n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

template <unsigned int N>
struct Factorial {
    static const unsigned int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static const unsigned int value = 1;
};

template <typename T>
bool isInteger(T value) {
    return typeid(value) == typeid(int);
}

template <typename T>
struct IsInteger {
    static constexpr bool value = std::is_same_v<T, int>;
};

int main()
{
    //Echo<int>::type val = 3;

    //std::cout << ValueIdentity14<int, 52>::value; //c++ 14
    //std::cout << ValueIdentity17<52>::value; //c++ 17
    //std::cout << ValueIdentity17_v<60>; //c++ 17
    //std::cout << Sum17<10, 10>::value << std::endl;
    //std::cout << SumConstexpr(20.f, 10.f) << std::endl;
    //std::cout << SumConstexpr(20, 10) << std::endl;

    //TypeIdentity<int>::type val = 3;
    //integral_constant<int, 3> test;
    //std::cout << test << std::endl;

    //std::cout << is_void<int>() << std::endl;
    //static_assert(is_void<volatile void>(), "Not a void");
    //static_assert(not is_void<int>(), "Not a int");

    //static_assert(is_same<int, unsigned>{});      -> fails
    //static_assert(is_same<int, int>{});           -> success

    /*
        const auto start = std::chrono::steady_clock::now();

        //runtime recursion 0.0003 sec
        //std::cout << "Factorial total value using runtime recursion: " << factorial(100) << std::endl;

        //metaprogramming recursion 0.0003 sec
        std::cout << "Factorial total value using metaprogramming: " << Factorial<100>::value << std::endl;

        const auto end = std::chrono::steady_clock::now();

        const std::chrono::duration<double> elapsed_seconds = end - start;

        std::cout << elapsed_seconds.count() << '\n'; // C++20: operator<< chrono::duration
    */
}
