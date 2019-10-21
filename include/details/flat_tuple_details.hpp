
#pragma once

#include <cstdint>
#include <utility>
#include <type_traits>

namespace pns::FlatTupleDetails
{

template <std::size_t I>
struct Getter {};
struct Nothing {};

template <typename T>
struct Aggregate
{
    using element_type = T;
    element_type value{};

    element_type& get() & { return value; }
    const element_type& get() const & { return value; }
    element_type&& get() && { return std::move(value); }
    const element_type&& get() const && { return std::move(value); }
};

template <typename T>
struct Compose : T
{
    using element_type = T;

    using element_type::element_type;
    Compose() = default;
    Compose(T& value) : element_type(value) {}
    Compose(T const& value) : element_type(value) {}
    Compose(T&& value) : element_type(std::move(value)) {}
    Compose(T const&& value) : element_type(std::move(value)) {}

    element_type& get() & { return *this; }
    const element_type& get() const & { return *this; }
    element_type&& get() && { return std::move(*this); }
    const element_type&& get() const && { return std::move(*this); }
};

template <typename T>
using Value = std::conditional_t<std::is_empty_v<T> and not std::is_final_v<T>,
                                 Compose<T>, Aggregate<T>>;


template <typename T, std::size_t I>
struct Elem : Value<T>
{
    using element_type = T;
    using Base = Value<T>;

    Elem() = default;
    Elem(Nothing) : Elem() {}
    Elem(T& value) : Base{value} {}
    Elem(T const& value) : Base{value} {}
    Elem(T&& value) : Base{std::move(value)} {}
    Elem(T const&& value) : Base{std::move(value)} {}

    using Base::get;

    element_type& get(Getter<I>) & { return get(); }
    const element_type& get(Getter<I>) const & { return get(); }
    element_type&& get(Getter<I>) && { return std::move(this).get(); }
    const element_type&& get(Getter<I>) const && { return std::move(this).get(); }
};


template <std::size_t I>
struct PickOne;

template <>
struct PickOne<0>
{
    template <typename F, typename ...T>
    static decltype(auto) get(F&& f, T&&...) {
        return std::forward<F>(f);
    }
    static Nothing get() {
        return {};
    }
};

template <std::size_t I>
struct PickOne
{
    template <typename F, typename ...T>
    static decltype(auto) get(F&& f, T&& ...t) {
        return PickOne<I - 1u>::get(std::forward<T>(t)...);
    }
    static Nothing get() {
        return {};
    }
};

template <std::size_t I, typename ...T>
constexpr decltype(auto) pickOne(T&& ...t)
{
    return PickOne<I>::get(std::forward<T>(t)...);
}


template <typename ...T>
struct Def {

    template <std::size_t ...I>
    struct Impl : Elem<T, I>... {
        template <typename ...U>
        Impl(U&& ...u) : Elem<T, I>{pickOne<I>(std::forward<U>(u)...)}... {}
        using Elem<T, I>::get...;
    };
    template <std::size_t ...I>
    static Impl<I...> get(std::integer_sequence<std::size_t, I...>);

    using Type = decltype(get(std::index_sequence_for<T...>{}));
};

}


