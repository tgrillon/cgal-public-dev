#ifndef marcutils
#define marcutils

#ifdef CGAL_CXX0X
#include <type_traits>
#include <utility>
#define CGAL_FORWARDABLE(T) T&&
#define CGAL_FORWARD(T,t) std::forward<T>(t)
#define CGAL_MOVE(t) std::move(t)
#define CGAL_CONSTEXPR constexpr
#else
#define CGAL_FORWARDABLE(T) T const&
#define CGAL_FORWARD(T,t) t
#define CGAL_MOVE(t) t
#define CGAL_CONSTEXPR
#endif
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <CGAL/Rational_traits.h>
#include <CGAL/tuple.h>

#ifdef CGAL_CXX0X
#define BOOSTD std::
#else
#define BOOSTD boost::
#endif

namespace CGAL {
namespace internal {
	BOOST_MPL_HAS_XXX_TRAIT_DEF(type)
}

	template <class T> struct Wrap_type { typedef T type; };

	// tell a function f(a,b,c) that its real argument is a(b,c)
	struct Eval_functor {};

	// forget the first argument. Useful to make something dependant
	// (and thus usable in SFINAE), although that's not a great design.
	template<class A,class B> struct Second_arg {
		typedef B type; 
	};      

	// like std::forward, except for basic types where it does a cast, to
	// avoid issues with narrowing conversions
#ifdef CGAL_CXX0X
	template<class T,class U,class V> inline
		typename std::conditional<std::is_arithmetic<T>::value&&std::is_arithmetic<typename std::remove_reference<U>::type>::value,T,U&&>::type
	       	forward_safe(V&& u) { return std::forward<U>(u); }
#else
	template<class T,class U> inline U const& forward_safe(U const& u) {
		return u;
	}
#endif

#ifdef CGAL_CXX0X
	template<class...> struct Constructible_from_each;
	template<class To,class From1,class...From> struct Constructible_from_each<To,From1,From...>{
		enum { value=std::is_convertible<From1,To>::value&&Constructible_from_each<To,From...>::value };
	};
	template<class To> struct Constructible_from_each<To>{
		enum { value=true };
	};
#else
// currently only used in C++0X code
#endif

	template<class T> struct Scale {
		T const& scale;
		Scale(T const& t):scale(t){}
		template<class FT>
#ifdef CGAL_CXX0X
		auto operator()(FT&& x)const->decltype(scale*std::forward<FT>(x))
#else
		FT operator()(FT const& x)const
#endif
		{
			return scale*CGAL_FORWARD(FT,x);
		}
	};
	template<class NT,class T> struct Divide {
#if !defined(CGAL_CXX0X) || !defined(BOOST_RESULT_OF_USE_DECLTYPE)
		// requires boost > 1.44
		// shouldn't be needed with C++0X
		//template<class> struct result;
		//template<class FT> struct result<Divide(FT)> {
		//	typedef FT type;
		//};
		typedef NT result_type;
#endif
		T const& scale;
		Divide(T const& t):scale(t){}
		template<class FT>
#ifdef CGAL_CXX0X
		//FIXME: gcc complains for Gmpq
		//auto operator()(FT&& x)const->decltype(Rational_traits<NT>().make_rational(std::forward<FT>(x),scale))
		NT operator()(FT&& x)const
#else
		NT operator()(FT const& x)const
#endif
		{
			return Rational_traits<NT>().
				make_rational(CGAL_FORWARD(FT,x),scale);
		}
	};

	template <class NT> struct has_cheap_constructor : boost::is_arithmetic<NT>{};
	template <bool p> struct has_cheap_constructor<Interval_nt<p> > {
		        enum { value=true };
	};

