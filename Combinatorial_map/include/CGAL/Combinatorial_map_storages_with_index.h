// Copyright (c) 2013 CNRS and LIRIS' Establishments (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL$
// $Id$
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Guillaume Damiand <guillaume.damiand@liris.cnrs.fr>
//
#ifndef CGAL_COMBINATORIAL_MAP_STORAGES_WITH_INDEX_H
#define CGAL_COMBINATORIAL_MAP_STORAGES_WITH_INDEX_HH 1

#include <CGAL/Compact_container_with_index_2.h>
#include <CGAL/Dart.h>
#include <CGAL/Handle_hash_function.h>
#include <bitset>

#include <boost/config.hpp>
#if defined(BOOST_GCC)
_Pragma("GCC diagnostic push")
_Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
#endif

namespace CGAL {

  namespace internal {
    template <typename M>
    struct Combinatorial_map_helper;

    template<typename Concurrent_tag, class T, class Alloc_>
    struct Container_type;
  }

  // Storage with combinatorial maps using index
  template<unsigned int d_, class Items_, class Alloc_, class Index_type_ >
  class Combinatorial_map_storage_2
  {
  public:
    typedef Combinatorial_map_storage_2<d_, Items_, Alloc_, Index_type_> Self;
    typedef CGAL::Tag_true Use_index;
    using Index_type=Index_type_;

    typedef internal::Combinatorial_map_helper<Self> Helper;

    typedef typename Items_::template Dart_wrapper<Self>  Dart_wrapper;
    typedef typename Dart_wrapper::Dart                   Dart;
    typedef typename Alloc_::template rebind<Dart>::other Dart_allocator;

    typedef Size_type size_type; // Type used as index.

    typedef Compact_container_with_index_2<Dart,Dart_allocator,
    Constant_size_policy_for_cc_with_size<1024>, size_type >
    Dart_container;

    typedef Items_ Items;
    typedef Alloc_ Alloc;

    template <typename T>
    struct Container_for_attributes : public
        Compact_container_with_index_2<T,
        typename Alloc_::template rebind<T>::other,
        Constant_size_policy_for_cc_with_size<1024>, size_type >
    {};

    /// Typedef for attributes
    typedef typename Dart_wrapper::Attributes Attributes;

    template<int i>
    struct Attribute_type: public Helper::template Attribute_type<i>
    {};
    template<int i>
    struct Attribute_handle: public Helper::template Attribute_handle<i>
    {};
    template<int i>
    struct Attribute_const_handle:
        public Helper::template Attribute_const_handle<i>
    {};
    template<int i>
    struct Attribute_range: public Helper::template Attribute_range<i>
    {};
    template<int i>
    struct Attribute_const_range:
        public Helper::template Attribute_const_range<i>
    {};

    /// Number of marks
    static const size_type NB_MARKS = 32;

    /// The dimension of the combinatorial map.
    static const unsigned int dimension = d_;

    // typedef unsigned int Dart_index;
    // typedef MyIndex<unsigned int> Dart_index;
    typedef typename Dart_container::Index Dart_index;

    // Definition of old types, for backward compatibility.
    typedef Dart_index Dart_handle;
    typedef Dart_index Dart_const_handle;

    /// Value of null handle (!= null_dart_handle !!)
    typedef size_type Null_handle_type;
    static const size_type null_handle;

    typedef Index_hash_function Hash_function;

    //**************************************************************************
    // Dart_range
    struct Dart_range
    {
      typedef typename Dart_container::iterator iterator;
      typedef typename Dart_container::const_iterator const_iterator;
      Dart_range(Self &amap) : mmap(amap)
      {}
      iterator begin()
      { iterator res=mmap.mdarts.begin(); ++res; return res; }
      iterator end() { return mmap.mdarts.end(); }
      const_iterator begin() const
      { const_iterator res=mmap.mdarts.begin(); ++res; return res; }
      const_iterator end() const { return mmap.mdarts.end(); }
      size_type size()
      { return mmap.mdarts.size()-1; }
      bool empty() const
      { return mmap.is_empty(); }
    private:
      Self & mmap;
    };
    typedef const Dart_range Dart_const_range;
    
