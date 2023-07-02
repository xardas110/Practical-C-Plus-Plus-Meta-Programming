#include <iostream>
#include <string>

template<class T>
void Test_1_Check_Integer(T val)
{
    static_assert(std::is_same<decltype(val),
        std::uint32_t>::value,
        "invalid integer detected!");
}

template<class T>
void Test_2_Is_Ptr(T ptr)
{
    static_assert(sizeof(void*) == sizeof(ptr), "expected 64-bit platform");
}

/*
The macros will silently set the wrong value if you have a typo in the
macro value, if you forget a header, or if an exotic platform on
which you compile doesn’t have the value properly defined.
Also, it is often very difficult to come up with good macros to detect
the correct platform (although Boost.Predef has now greatly
reduced the complexity of the task)
*/
void Test_3_Macro_Vs_Meta()
{
#ifdef IS_MY_PLATFORM_64
    static const std::uint64_t default_buffer_size
        = 100 * 1024 * 1024;
#else
    static const std::uint64_t default_buffer_size
        = 1024 * 1024 * 1024;
#endif

    static const std::uint64_t default_buffer_size1 =
        std::conditional<sizeof(void*) == 8,
        std::integral_constant<std::uint64_t, 100 * 1024 * 1024>, // 100 mb for 32-bit platforms
        std::integral_constant<std::uint64_t, 1024 * 1024 * 1024> // 1000mb for 64-bit
        >::type::value;
}

void Test_4_Conditional()
{
    using Type1 = std::conditional<true, int, double>::type;
    using Type2 = std::conditional<false, int, double>::type;
    using Type3 = std::conditional<sizeof(int) >= sizeof(double), int, double>::type;

    std::cout << typeid(Type1).name() << '\n';
    std::cout << typeid(Type2).name() << '\n';
    std::cout << typeid(Type3).name() << '\n';
}

constexpr int Test_5(int a) { return a + 1; }

static int call_count = 3; 
constexpr int Test_6(int a)
{
   return a + (call_count++); //not meta due to side effect rule
}

template < typename T>
struct Fun_ { using type = T; };

template < > // Specialized cases
struct Fun_<int> { using type = unsigned int; };

template < >
struct Fun_<long> { using type = unsigned long; };

template < >
struct Fun_<float> { using type = double; };

template < typename T>
using Fun = typename Fun_<T>::type;

int main()
{

    // Test_1_Check_Integer(1U);        -> Success 
    // Test_1_Check_Integer(1);         -> Fails 

    //Test_2_Is_Ptr(3);                 -> Fails
    //Test_2_Is_Ptr(3ULL);              -> Success
    //Test_2_Is_Ptr((void*)3);          -> Success

    //Test_3_Macro_Vs_Meta();

    //Test_4_Conditional();

    //std::cout << (Test_5(3)) << std::endl;

    //Fun_<int>::type h = 3; //Type int -> unsigned
    //Fun_<long>::type h1 = 3; // Type long -> unsigned long
    //Fun_<float>::type s1 = 3.f; // Type float -> double

    //Fun<int> h = 3;
}
