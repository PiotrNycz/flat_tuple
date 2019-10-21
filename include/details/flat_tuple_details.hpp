#include <cstdint>
#include <utility>
#include <type_traits>

namespace pns::FlatTupleDetails
{

template <std::size_t I>
struct Getter {};
struct Nothing {};

template <typename T, std::size_t I>
struct ElemAggregate
{
    using element_type = T;
    element_type value{};

    ElemAggregate() = default;
    ElemAggregate(Nothing) : ElemAggregate() {}
    ElemAggregate(T& value) : value(value) {}
    ElemAggregate(T const& value) : value(value) {}
    ElemAggregate(T&& value) : value(std::move(value)) {}
    ElemAggregate(T const&& value) : value(std::move(value)) {}

    element_type& get(Getter<I>) & { return value; }
    const element_type& get(Getter<I>) const & { return value; }
    element_type&& get(Getter<I>) && { return std::move(value); }
    const element_type&& get(Getter<I>) const && { return std::move(value); }
};

template <typename T, std::size_t I>
struct ElemCompose : T
{
    using element_type = T;

    using element_type::element_type;
    ElemCompose() = default;
    ElemCompose(Nothing) : ElemCompose() {}
    ElemCompose(T& value) : element_type(value) {}
    ElemCompose(T const& value) : element_type(value) {}
    ElemCompose(T&& value) : element_type(std::move(value)) {}
    ElemCompose(T const&& value) : element_type(std::move(value)) {}

    element_type& get(Getter<I>) & { return *this; }
    const element_type& get(Getter<I>) const & { return *this; }
    element_type&& get(Getter<I>) && { return std::move(*this); }
    const element_type&& get(Getter<I>) const && { return std::move(*this); }
};

template <typename T, std::size_t I>
using Elem = std::conditional_t<std::is_class_v<T> or std::is_union_v<T>,
                               std::conditional_t<std::is_final_v<T>,
                                                  ElemAggregate<T, I>, ElemCompose<T, I>>,
                               ElemAggregate<T, I>>;

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


