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

TARGET = libyaarg.so.0.0.1
LIB_COMPILE_FLAGS = -fPIC
LIB_LINK_FLAGS = -shared -Wl,-soname,libyaarg.so.0

CPPFLAGS = -Wall -pedantic -ggdb3 -O0 -DPRINT_DEBUG -Wformat-nonliteral -Wextra -Wstrict-overflow=5 -Wfloat-equal -Wconversion -Wlogical-op -fstack-protector-all -Wno-unused-parameter -fno-exceptions -std=c++0x $(LIB_COMPILE_FLAGS)
# CPPFLAGS = -Wall -pedantic -O0 -DPRINT_DEBUG
# -Wunreachable-code -> causes lot of warnings due to standard libraries!
# -Wformat-zero-length -> C and objective C only.
# -Wshadow -> arguments like size and friends shadow globals :(
# TRY USING mudflap library!

LDFLAGS = -lstdc++ -ggdb3 $(LIB_LINK_FLAGS) #-lduma -lm

all: $(TARGET)

config-parser-options.o: config-parser-options.cc
config-parser.o: config-parser.cc
config-parser-argv.o: config-parser-argv.cc

$(TARGET): config-parser-options.o config-parser.o config-parser-argv.o 
	$(CC) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f *.o *.so.*
