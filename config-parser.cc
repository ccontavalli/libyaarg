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

#include "config-parser.h"

CommandHolder::CommandHolder() : name_("main"), found_command_(NULL) {
}

CommandHolder::CommandHolder(const char* name) : name_(name), found_command_(NULL) {
}

void CommandHolder::RegisterOptionByLongName(const char* name, Option* option) {
  // FIXME: detect duplicate options and be angry.
  long_options_[name] = option;
}

void CommandHolder::RegisterOptionByShortName(const char* name, Option* option) {
  // FIXME: detect duplicate options and be angry.
  short_options_[name] = option;
}

void CommandHolder::RegisterCommand(const char* name, Command* command) {
  // FIXME: detect duplicate options and be angry.
  commands_[name] = command;
}

void CommandHolder::AddMessage(const string& message) {
  messages_.push_back(message);
}

bool CommandHolder::HasMessages() const {
  return !messages_.empty();
}

void CommandHolder::FlushMessages() {
  messages_.clear();
}

void CommandHolder::SetFoundCommand(Command* command) {
  found_command_ = command;
}

Command* CommandHolder::GetFoundCommand() const {
  return found_command_;
}

bool CommandHolder::PrintMessages(ostream* estr) const {
  if (!HasMessages())
    return false;

  for (list<string>::const_iterator it = messages_.begin();
       it != messages_.end(); ++it) {
    (*estr) << *it << '\n';
  }

  return true;
}

bool CommandHolder::GetMessages(string* output) const {
  if (!HasMessages())
    return false;

  for (list<string>::const_iterator it = messages_.begin();
       it != messages_.end(); ++it) {
    output->append(*it);
    output->append("\n");
  }

  return true;
}

void CommandHolder::AddError(const string& error) {
  errors_.push_back(error);
}

bool CommandHolder::HasErrors() const {
  return !errors_.empty();
}

void CommandHolder::FlushErrors() {
  errors_.clear();
}

bool CommandHolder::PrintErrors(ostream* estr) const {
  if (!HasErrors())
    return false;

  for (list<string>::const_iterator it = errors_.begin();
       it != errors_.end(); ++it) {
    (*estr) << *it << '\n';
  }

  return true;
}

bool CommandHolder::GetErrors(string* output) const {
  if (!HasErrors())
    return false;

  for (list<string>::const_iterator it = errors_.begin();
       it != errors_.end(); ++it) {
    output->append(*it);
    output->append("\n");
  }

  return true;
}

ConfigParser::ConfigParser(int flags, const char* description)
    : description_(description) {
}

Option::Option(
    CommandHolder* parser, int flags, const char* lname,
    const char* sname, const char* description)
    : long_name_(lname), short_name_(sname), description_(description) {
  if (!parser)
    return;

  parser->RegisterOptionByLongName(lname, this);
  if (sname)
    parser->RegisterOptionByShortName(sname, this);
}

Command::Command(
    CommandHolder* holder, int flags, const char* name,
    const char* description)
    : CommandHolder(name), holder_(holder), description_(description) {
  if (holder)
    holder->RegisterCommand(name, this);
}
