
# ======================================================================
# Makefile for GoDot v1.00
# ======================================================================

#
# directory structure of GoDot repository
#

MAINDIR = system
SYSTEMDIR = system
LIBDIR = libraries
DEVICEDIR = devices
EXTRADIR = extras
LOADERDIR = loaders
MODIFIERDIR = modifiers
SAVERDIR = savers
BUILDDIR = build
D81FILE = $(BUILDDIR)/tmp.d81

# ======================================================================
# configuration
# ======================================================================

ifeq ($(OS),Windows_NT)

# --- configuration for windows/dos ---
	
	# the shell that the Maketool should use to execute commands
	SHELL=cmd

	# basedir of sourcefiles (with absolute path)
	# SOURCEDIR = $(shell cd)
	SOURCEDIR = $(shell cd)

	# the assembler you want to use, if neccessary with full
	# path. sourcecodes are in ACME syntax
	ASS = acme

	# additional options for the assembler
	# AD: changed
	ASSFLAGS = -I $(LIBDIR) -f cbm 

	# name of the c1541 tool from the vice/x64 package, if
	# neccessary with path. only needed when you want
	# to create diskimages.
	C1541 = c1541

	# name of the x64 emulator from the vice package, if
	# neccessary with path. only needed when you want
	# to start the emulator.
	X64 = x64

	# additional flags for the x64 emulator
	# AD: changed +reu to -reu 
	X64FLAGS = -reu -reusize 512

	# command that should be used to create directories
	MKDIRCOMMAND = mkdir 

	# command that should be used to delete files
	REMOVECOMMAND = del
	DELPAR = /q
	REMOVEDIRCOMMAND = rmdir
	RMPAR = /s /q

# --- end of configuration for windows ---

else

# --- configuration for linux ---

	SOURCEDIR = ${PWD}

	ASS = acme
	ASSFLAGS = -I $(LIBDIR) -f cbm 

	C1541 = c1541

	X64 = \x64
	X64FLAGS = -reu -reusize 512

	MKDIRCOMMAND = mkdir -p
	REMOVECOMMAND = \rm -f

# --- end of configuration for linux ---

endif

# ======================================================================
# some macros
# ======================================================================

#
# the libraries of godot. don't need to be assembled on their own
# but other sourcecodes depend on them
#

GODOTLIBS = $(LIBDIR)/godotkernal.lib $(LIBDIR)/godotlib.lib
INPUTLIB  = $(LIBDIR)/input.inc

#
# lists of filenames for sourcecodes, objectcodes and dependencies
# for the mainfile
#

MAINSRCS = $(MAINDIR)/godot.a
MAINOBJS = $(MAINSRCS:$(MAINDIR)/%.a=$(BUILDDIR)/%)
MAINDEPS = $(GODOTLIBS)

#
# lists of filenames for sourcecodes, objectcodes and dependencies
# for systemfiles
#

SYSTEMSRCS = $(wildcard $(SYSTEMDIR)/gd_*.a)
SYSTEMOBJS = $(SYSTEMSRCS:$(SYSTEMDIR)/gd_%.a=$(BUILDDIR)/god.%)
SYSTEMDEPS = $(GODOTLIBS)

#
# lists of filenames for sourcecodes, objectcodes and dependencies
# for devicedrivers
#

DEVICESRCS = $(wildcard $(DEVICEDIR)/d_*.a)
DEVICEOBJS = $(DEVICESRCS:$(DEVICEDIR)/d_%.a=$(BUILDDIR)/dev.%)
DEVICEDEPS = $(GODOTLIBS)

#
# lists of filenames for sourcecodes, objectcodes and dependencies
# for extras
#

