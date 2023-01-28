#pragma once

#include "defs.h"
#include "helper.tcc"

//! \defgroup tuple
//! \defgroup object
//! \defgroup tuplehelper


/*! \ingroup tuple
 * Empty Tuple.
 */
template <class... Membs>
struct Tuple {};

/*! \ingroup tuple
 * Non-empty Tuple.
 */
template <class H, class... Tail>
struct Tuple<H, Tail...> {
  H head {};               //!< First element.
  Tuple<Tail...> tail {};  //!< Remaining elements.
};


/*!
 * Access the type of the *k*-th element in a Tuple.
 *
 * https://eli.thegreenplace.net/2014/variadic-templates-in-c/#id5
 */
template <size_t, class>
struct ElemTypeHolder_;

//! \copydoc ElemTypeHolder_
template <class H, class... Tail>
struct ElemTypeHolder_<0, Tuple<H, Tail...> > {
  typedef H type;
};

//! \copydoc ElemTypeHolder_
template <size_t k, class H, class... Tail>
struct ElemTypeHolder_<k, Tuple<H, Tail...> > {
  typedef typename ElemTypeHolder_<k - 1, Tuple<Tail...> >::type type;
};


/*! \ingroup tuplehelper
 * Get the *k*-th element of a Tuple or Object.
 *
 * This can be used for both retrieval as well as assignment.
 *
 * \param t A Tuple.
 *
 * \return Reference to the *k*-th element in `t`.
 */
template <size_t k, class... Membs>
//! \cond
typename enableIf<
    k == 0, typename ElemTypeHolder_<0, Tuple<Membs...> >::type&>::type
//! \endcond
    get(Tuple<Membs...>& t) {
  return t.head;
}

template <size_t k, class... Membs>
//! \cond
typename enableIf<
    k != 0, typename ElemTypeHolder_<k, Tuple<Membs...> >::type&>::type
//! \endcond
    get(Tuple<Membs...>& t) {
  return get<k - 1>(t.tail);
}


//! Recursion terminator for `fill()`.
inline void fill(Tuple<>) {}

/*!
 * Fill a Tuple.
 *
 * \param t Tuple.
 * \param head First element.
 * \param tail Remaining elements.
 */
template <class H, class... Tail>
void fill(Tuple<H, Tail...>& t, H head, Tail... tail) {
  t.head = head;
  fill(t.tail, tail...);
}


/*! \ingroup tuplehelper
 * Make a Tuple from a parameter pack.
 *
 * \param args Values to store in a Tuple.
 *
 * \return Tuple containing `args`.
 */
template <class... Args>
Tuple<Args...> pack(Args... args) {
  Tuple<Args...> t;
  fill(t, args...);

  return t;
}


/*! \ingroup object
 * Object.
 */
template <class... Membs>
struct Object : Tuple<Membs...> {
  /*
   * Preferably this would have been an alias, but this is not supported in
   * C++11.
   */
  Object() {}
  Object(Membs... args) : Tuple<Membs...>() {
    fill(*dynamic_cast<Tuple<Membs...>*>(this), args...);
  }
};