    /// @return a Dart_range (range through all the darts of the map).
    Dart_range& darts()             { return mdarts_range;}
    Dart_const_range& darts() const { return mdarts_range; }
    //**************************************************************************
    
    Combinatorial_map_storage_2() : mdarts_range(*this)
    {}

    void init_storage()
    {
      // Allocate a dart for null_dart_handle
      null_dart_handle = mdarts.emplace();
    }

    void clear_storage()
    {}
    
    /** Test if the map is empty.
     *  @return true iff the map is empty.
     */
    bool is_empty() const
    { return  this->mdarts.size()==1; }

    /// @return the number of darts.
    size_type number_of_darts() const
    { return mdarts.size()-1; }

    /** Return if this dart is free for adimension.
     * @param dh a dart handle
     * @param i the dimension.
     * @return true iff dh is linked with NULL for \em adimension.
     */
    template<unsigned int i>
    bool is_free(Dart_const_handle dh) const
    {
      CGAL_assertion(i <= dimension);
      return mdarts[dh].mbeta[i]==null_dart_handle;
    }
    bool is_free(Dart_const_handle dh, unsigned int i) const
    {
      CGAL_assertion(i <= dimension);
      return mdarts[dh].mbeta[i]==null_dart_handle;
    }

    /// Set simultaneously all the marks of this dart to a given value.
    void set_dart_marks(Dart_const_handle ADart,
                        const std::bitset<NB_MARKS>& amarks) const
    {
      mdarts[ADart].set_marks(amarks);
    }
    /// Return all the marks of a dart.
    std::bitset<NB_MARKS> get_dart_marks(Dart_const_handle ADart) const
    {
      return mdarts[ADart].get_marks();
    }
    /// Return the mark value of dart a given mark number.
    bool get_dart_mark(Dart_const_handle ADart, int amark) const
    {
      return mdarts[ADart].get_mark(amark);
    }

    /// Set the mark of a given mark number to a given value.
    void set_dart_mark(Dart_const_handle ADart, int amark, bool avalue) const
    {
      mdarts[ADart].set_mark(amark, avalue);
    }

    /// Flip the mark of a given mark number to a given value.
    void flip_dart_mark(Dart_const_handle ADart, int amark) const
    {
      mdarts[ADart].flip_mark(amark);
    }

    // Access to beta maps
    Dart_handle get_beta(Dart_handle ADart, int B1)
    {
      CGAL_assertion(B1>=0 && B1<=dimension);
      return mdarts[ADart].mbeta[B1];
    }
    Dart_const_handle get_beta(Dart_const_handle ADart, int B1) const
    {
      CGAL_assertion(B1>=0 && B1<=dimension);
      return  mdarts[ADart].mbeta[B1];
    }
    template<int B1>
    Dart_handle get_beta(Dart_handle ADart)
    {
      CGAL_assertion(B1>=0 && B1<=dimension);
      return  mdarts[ADart].mbeta[B1];
    }
    template<int B1>
    Dart_const_handle get_beta(Dart_const_handle ADart) const
    {
      CGAL_assertion(B1>=0 && B1<=dimension);
      return  mdarts[ADart].mbeta[B1];
    }

    // return a handle on the i-attribute
    template<unsigned int i>
    typename Attribute_handle<i>::type attribute(Dart_handle ADart)
    {
      CGAL_static_assertion_msg(Helper::template Dimension_index<i>::value>=0,
                     "attribute<i> called but i-attributes are disabled.");
      return CGAL::cpp11::get<Helper::template Dimension_index<i>::value>
        (mdarts[ADart].mattribute_handles);
    }
    template<unsigned int i>
    typename Attribute_const_handle<i>::type
    attribute(Dart_const_handle ADart) const
    {
      CGAL_static_assertion_msg(Helper::template Dimension_index<i>::value>=0,
                     "attribute<i> called but i-attributes are disabled.");
      return CGAL::cpp11::get<Helper::template Dimension_index<i>::value>
        (mdarts[ADart].mattribute_handles);
    }

