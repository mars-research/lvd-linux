ACLOCAL_AMFLAGS = -I m4

insmod_SOURCES = insmod.c testing.h
lsmod_SOURCES = lsmod.c testing.h
modprobe_SOURCES = modprobe.c zlibsupport.c zlibsupport.h testing.h
rmmod_SOURCES = rmmod.c testing.h
depmod_SOURCES = depmod.c zlibsupport.c tables.c \
	zlibsupport.h tables.h testing.h
modinfo_SOURCES = modinfo.c zlibsupport.c zlibsupport.h testing.h
modindex_SOURCES = modindex.c zlibsupport.c zlibsupport.h testing.h

insmod_static_SOURCES = insmod.c
insmod_static_LDFLAGS = -static
# We don't want the $(zlib_flags) here: that makes a dynamic executable
insmod_static_LDADD = libmodtools.a

EXTRA_insmod_SOURCES =
EXTRA_lsmod_SOURCES =
EXTRA_modprobe_SOURCES = 
EXTRA_rmmod_SOURCES =
EXTRA_insmod_static_SOURCES =
EXTRA_depmod_SOURCES =
EXTRA_modinfo_SOURCES =

libmodtools_a_SOURCES = util.c logging.c index.c config_filter.c elfops.c \
	util.h depmod.h logging.h index.h list.h config_filter.h elfops.h
libmodtools_a_CFLAGS = -ffunction-sections

EXTRA_libmodtools_a_SOURCES = elfops_core.c

insmod_LDADD = $(LDADD) libmodtools.a
lsmod_LDADD = $(LDADD) libmodtools.a
modprobe_LDADD = $(LDADD) libmodtools.a
rmmod_LDADD = $(LDADD) libmodtools.a
depmod_LDADD = $(LDADD) libmodtools.a
modinfo_LDADD = $(LDADD) libmodtools.a
modindex_LDADD = $(LDADD) libmodtools.a

MAN5 = depmod.conf.5 depmod.d.5 modprobe.conf.5 modprobe.d.5 \
	modules.dep.5 modules.dep.bin.5
MAN8 = depmod.8 insmod.8 lsmod.8 rmmod.8 modprobe.8 modinfo.8
SGML = $(addprefix doc/,  $(MAN5:%.5=%.sgml) $(MAN8:%.8=%.sgml))
dist_man_MANS = $(MAN5) $(MAN8)
# If they haven't overridden mandir, fix it (never /man!)
mandir =$(shell if [ @mandir@ = $(prefix)/man ]; then if [ $(prefix) = / ]; then echo /usr/share/man; else echo $(prefix)/share/man; fi; else echo @mandir@; fi)

TESTSUITE := $(shell test -e @srcdir@/tests && find @srcdir@/tests -type f ! -name '*~')

EXTRA_DIST = generate-modprobe.conf FAQ CODING stress_modules.sh install-with-care $(SGML) $(man_MANS) $(TESTSUITE)

sbin_PROGRAMS = insmod modprobe rmmod depmod modinfo
if BUILD_STATIC_UTILS
sbin_PROGRAMS += insmod.static
endif
bin_PROGRAMS = lsmod
noinst_PROGRAMS=modindex
noinst_LIBRARIES = libmodtools.a
INSTALL = $(SHELL) $(top_srcdir)/install-with-care

DIST_DIR := /data/work/releases/module-init-tools/

GIT_RELEASE_REMOTE := origin
GIT_RELEASE_BRANCH := master
SCP_RELEASE_URL := "jcm@master.kernel.org:/pub/linux/utils/kernel/module-init-tools/"


MAINTAINERCLEANFILES := $(man_MANS)

# docbook2man writes file itself, doesn't do stdout.
%.8: doc/%.sgml
	if [ "$(DOCBOOKTOMAN)" = "docbook2man" ]; then                     \
	    $(DOCBOOKTOMAN) $< > /dev/null 2>&1;                           \
	else                                                               \
	    $(DOCBOOKTOMAN) $< 2>&1 > $@ | sed 's/^[^:]*://';              \
	fi

%.5: doc/%.sgml
	if [ -h $< ]; then                                                 \
	    ln -s $$(basename $$(readlink $<) .sgml).5 $@;                 \
	elif [ "$(DOCBOOKTOMAN)" = "docbook2man" ]; then                   \
	    $(DOCBOOKTOMAN) $< > /dev/null 2>&1;                           \
	else                                                               \
	    $(DOCBOOKTOMAN) $< 2>&1 > $@ | sed 's/^[^:]*://';              \
	fi

# docbook2man creates extra crap files which need cleaning.
distclean-local: killbuildmanpages

