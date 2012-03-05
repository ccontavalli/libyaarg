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

#include "config-parser-argv.h"
#include "utils.h"

#include <sstream>
#include <iomanip>

ConfigParserArgv::ConfigParserArgv(int flags, const char* description)
    : ConfigParser(flags, description),
      option_spacing_(12),
      should_continue_(true) {
}

void ConfigParserArgv::RunOptionParser(
    const char* name, Option* option, const char* argument,
    deque<const char*>* arguments) {
  ostringstream estream, ostream;
  bool should_continue
      = option->Parse(argument, arguments, &estream, &ostream);

  if (!estream.str().empty()) {
    string error("option ");
    error.append(name);
    error.append(": ");
    error.append(estream.str());
    AddError(error);
    should_continue = false;
  }
  if (!ostream.str().empty())
    AddMessage(ostream.str());

  should_continue_ = should_continue_ && should_continue;
}

void ConfigParserArgv::ParseLongOption(
    const char* argument, deque<const char*>* arguments) {
  const char* equal = strchr(argument, '=');
  const char* value = NULL;

  OptionMap::const_iterator it;
  string option("--");
  // If we have an equal sign...
  if (equal) {
    option.append(argument, equal - argument);
    it = long_options_.find(option.c_str() + 2);

    // Value follows the equal sign.
    value = equal + 1;
  } else {
    option.append(argument);
    it = long_options_.find(argument);
  }

  if (it == long_options_.end()) {
    string error("unknown option: ");
    error.append(option);
    AddError(error);
    return;
  }

  RunOptionParser(option.c_str(), it->second, value, arguments);
}

void ConfigParserArgv::ParseShortOption(
    const char* argument, deque<const char*>* arguments) {
  static deque<const char*> empty;
  char short_option[] = "-\0\0";
  for (const char* cursor = argument; *cursor; cursor++) {
    short_option[1] = *cursor;
    OptionMap::const_iterator it(short_options_.find(short_option + 1));
    if (it == short_options_.end()) {
      string error("unknown option: -");
      error.append(short_option);
      AddError(error);
      continue;
    }

    deque<const char*>* topass = &empty;
    if (!*(cursor + 1))
      topass = arguments;

    RunOptionParser(short_option, it->second, NULL, topass);
  }
}

void ConfigParserArgv::ParseOption(
    const char* argument, deque<const char*>* arguments) {
  if (*argument != '-') {
    string error("internal error: ");
    error.append(argument);
    error.append(" does not start with -");
    AddError(error);
    return;
  }

  if (*(argument+1) == '-')
    ParseLongOption(argument + 2, arguments);
  else
    ParseShortOption(argument + 1, arguments);
}

void ConfigParserArgv::Parse(int argc, const char** argv) {
  deque<const char*> arguments(argv + 1, argv + argc);
  deque<const char*> left;

  while (!arguments.empty()) {
    const char* argument(arguments.front());
    arguments.pop_front();

    if (*argument != '-') {
      left.push_back(argument);
      continue;
    }
    
    if (!strcmp(argument, "--")) {
      left.insert(left.end(), arguments.begin(), arguments.end());
      break;
    }

    ParseOption(argument, &arguments);
  }

  for (deque<const char*>::const_iterator it = left.begin();
       it != left.end(); ++it) {
    string message("unknown argument: ");
    message.append(*it);
    AddError(message);
  }
}

void ConfigParserArgv::GetHelp(string* str) const {
  // TODO
}

void ConfigParserArgv::PrintOptionHelp(
    Option* option, ostream* stream) const {
  if (option->GetShortName()) {
    (*stream) << "-" << option->GetShortName();
    if (option->GetLongName())
      (*stream) << ", ";
    else
      (*stream) << "  ";
  } else {
    (*stream) << "    ";
  }

  (*stream) << setiosflags(ios::left);
  if (option->GetLongName())
    (*stream) << "--" << setw(option_spacing_) << option->GetLongName();
  else
    (*stream) << "  " << setw(option_spacing_) << " ";

  (*stream) << " " << option->GetDescription();
  (*stream).flush();

  if (option->GetType())
    (*stream) << " (" << option->GetType() << ")" << endl;
  else
    (*stream) << endl;
}

void ConfigParserArgv::PrintHelp(ostream* stream) const {
  unordered_set<Option*> already_seen;

  (*stream) << GetDescription() << endl;
  for (OptionMap::const_iterator it = long_options_.begin();
       it != long_options_.end(); ++it) {
    if (!StlSetInsert(&already_seen, it->second))
      continue;
    PrintOptionHelp(it->second, stream);
  }

  for (OptionMap::const_iterator it = short_options_.begin();
       it != short_options_.end(); ++it) {
    if (!StlSetInsert(&already_seen, it->second))
      continue;
    PrintOptionHelp(it->second, stream);
  }
}

void ConfigParserArgv::DumpConfigs(int flags, ostream* stream) const {
  unordered_set<Option*> already_dumped;

  for (OptionMap::const_iterator it = long_options_.begin();
       it != long_options_.end(); ++it) {
    if (!StlSetInsert(&already_dumped, it->second))
      continue;

    string value;
    if (it->second->GetAsString(&value)) {
      (*stream) << "--" << it->first << "=" << value << endl;
    } else {
      (*stream) << "--" << it->first << endl;
    }
  }

  for (OptionMap::const_iterator it = short_options_.begin();
       it != short_options_.end(); ++it) {
    if (!StlSetInsert(&already_dumped, it->second))
      continue;

    string value;
    if (it->second->GetAsString(&value)) {
      (*stream) << "-" << it->first << " " << value << endl;
    } else {
      (*stream) << "-" << it->first << endl;
    }
  }
}

bool ConfigParserArgv::ShouldExit() const {
  return !should_continue_;
}