    template<unsigned int i>
    typename Attribute_type<i>::type&
    get_attribute(typename Attribute_handle<i>::type ah)
    {
      CGAL_assertion( ah!=null_handle );
      return CGAL::cpp11::get<Helper::template Dimension_index<i>::value>
        (mattribute_containers)[ah];
    }

    template<unsigned int i>
    const typename Attribute_type<i>::type&
    get_attribute(typename Attribute_const_handle<i>::type ah) const
    {
      CGAL_assertion( ah!=null_handle );
      return CGAL::cpp11::get<Helper::template Dimension_index<i>::value>
        (mattribute_containers)[ah];
    }

    Dart & get_dart(Dart_handle ah)
    {
      CGAL_assertion( ah!=null_handle );
      return mdarts[ah];
    }
    const Dart & get_dart(Dart_const_handle ah) const
    {
      CGAL_assertion( ah!=null_handle );
      return mdarts[ah];
    }

    // Get the attribute of a dart
    template<unsigned int i>
    typename Attribute_type<i>::type& get_attribute_of_dart(Dart_handle adart)
    {
      CGAL_assertion( adart!=null_handle );
      CGAL_assertion( attribute<i>(adart)!=null_handle );
      return get_attribute<i>(attribute<i>(adart));
    }
    template<unsigned int i>
    const typename Attribute_type<i>::type&
    get_attribute_of_dart(Dart_const_handle adart) const
    {
      CGAL_assertion( adart!=null_handle );
      CGAL_assertion( attribute<i>(adart)!=null_handle );
      return get_attribute<i>(attribute<i>(adart));
    }

    // Get the dart of the given attribute
    template<unsigned int i>
    Dart_handle dart_of_attribute(typename Attribute_handle<i>::type ah)
    {
      CGAL_assertion( ah!=null_handle );
      return CGAL::cpp11::get<Helper::template Dimension_index<i>::value>
        (mattribute_containers)[ah].dart();
    }
    template<unsigned int i>
    Dart_const_handle
    dart_of_attribute(typename Attribute_const_handle<i>::type ah) const
    {
      CGAL_assertion( ah!=null_handle );
      return CGAL::cpp11::get<Helper::template Dimension_index<i>::value>
        (mattribute_containers)[ah].dart();
    }

    // Set the dart of the given attribute
    template<unsigned int i>
    void set_dart_of_attribute(typename Attribute_handle<i>::type ah,
                               Dart_handle adart)
    {
      CGAL_assertion( ah!=null_handle );
       CGAL::cpp11::get<Helper::template Dimension_index<i>::value>
        (mattribute_containers)[ah].set_dart(adart);
    }

    // Get the information associated with a given dart
    Dart_info& info(Dart_handle adart)
    { return mdarts[adart].info(); }
    const Dart_info& info(Dart_const_handle adart) const
    { return mdarts[adart].info(); }

    // Get the info of the given attribute
    template<unsigned int i>
    typename Attribute_type<i>::type::Info &
    info_of_attribute(typename Attribute_handle<i>::type ah)
    {
      CGAL_assertion( ah!=null_handle );
      return CGAL::cpp11::get<Helper::template Dimension_index<i>::value>
        (mattribute_containers)[ah].info();
    }
    template<unsigned int i>
    const typename Attribute_type<i>::type::Info &
    info_of_attribute(typename Attribute_const_handle<i>::type ah) const
    {
      CGAL_assertion( ah!=null_handle );
      return CGAL::cpp11::get<Helper::template Dimension_index<i>::value>
        (mattribute_containers)[ah].info();
    }