killbuildmanpages:
	rm -f doc/*.tmp manpage.refs manpage.links

tarball: $(DIST_DIR)/module-init-tools-$(VERSION).tar.gz

#$(DIST_DIR)/module-init-tools-$(VERSION).tar.gz: dist
#	mv module-init-tools-$(VERSION).tar.gz $@

# git based version
$(DIST_DIR)/module-init-tools-$(VERSION).tar.gz:
	cd $$(git rev-parse --show-toplevel) && git archive --format=tar --prefix=module-init-tools-$(VERSION)/ v$(VERSION) | (cd $(DIST_DIR) && tar xv)
	cd $(DIST_DIR)/module-init-tools-$(VERSION) && aclocal -I m4 && automake --add-missing --copy && autoconf
	cd $(DIST_DIR) && tar cvfz module-init-tools-$(VERSION).tar.gz module-init-tools-$(VERSION)

$(DIST_DIR)/.pushed/module-init-tools-$(VERSION).tar.gz:
	mkdir -p $(DIST_DIR)/.pushed
	ln -s $(DIST_DIR)/module-init-tools-$(VERSION).tar.gz $(DIST_DIR)/.pushed/module-init-tools-$(VERSION).tar.gz

gpg-tarball: $(DIST_DIR)/module-init-tools-$(VERSION).tar.gz.sign

$(DIST_DIR)/.pushed/module-init-tools-$(VERSION).tar.gz.sign:
	mkdir -p $(DIST_DIR)/.pushed
	ln -s $(DIST_DIR)/module-init-tools-$(VERSION).tar.gz.sign $(DIST_DIR)/.pushed/module-init-tools-$(VERSION).tar.gz.sign

$(DIST_DIR)/module-init-tools-$(VERSION).tar.gz.sign: tarball
	cd $(DIST_DIR) && gpg -a -b -s -o module-init-tools-$(VERSION).tar.gz.sign module-init-tools-$(VERSION).tar.gz

srpm: $(DIST_DIR)/module-init-tools-$(VERSION).tar.gz
	ln -sf $(DIST_DIR)/module-init-tools-$(VERSION).tar.gz /usr/src/rpm/SOURCES/
	set -e; RELEASE=`grep '^Release:' /usr/src/rpm/SPECS/modutils.spec | awk '{ print $$2 }'`; \
	if [ "`grep '%define mit_ver' /usr/src/rpm/SPECS/modutils.spec | awk '{print $$3}'`" != $(VERSION) ]; then \
		sed -e "s/Release: $$RELEASE/Release: `expr $$RELEASE + 1`/" -e 's/%define mit_ver.*/%define mit_ver $(VERSION)/' < /usr/src/rpm/SPECS/modutils.spec > /tmp/modutils.spec; \
		rpm -ba /tmp/modutils.spec; \
		mv /usr/src/rpm/SRPMS/modutils-2.4.21-`expr $$RELEASE + 1`.src.rpm $(DIST_DIR); \
		mv /tmp/modutils.spec /usr/src/rpm/SPECS/modutils.spec; \
	else \
		rpm -ba /usr/src/rpm/SPECS/modutils.spec; \
		mv /usr/src/rpm/SRPMS/modutils-2.4.21-$$RELEASE.src.rpm $(DIST_DIR); \
	fi

# No testsuite?
tests/runtests:
	mkdir tests
	echo '#! /bin/sh' > $@
	echo '# Install the testsuite over this dir. ' >> $@
	chmod a+x $@

$(DIST_DIR)/module-init-tools-testsuite-$(VERSION).tar.gz:
	ln -sfn `pwd` /tmp/module-init-tools-$(VERSION) 
	cd /tmp && tar --exclude '*~' -c -z -f $@ module-init-tools-$(VERSION)/tests
	rm /tmp/module-init-tools-$(VERSION)

old-release: check clean tarball

# git based release
git-clean:
	git gc
	git prune

git-tag:
	if [ "xv$(VERSION)" != x$$(git tag -l v$(VERSION)) ]; \
	then \
		git tag v$(VERSION); \
	fi

git-push:
	git push $(GIT_RELEASE_REMOTE) $(GIT_RELEASE_BRANCH) --tags

scp-tarball: $(DIST_DIR)/.pushed/module-init-tools-$(VERSION).tar.gz $(DIST_DIR)/.pushed/module-init-tools-$(VERSION).tar.gz.sign
	cd $(DIST_DIR) && scp -C module-init-tools-$(VERSION).tar.gz module-init-tools-$(VERSION).tar.gz.sign $(SCP_RELEASE_URL)

git-release: git-clean git-tag

release: git-release

sign-release: git-release gpg-tarball

push-release: sign-release git-push scp-tarball

push: push-release
