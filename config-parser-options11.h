// Copyright (c) 2013,2014 Carlo Contavalli (ccontavalli@gmail.com).
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

#ifndef LIBYAARG_CONFIG_PARSER_OPTIONS11_H
# define LIBYAARG_CONFIG_PARSER_OPTIONS11_H

# include "config-parser.h"

# include <memory>
# include <sstream>
template<typename TYPE>
class OwnedPointerOption : public Option {
 public:
  OwnedPointerOption(CommandHolder* holder, int flags, const char* lname, const char* sname,
                const char* expected, const char* description)
    : Option(holder, flags, lname, sname, description),
      has_been_set_(false),
      expected_value_(expected) {
  }

  bool HasBeenSet() const override { return has_been_set_; }
  bool HasDefaultValue() const override { return ptr_ == NULL; } 

  const TYPE* Get() const { return ptr_.get(); } 
  TYPE* Get() { return ptr_.get(); }
  void Set(TYPE* value) { has_been_set_ = true; ptr_.reset(value); }

  const char* GetType() const override { return expected_value_; }
  bool GetAsString(string* value) const override {
    if (!value)
      return true;

    auto ptr = Get();
    if (!ptr) {
      value->assign("null");
    } else {
      std::stringstream stream;
      stream << *ptr;
      value->assign(stream.str());
    }
    return true;
  }

 private:
  bool has_been_set_;
  const char* expected_value_;

  unique_ptr<TYPE> ptr_;
};

# include <vector>

template<typename TYPE>
class PrefixFactory : public OwnedPointerOption<TYPE> {
 public:
  struct Callback {
    const char* prefix;
    std::function<TYPE*(const char* suffix, ostream* error, ostream* output)> callback;
  };

  PrefixFactory(CommandHolder* holder, int flags, const char* lname, const char* sname,
                const char* expected, const char* description, const std::vector<Callback>& callbacks)
    : OwnedPointerOption<TYPE>(holder, flags, lname, sname, expected, description),
      callbacks_(callbacks) {
  }

  //bool GetAsString(string* value) const final;
  bool Parse(const char* value, deque<const char*>* argv,
             ostream* error, ostream* output) final;

 private:
  const std::vector<Callback> callbacks_;
};

//// Implementations


template<typename TYPE>
inline bool PrefixFactory<TYPE>::Parse(
    const char* value, deque<const char*>* argv, ostream* error,
    ostream* output) {
  if (!value) {
    if (argv->empty()) {
      (*error) << "option requires a string as a value";
      return false;
    }
    value = (*argv)[0];
    argv->pop_front();
  }

  for (const auto& callback : callbacks_) {
    auto prefix = callback.prefix;
    auto cursor = value;
    //std::cerr << "COMPARING " << prefix << " " << cursor << std::endl;
    for (; *prefix && *prefix == *cursor; prefix++, cursor++)
      ;

    if (*prefix)
      continue;

    auto ptr = callback.callback(cursor, error, output);
    if (ptr) {
      this->Set(ptr);
      return true;
    }
    return false;
  }

  // FIXME: list the prefixes instead?
  (*error) << "string provided does not match any of the supported prefixes";
  return false;
}


#endif /* CONFIG_PARSER_OPTIONS11_H */
