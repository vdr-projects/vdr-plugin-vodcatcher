#
# Makefile for the Vodcatcher VDR plugin
#
# $Id$

# The official name of this plugin.
# This name will be used in the '-P...' option of VDR to load the plugin.
# By default the main source file also carries this name.
#
PLUGIN = vodcatcher

### The version number of this plugin (taken from the main source file):

VERSION = $(shell grep 'static const char VERSION\[\] =' src/Version.h | \
  awk '{ print $$6 }' | sed -e 's/[";]//g')

### The C++ compiler and options:

CXX      ?= g++
CXXFLAGS ?= -fPIC -O2 -Wall -Woverloaded-virtual

### The directory environment:

DVBDIR = ../../../../DVB
VDRDIR = ../../..
LIBDIR = ../../lib
TMPDIR = /tmp

### Conditionals:
#DEBUG=1

### Some defaults that can be overriden:

DEFAULT_CACHE_DIR = /var/cache/vdr-plugin-vodcatcher

### Allow user defined options to overwrite defaults:

-include $(VDRDIR)/Make.config

### The version number of VDR's plugin API (taken from VDR's "config.h"):

APIVERSION = $(shell sed -ne '/define APIVERSION/s/^.*"\(.*\)".*$$/\1/p' \
  $(VDRDIR)/config.h)

### The name of the distribution archive:

ARCHIVE = $(PLUGIN)-$(VERSION)
PACKAGE = vdr-$(ARCHIVE)

### Includes and Defines (add further entries here):

INCLUDES += -I. -I$(VDRDIR)/include -I$(DVBDIR)/include
INCLUDES += `curl-config --cflags` 
INCLUDES += -I./tinyxml

DEFINES += -D_GNU_SOURCE -DPLUGIN_NAME_I18N='"$(PLUGIN)"'
DEFINES += -DCXXTEST_ABORT_TEST_ON_FAIL 
DEFINES += -DCXXTEST_HAVE_EH
DEFINES += -DCXXTEST_HAVE_STD
DEFINES += -DTIXML_USE_STL

DEFINES += -DDEFAULT_CACHE_DIR='"$(DEFAULT_CACHE_DIR)"'

ifdef DEBUG
	DEFINES += -DDEBUG
endif

# libcurl, libxml, libxml++
LIBS += `curl-config --libs`

### The source files (add further files here):

-include sources.mk

SRCS_TESTS := $(wildcard src/*_test.cc)
SRCS_TESTPARTS := $(addsuffix .cxx.cc,$(basename ${SRCS_TESTS}))

### The object files

OBJS := $(addsuffix .o,$(basename ${SRCS}))
OBJS_TESTABLE  = $(addsuffix .o,$(basename ${SRCS_TESTABLE}))
OBJS_TESTS := $(addsuffix .o,$(basename ${SRCS_TESTS}))
OBJS_TESTONLY := $(addsuffix .o,$(basename ${SRCS_TESTONLY}))
OBJS_TESTPARTS := $(addsuffix .o,$(basename ${SRCS_TESTPARTS}))
OBJS += $(OBJS_TESTABLE)

### Implicit rules:

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(DEFINES)  $(INCLUDES) $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $(DEFINES)  $(INCLUDES) $< -o $@

#.PRECIOUS: %_test.cxx.cc
%_test.cxx.cc: %_test.cc
	./cxxtestgen --have-std --part -o $@ $<

# Dependencies:

MAKEDEP = $(CXX) -MM
BUILD_DEPFILE = .dependencies
TESTS_DEPFILE = .dependencies.tests

$(BUILD_DEPFILE): Makefile
	@$(MAKEDEP) $(DEFINES) $(INCLUDES) $(SRCS) $(SRCS_TESTABLE) \
	  | sed "s/.*: \([^ ]*\/\).*/\1\0/" > $@

$(TESTS_DEPFILE): Makefile $(SRCS_TESTPARTS)
	@$(MAKEDEP) $(DEFINES) $(INCLUDES) $(SRCS_TESTABLE) $(SRCS_TESTONLY) \
          $(SRCS_TESTPARTS) | sed "s/.*: \([^ ]*\/\).*/\1\0/" > $@

ifeq ($(MAKECMDGOALS),runtests)
    -include $(TESTS_DEPFILE)
else
    -include $(BUILD_DEPFILE)
endif

### Internationalization (I18N):

PODIR     = po
LOCALEDIR = $(VDRDIR)/locale
I18Npo    = $(wildcard $(PODIR)/*.po)
I18Nmsgs  = $(addprefix $(LOCALEDIR)/, $(addsuffix /LC_MESSAGES/vdr-$(PLUGIN).mo, $(notdir $(foreach file, $(I18Npo), $(basename $(file))))))
I18Npot   = $(PODIR)/$(PLUGIN).pot

%.mo: %.po
	msgfmt -c -o $@ $<

$(I18Npot): $(SRCS) $(SRCS_TESTABLE)
	xgettext -C -cTRANSLATORS --no-wrap --no-location -k -ktr -ktrNOOP --msgid-bugs-address='<tg@e-tobi.net>' -o $@ $^

%.po: $(I18Npot)
	msgmerge -U --no-wrap --no-location --backup=none -q $@ $<
	@touch $@

$(I18Nmsgs): $(LOCALEDIR)/%/LC_MESSAGES/vdr-$(PLUGIN).mo: $(PODIR)/%.mo
	@mkdir -p $(dir $@)
	mv $< $@

.PHONY: i18n
i18n: $(I18Nmsgs)

### Targets:

all: libvdr-$(PLUGIN).so i18n

testrunner.cc:
	./cxxtestgen -o testrunner.cc --error-printer --root

testrunner: $(OBJS_TESTPARTS) $(OBJS_TESTABLE) $(OBJS_TESTONLY) testrunner.cc
	$(CXX) $(CXXFLAGS) testrunner.cc $(OBJS_TESTPARTS) $(OBJS_TESTABLE) \
          $(OBJS_TESTONLY) $(DEFINES) $(INCLUDES) $(LIBS) -o $@  

runtests: testrunner
	./testrunner

libvdr-$(PLUGIN).so: $(OBJS)
	$(CXX) $(CXXFLAGS) -shared $(OBJS) -L. $(LIBS) -o $@
	@cp $@ $(LIBDIR)/$@.$(APIVERSION)

dist: clean
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@mkdir $(TMPDIR)/$(ARCHIVE)
	@cp -a * $(TMPDIR)/$(ARCHIVE)
	@tar czf $(PACKAGE).tar.gz -C $(TMPDIR) --exclude debian --exclude CVS \
	  --exclude .svn --exclude tools --exclude .cproject --exclude .project \
          --exclude continuoustesting $(ARCHIVE)
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@echo Distribution package created as $(PACKAGE).tar.gz

clean:
	@-rm -f $(BUILD_DEPFILE) $(TESTS_DEPFILE) *.so* *.tar.gz core* *~
	@-find . -name \*.\o -exec rm -f {} \; 
	@-rm -f testrunner testrunner.cc src/*_test.cxx.cc
