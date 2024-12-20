# This software is released under the Boost Software License, version 1.0.
# See LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt
# Copyright Ken Smith kgsmith at gmail dot com, 2013.

# User configurable portion.
prefix := $(HOME)/.local
cpus := 4
opt := -Os -flto=auto
#debug := -g -DDEBUG

# Implementation
prog ?= $(notdir $(CURDIR))
MAKEFLAGS := j$(cpus)
SHELL := bash
os := $(shell uname -o)
g := g++ $(opt) $(debug)
g += -std=c++17
g += -Wall -Werror -pedantic -Wno-unused-local-typedefs
$(if $(filter Darwin,$(os)), \
  $(eval g += -I/opt/homebrew/include) \
 )
g := $(strip $(g))
c := $(g) -c -MMD -I . -I ..
l := $(g)
$(if $(filter Darwin,$(os)), \
  $(eval l += -L/opt/homebrew/lib) \
 )
l := $(strip $(l))
s := $(wildcard *.cpp)
o := $(addprefix .,$(patsubst %.cpp,%.o,$(s)))
d := $(o:o=d)
p := $(filter-out .test-%.o,$(o))
t := $(patsubst %.o,%,$(filter .test-%.o,$(o)))
x := $(filter-out -Werror,$(c))
x := $(filter-out -Wall,$(x))
x := $(filter-out -pedantic,$(x))
pd := .postdepend.sh

.PRECIOUS\
:$(t)\
 $(o)

$(prog)\
:.main.o $(addsuffix .passed,$(t))\
;$(l) -o $@ $<

.test-%.o\
:test-%.cpp $(MAKEFILE_LIST) $(pd)\
;$(x) -o $@ $<\
;export $(subst -,_,$(subst .,_,$@))=$$?\
;if [[ "$$$(subst -,_,$(subst .,_,$@))" == "0" ]]\
;then ./$(pd) $(@:o=d)\
;fi\
;exit $$$(subst -,_,$(subst .,_,$@))

# I want a rule to build a file and post process the
# outputs. I want the rule to be aware of all of the files I
# produce in the rule. I want to be able to choose which of
# the commands' exit codes makes it out of the rule to
# determine success of the rule.
# Python sucks.
# Ruby sucks.
# Lua also sucks.
# I need parallel builds by default.
# I want the build tool to collect all the standard outputs
# independently and weave them together without interleaving
# the output of multiple rules.
# I want to be able to single step the rule generation as
# well as the rule execution
.%.o\
:%.cpp $(MAKEFILE_LIST) $(pd)\
;$(c) -o $@ $<\
;export $(subst -,_,$(subst .,_,$@))=$$?\
;if [[ "$$$(subst -,_,$(subst .,_,$@))" == "0" ]]\
;then ./$(pd) $(@:o=d)\
;fi\
;exit $$$(subst -,_,$(subst .,_,$@))

.test-%.passed\
:.test-%\
;./$< --random --report_level=short --show_progress\
;export $(subst -,_,$(subst .,_,$@))=$$?\
;if [[ "$$$(subst -,_,$(subst .,_,$@))" == "0" ]]\
;then touch $@\
;fi\
;exit $$$(subst -,_,$(subst .,_,$@))

.test-%\
:.test-%.o\
;$(l) -o $@ $< -lboost_unit_test_framework

.PHONY\
:clean\
 install

install\
:$(prog)\
;cp $(prog) $(prefix)/bin

clean\
:\
;rm -Rf $(prog) .test-* .*.o .*.d .*.passed html latex .postdepend.sh

.postdepend.sh\
:$(MAKEFILE_LIST)\
;echo -e\
 "#!/bin/sh\n"\
 "\n"\
 "cp \$$1 \$$1.p\n"\
 "cat \$$1"\
 "| sed"\
 "-e 's/\#.*//'"\
 "-e 's/^[^:]*: *//'" \
 "-e 's/ *\$$//'"\
 "-e '/^\$$/ d' "\
 "-e 's/$$/ :/'"\
 ">> \$$1.p\n"\
 "mv \$$1.p \$$1"\
> $@\
;chmod +x $@

-include $(d)

# vim:tw=60:
