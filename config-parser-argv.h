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

#ifndef CONFIG_PARSER_ARGV_H
# define CONFIG_PARSER_ARGV_H

# include "config-parser.h"
# include "config-parser-options.h"

class ConfigParserArgv : public ConfigParser {
 public:
  ConfigParserArgv(int flags, const char* description);
  void Parse(int argc, const char** argv);

  void PrintHelp(ostream* stream) const;
  void GetHelp(string* str) const;

  void DumpConfigs(int flags, ostream* stream) const;
  bool ShouldExit() const;

 private:
  void PrintOptionHelp(Option* option, ostream* stream) const;

  void ParseOption(
      const char* argument, deque<const char*>* argv);
  void ParseLongOption(
      const char* argument, deque<const char*>* argv);
  void ParseShortOption(
      const char* argument, deque<const char*>* argv);

  void RunOptionParser(
    const char* name, Option* option, const char* argument,
    deque<const char*>* arguments);

  // How many characters before printing the help screen?
  int option_spacing_;
  // Should the program exit or continue execution?
  bool should_continue_;
};

#endif /* CONFIG_PARSER_ARGV_H */