    // Get the info of the i-cell attribute associated with the given dart
    template<unsigned int i>
    typename Attribute_type<i>::type::Info & info(Dart_handle adart)
    {
      CGAL_assertion( adart!=null_handle );
      CGAL_assertion( this->template attribute<i>(adart)!=null_handle );
      return info_of_attribute<i>(attribute<i>(adart));
    }
    template<unsigned int i>
    const typename Attribute_type<i>::type::Info &
    info(Dart_const_handle adart) const
    {
      CGAL_assertion( adart!=null_handle );
      CGAL_assertion( attribute<i>(adart)!=null_handle );
      return info_of_attribute<i>(attribute<i>(adart));
    }

    // Get the dart of the i-cell attribute associated with the given dart
    template<unsigned int i>
    Dart_handle dart(Dart_handle adart)
    {
      CGAL_assertion( adart!=null_handle );
      CGAL_assertion( attribute<i>(adart)!=null_handle );
      return dart_of_attribute<i>(attribute<i>(adart));
    }
    template<unsigned int i>
    Dart_const_handle dart(Dart_const_handle adart) const
    {
      CGAL_assertion( adart!=null_handle );
      CGAL_assertion( attribute<i>(adart)!=null_handle );
      return dart_of_attribute<i>(attribute<i>(adart));
    }

    // Debug function
    void display_dart(Dart_const_handle ADart) const
    { std::cout<<ADart; }

    template<unsigned int i>
    void display_attribute(typename Attribute_const_handle<i>::type ah) const
    { std::cout<<ah; }

  protected:
    // Set the handle on the i th attribute
    template<unsigned int i>
    void basic_set_dart_attribute(Dart_handle ADart,
                                  typename Attribute_handle<i>::type ah)
    {
      CGAL::cpp11::get<Helper::template Dimension_index<i>::value>
        (mdarts[ADart].mattribute_handles) = ah;
    }

    /** Link a dart with a given dart for a given dimension.
     * @param adart the dart to link.
     * @param adart2 the dart to link with.
     * @param i the dimension.
     */
    template<unsigned int i>
    void dart_link_beta(Dart_handle adart, Dart_handle adart2)
    {
      CGAL_assertion(i <= dimension);
      CGAL_assertion(adart!=null_dart_handle);
      mdarts[adart].mf[i] = adart2;
    }
    void dart_link_beta(Dart_handle adart, Dart_handle adart2, unsigned int i)
    {
      CGAL_assertion(i <= dimension);
      CGAL_assertion(adart!=null_dart_handle);
      mdarts[adart].mf[i] = adart2;
    }

    /** Unlink a dart for a given dimension.
     * @param adart a dart.
     * @param i the dimension.
     */
    template<unsigned int i>
    void dart_unlink_beta(Dart_handle adart)
    {
      CGAL_assertion(i <= dimension);
      mdarts[adart].mf[i] = null_dart_handle;
    }
    void dart_unlink_beta(Dart_handle adart, unsigned int i)
    {
      CGAL_assertion(i <= dimension);
      mdarts[adart].mf[i] = null_dart_handle;
    }

  public:
    /// Void dart. A dart d is i-free if beta_i(d)=null_dart_handle.
    static const Dart_index null_dart_handle; //=0;

  protected:
    /// Dart container.
    Dart_container mdarts;
    Dart_range mdarts_range;
    
    /// Tuple of attributes containers
    typename Helper::Attribute_containers mattribute_containers;
  };

  /// null_dart_handle
  template<unsigned int d_, class Items_, class Alloc_, class Size_type>
  const typename Combinatorial_map_storage_2<d_, Items_, Alloc_, Size_type>::Dart_index
  Combinatorial_map_storage_2<d_, Items_, Alloc_, Size_type>::null_dart_handle(0);

  /// null_handle
  template<unsigned int d_, class Items_, class Alloc_, class Size_type>
  const Size_type Combinatorial_map_storage_2<d_, Items_, Alloc_, Size_type>::null_handle((std::numeric_limits<size_type>::max)()/2);  

} // namespace CGAL

#if defined(BOOST_GCC)
 _Pragma("GCC diagnostic pop")
#endif

#endif // CGAL_COMBINATORIAL_MAP_STORAGES_WITH_INDEX_H //
// EOF //
