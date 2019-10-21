#pragma once

#include "details/flat_tuple_details.hpp"

namespace pns
{

template <typename ...T>
using flat_tuple = typename FlatTupleDetails::Def<T...>::Type;

template <std::size_t I, typename T>
decltype(auto) get(FlatTupleDetails::Elem<T, I>& t)
{
    return t.get(FlatTupleDetails::Getter<I>{});
}

template <std::size_t I, typename T>
decltype(auto) get(FlatTupleDetails::Elem<T, I>&& t)
{
    return std::move(t).get(FlatTupleDetails::Getter<I>{});
}


template <std::size_t I, typename T>
decltype(auto) get(const FlatTupleDetails::Elem<T, I>& t)
{
    return t.get(FlatTupleDetails::Getter<I>{});
}

template <std::size_t I, typename T>
decltype(auto) get(const FlatTupleDetails::Elem<T, I>&& t)
{
    return std::move(t).get(FlatTupleDetails::Getter<I>{});
}


}