	// like std::multiplies but allows mixing types
	// in C++0x in doesn't need to be a template
	template < class Ret >
	struct multiplies {
		template<class A,class B>
#ifdef CGAL_CXX0X
		auto operator()(A&&a,B&&b)const->decltype(std::forward<A>(a)*std::forward<B>(b))
#else
		Ret operator()(A const& a, B const& b)const
#endif
		{
			return CGAL_FORWARD(A,a)*CGAL_FORWARD(B,b);
		}
	};
	template < class Ret >
	struct division {
		template<class A,class B>
#ifdef CGAL_CXX0X
		auto operator()(A&&a,B&&b)const->decltype(std::forward<A>(a)/std::forward<B>(b))
#else
		Ret operator()(A const& a, B const& b)const
#endif
		{
			return CGAL_FORWARD(A,a)/CGAL_FORWARD(B,b);
		}
	};

#ifdef CGAL_CXX0X
	using std::decay;
#else
	template<class T> struct decay : boost::remove_cv<typename boost::decay<T>::type> {};
#endif

	template<class T,class U> struct Type_copy_ref { typedef U type; };
	template<class T,class U> struct Type_copy_ref<T&,U> { typedef U& type; };
#ifdef CGAL_CXX0X
	template<class T,class U> struct Type_copy_ref<T&&,U> { typedef U&& type; };
#endif
	template<class T,class U> struct Type_copy_cv { typedef U type; };
	template<class T,class U> struct Type_copy_cv<T const,U> { typedef U const type; };
	template<class T,class U> struct Type_copy_cv<T volatile,U> { typedef U volatile type; };
	template<class T,class U> struct Type_copy_cv<T const volatile,U> { typedef U const volatile type; };

	template<class T,class U> struct Type_copy_cvref :
		Type_copy_ref<T,typename Type_copy_cv<typename boost::remove_reference<T>::type,U>::type> {};

	struct Dereference_functor {
		template<class> struct result{};
		template<class It> struct result<Dereference_functor(It)> {
			typedef typename std::iterator_traits<It>::reference type;
		};
		template<class It> typename result<Dereference_functor(It)>::type
			operator()(It const&i)const{
				return *i;
			}
	};

#ifdef CGAL_CXX0X
	template<int...> struct Indices{};
	template<class> struct Next_increasing_indices;
	template<int...I> struct Next_increasing_indices<Indices<I...> > {
		typedef Indices<I...,sizeof...(I)> type;
	};
	template<int N> struct N_increasing_indices {
		typedef typename Next_increasing_indices<typename N_increasing_indices<N-1>::type>::type type;
	};
	template<> struct N_increasing_indices<0> { typedef Indices<> type; };
	namespace internal {
	template<class F,class...U,int...I> inline typename std::result_of<F&&(U...)>::type
	do_call_on_tuple_elements(F&&f, std::tuple<U...>&&t, Indices<I...>&&) {
		return f(std::get<I>(std::move(t))...);
	}
	} // internal
	template<class/*result type, ignored*/,class F,class...U>
	inline typename std::result_of<F&&(U...)>::type
	call_on_tuple_elements(F&&f, std::tuple<U...>&&t) {
		return internal::do_call_on_tuple_elements(std::forward<F>(f),std::move(t),
				typename N_increasing_indices<sizeof...(U)>::type());
	}
#else
#define VAR(Z,N,_) cpp0x::get<N>(t)
#define CODE(Z,N,_) template<class Res, class F BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N,class U)> \
	inline Res call_on_tuple_elements(F const&f, \
			cpp0x::tuple<BOOST_PP_ENUM_PARAMS(N,U)> const&t) { \
		return f(BOOST_PP_ENUM(N,VAR,)); \
	}
BOOST_PP_REPEAT_FROM_TO(0, 8, CODE, _ )
#undef CODE
#undef VAR
#endif

	template<class A> struct Factory {
	  typedef A result_type;
#ifdef CGAL_CXX0X
	  template<class...U> result_type operator()(U&&...u)const{
	    return A(std::forward<U>(u)...);
	  }
#else
	  result_type operator()()const{
	    return A();
	  }
#define CODE(Z,N,_) template<BOOST_PP_ENUM_PARAMS(N,class U)> \
	  result_type operator()(BOOST_PP_ENUM_BINARY_PARAMS(N,U,const&u))const{ \
	    return A(BOOST_PP_ENUM_PARAMS(N,u)); \
	  }
BOOST_PP_REPEAT_FROM_TO(1, 8, CODE, _ )
#undef CODE
#endif
	};
}

#endif
