#pragma once

#include "details/flat_tuple_details.hpp"

namespace pns
{

template <std::size_t I, typename T>
decltype(auto) get(T&& t)
{
    return std::forward<T>(t).get(FlatTupleDetails::Getter<I>{});
}

template <typename ...T>
using flat_tuple = typename FlatTupleDetails::Def<T...>::Type;

}