EXTRASRCS = $(wildcard $(EXTRADIR)/*.a)
EXTRAOBJS = $(EXTRASRCS:$(EXTRADIR)/%.a=$(BUILDDIR)/%)
#EXTRADEPS = $(GODOTLIBS)

#
# lists of filenames for sourcecodes, objectcodes and dependencies
# for loaders
#

LOADERSRCS = $(wildcard $(LOADERDIR)/l_*.a)
LOADEROBJS = $(LOADERSRCS:$(LOADERDIR)/l_%.a=$(BUILDDIR)/ldr.%)
LOADERDEPS = $(GODOTLIBS)

#
# lists of filenames for sourcecodes, objectcodes and dependencies
# for modifiers
#

MODIFIERSRCS = $(wildcard $(MODIFIERDIR)/m_*.a)
MODIFIEROBJS = $(MODIFIERSRCS:$(MODIFIERDIR)/m_%.a=$(BUILDDIR)/mod.%)
MODIFIERDEPS = $(GODOTLIBS)

#
# lists of filenames for sourcecodes, objectcodes and dependencies
# for savers
#

SAVERSRCS = $(wildcard $(SAVERDIR)/s_*.a)
SAVEROBJS = $(SAVERSRCS:$(SAVERDIR)/s_%.a=$(BUILDDIR)/svr.%)
SAVERDEPS = $(GODOTLIBS) $(INPUTLIB)

#
# list of all objectcodes that need to be assembled
#

OBJS = $(MAINOBJS) $(SYSTEMOBJS) $(DEVICEOBJS) $(EXTRAOBJS) $(LOADEROBJS) $(MODIFIEROBJS) $(SAVEROBJS) 

#
# list of targets that should be built
#

ALLTARGETS = $(BUILDDIR) $(OBJS)

# ======================================================================
# rules
# ======================================================================

.PHONY: all start d81 clean rebuild

#
# build all targets
#

all: $(ALLTARGETS)
files: all

#
# assemble main
#

$(BUILDDIR)/godot: $(MAINSRCS) $(MAINDEPS)
	$(ASS) $(ASSFLAGS) -o $@ $<

#
# assemble a system file
#

$(BUILDDIR)/god.%: $(SYSTEMDIR)/gd_%.a $(SYSTEMDEPS)
	$(ASS) $(ASSFLAGS) -o $@ $<

#
# assemble a device
#

$(BUILDDIR)/dev.%: $(DEVICEDIR)/d_%.a $(DEVICEDEPS)
	$(ASS) $(ASSFLAGS) -o $@ $<

#
# assemble an extra
#

$(BUILDDIR)/%: $(EXTRADIR)/%.a $(EXTRADEPS)
	$(ASS) $(ASSFLAGS) -o $@ $<

#
# assemble a loader
#

$(BUILDDIR)/ldr.%: $(LOADERDIR)/l_%.a $(LOADERDEPS)
	$(ASS) $(ASSFLAGS) -o $@ $<

#
# assemble a modifier
#

$(BUILDDIR)/mod.%: $(MODIFIERDIR)/m_%.a $(MODIFIERDEPS)
	$(ASS) $(ASSFLAGS) -o $@ $<

#
# assemble a saver
#

$(BUILDDIR)/svr.%: $(SAVERDIR)/s_%.a $(SAVERDEPS)
	$(ASS) $(ASSFLAGS) -o $@ $<

#
# create $(BUILDDIR)
#

$(BUILDDIR): 
	$(MKDIRCOMMAND) $(BUILDDIR)

#
# start godot in x64 emulator
#

start: d81
	$(X64) $(X64FLAGS) -drive8type 1581 -truedrive -8 -autostart $(BUILDDIR)/tmp.d81

#
# create a d81 image
#

d81: $(BUILDDIR)/tmp.d81
$(BUILDDIR)/tmp.d81: $(ALLTARGETS)
	cd $(BUILDDIR) && $(C1541) -format godot,21 d81 tmp.d81
	cd $(BUILDDIR) && $(C1541) -attach tmp.d81 $(notdir $(OBJS:%= -write %)) -write	../$(EXTRADIR)/god.set god.set -write ../$(EXTRADIR)/god.ini god.ini
	$(REMOVECOMMAND) labels.txt $(DELPAR) $(subst /,\,$(OBJS)) 

#
# clean up temporary files
#

ifeq ($(OS),Windows_NT)
clean:
	$(REMOVECOMMAND) $(DELPAR) labels.txt $(subst /,\,$(OBJS)) $(subst /,\,$(D81FILE))
else
clean:
	$(REMOVECOMMAND) labels.txt $(OBJS) $(BUILDDIR)/tmp.d81
endif

#
# clean and build
#

rebuild: clean start

# ======================================================================
# by: Rapid Eraser (and a little finetuning by $AD)
# ======================================================================

