# Copyright (c) 2008,2009,2010,2011 Carlo Contavalli (ccontavalli@gmail.com).
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
#    1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 
#    2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY Carlo Contavalli ''AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL Carlo Contavalli OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# The views and conclusions contained in the software and documentation are
# those of the authors and should not be interpreted as representing official
# policies, either expressed or implied, of Carlo Contavalli.

STATIC_TARGET = libyaarg0.a

DYNAMIC_TARGET = libyaarg0.so.0.0.1
DYNAMIC_COMPILE_FLAGS = -fPIC
DYNAMIC_LINK_FLAGS = -shared -Wl,-soname,libyaarg.so.0

ALLSOURCES = *.cc
LDFLAGS = -lstdc++ -ggdb3 #-lduma -lm
CPPFLAGS = -Wall -pedantic -ggdb3 -O0 -DPRINT_DEBUG -Wformat-nonliteral -Wextra -Wstrict-overflow=5 -Wfloat-equal -Wconversion -Wlogical-op -fstack-protector-all -Wno-unused-parameter -fno-exceptions -std=c++0x
# CPPFLAGS = -Wall -pedantic -O0 -DPRINT_DEBUG
# -Wunreachable-code -> causes lot of warnings due to standard libraries!
# -Wformat-zero-length -> C and objective C only.
# -Wshadow -> arguments like size and friends shadow globals :(
# TRY USING mudflap library!

.PHONY: all deps clean

all: dynamic

deps: Makefile.deps

-include Makefile.deps

Makefile.deps: Makefile
	fastdep --extraremakedep=Makefile --remakedeptarget=Makefile.deps $(ALLSOURCES) > Makefile.deps

static: $(STATIC_TARGET)

dynamic: LDFLAGS += $(DYNAMIC_LINK_FLAGS)
dynamic: CPPFLAGS += $(DYNAMIC_COMPILE_FLAGS)
dynamic: $(DYNAMIC_TARGET)

config-parser-options.o: config-parser-options.cc
config-parser.o: config-parser.cc
config-parser-argv.o: config-parser-argv.cc

DEPENDENCIES = config-parser-options.o config-parser.o config-parser-argv.o 

$(DYNAMIC_TARGET): $(DEPENDENCIES)
	$(CC) $(LDFLAGS) -o $(DYNAMIC_TARGET) $^

$(STATIC_TARGET): $(DEPENDENCIES)
	$(AR) rcs -o $(STATIC_TARGET) $^

clean:
	rm -f *.o *.so.* *.a
