// Copyright (c) 2008,2009,2010,2011 Carlo Contavalli (ccontavalli@gmail.com).
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//    1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
//    2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY Carlo Contavalli ''AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL Carlo Contavalli OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// The views and conclusions contained in the software and documentation are
// those of the authors and should not be interpreted as representing official
// policies, either expressed or implied, of Carlo Contavalli.

#ifndef UTILS_H
# define UTILS_H

# include "base.h"
# include <string.h>
# include <iostream>

// Useful if you need to create any sort of map or set using C strings
// as keys, eg, compares the string value rather than the pointer address
// when const char* is used as a key.
struct CStringCmpFunctor {
  bool operator()(char const *a, char const *b) const {
    bool result = (!a || !b) ?  (!a && b) : strcmp(a, b) < 0;
    return result;
  }
};

template<typename MAP>
typename MAP::value_type::second_type
StlMapGet(const MAP& map, const typename MAP::key_type& key) {
  typename MAP::const_iterator it(map.find(key));
  if (it == map.end())
    return NULL;
  return it->second;
}

template<typename SET>
bool StlSetInsert(SET* set, const typename SET::value_type& value) {
  return set->insert(value).second;
}

template<typename SET>
bool StlSetHas(const SET& set, const typename SET::key_type& key) {
  typename SET::const_iterator it(set.find(key));
  if (it == set.end())
    return false;
  return true;
}

template<typename HOLDER>
inline void StlDeleteElements(HOLDER* holder) {
  for (typename HOLDER::iterator it(holder->begin());
       it != holder->end(); ++it) {
    delete *it;
  }
}

template<typename HOLDER>
class StlAutoDeleteElements {
 public:
  explicit StlAutoDeleteElements(HOLDER* holder)
     : holder_(holder) {
  }

  ~StlAutoDeleteElements() {
    StlDeleteElements(holder_);
  }

 private:
  HOLDER* holder_;
};

// FIXME: this is ugly.
#ifdef __GXX_EXPERIMENTAL_CXX0X__
# define AUTO_DELETE_ELEMENTS(holder) StlAutoDeleteElements<decltype(holder)> __auto_delete_##holder(&(holder))
#else
# define AUTO_DELETE_ELEMENTS(holder) StlAutoDeleteElements<typeof(holder)> __auto_delete_##holder(&(holder))
#endif

#endif /* UTILS_H */
