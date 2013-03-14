# Default target
all:: build

# vars utils
UTIL_SPACE := $() #

# Programs
SHELL_PATH = /bin/sh
CP = /bin/cp -f
RM = /bin/rm -f --preserve-root
MV = /bin/mv
MKDIR = /bin/mkdir -p
INSTALL = /bin/install
GZ = /bin/gzip --best
TAR = /bin/tar
PYTHON = /usr/bin/python
GIT = /usr/bin/git
MAKE ?= /usr/bin/make
CMAKE ?= /usr/bin/cmake

# Source directories
SD_ROOT = $(subst $(UTIL_SPACE),\$(UTIL_SPACE),$(shell pwd))
SD_BUILD = $(SD_ROOT)/build
SD_SRC = $(SD_ROOT)/src
SD_TEST = $(SD_ROOT)/test
SD_DIST = $(SD_ROOT)/dist
SD_DOC = $(SD_ROOT)/doc
SD_TOOLS = $(SD_ROOT)/tools

# source environement
VERSION = $(shell $(SD_ROOT)/git-version-gen.sh)

# install environement
EXENAME = cpulimit

# Install directories
ifdef ID_ROOT
  ID_ROOT := $(ID_ROOT)
else
  ID_ROOT =
endif
ifdef PREFIX
  ID_PREFIX := $(PREFIX)
else
  ID_PREFIX = usr
endif
ID_LOCALSTATE = var
ID_SYSCONF = $(ID_PREFIX)/etc
ID_LIBEXEC = $(ID_PREFIX)/lib
ID_EXEC = $(ID_PREFIX)/bin
ID_DATA = $(ID_PREFIX)/share
ID_MAN = $(ID_DATA)/man
ID_INFO = $(ID_DATA)/info

build: FORCE
	@cd $(SD_SRC); $(MAKE) $@

install: FORCE
	@cd $(SD_SRC); $(MAKE) $@

uninstall: FORCE
	@cd $(SD_SRC); $(MAKE) $@

$(SD_DIST)/$(EXENAME)-$(VERSION): all
	$(MKDIR) $(SD_DIST)/$(EXENAME)-$(VERSION)
	cd $(SD_BUILD); pwd; $(TAR) -jcf $(SD_DIST)/$(EXENAME)-$(VERSION)/$(EXENAME)-$(VERSION).tar.bz2 *
	$(GIT) archive --prefix="$(EXENAME)-$(VERSION)/" --format=tar HEAD | bzip2 -c > $(SD_DIST)/$(EXENAME)-$(VERSION)/$(EXENAME)-$(VERSION)-src.tar.bz2
	$(CP) setup.hint $(SD_DIST)/$(EXENAME)-$(VERSION)

package: $(SD_DIST)/$(EXENAME)-$(VERSION)

packageclean:
	$(RM) -r $(SD_DIST)

clean: FORCE
	@cd $(SD_SRC); $(MAKE) $@

mrproper: FORCE clean packageclean
	$(RM) -r $(SD_BUILD)

.PHONY: FORCE
.EXPORT_ALL_VARIABLES: