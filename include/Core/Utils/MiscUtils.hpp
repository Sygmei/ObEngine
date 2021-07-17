
#pragma once

namespace obe::Utils::Misc
{

/**
 * \brief A function object class to apply functions to a variant
 *
 * Example:
 *  constexpr Visitor example_visitor{
 *      [](double d) -> int {return d + 3.4;},
 *      [](int i) -> int {return i - 2;},
 *  };
 *  constexpr auto example_variant = std::variant<double,int>{9.0};
 *  constexpr auto example_result = std::visit(example_visitor, example_variant); //will be 12
 *
 */
template<typename... Base>
class Visitor : public Base...
{
public:
    using Base::operator()...;
};
template<typename... T>
Visitor(T...) -> Visitor<T...>;

} //namespace obe::Utils::Misc
