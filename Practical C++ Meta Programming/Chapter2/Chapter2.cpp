#include <iostream>
#include <type_traits>

template <typename ValueFunction, typename PointerFunction>
double test_1_value_ptrs(ValueFunction vf,
    PointerFunction pf,
    double param)
{
    double v = vf(param);
    pf(&v);

    return v;
}

double val_func(double param)
{
    return param;
}

void ptr_func(double* param)
{
    *param = *param * *param * 2.0;
}

//Type manipulation
using double_ptr_type = std::add_pointer<double>::type;
using double_type = std::remove_pointer<double*>::type;
using double_too_type = std::remove_pointer<double>::type;

template <typename F>
struct make_tuple_of_params;
template <typename Ret, typename... Args>
struct make_tuple_of_params<Ret(Args...)>
{
    using type = std::tuple<Args...>;
};

// Convenience function
template <typename F>
using make_tuple_of_params_t =
typename make_tuple_of_params<F>::type;

int main()
{
    //std::cout << "Double Val: " << Test_1_Value_Ptrs<decltype(ValFunc), decltype(PtrFunc)>(ValFunc, PtrFunc, 3.0) << std::endl;
}
