AC_INIT(module-init-tools,3.16)

AC_CANONICAL_SYSTEM

AX_ENABLE_BUILDDIR([build])

# Use -no-portability since we're never going to use module-init-tools on
# non-Linux systems and it's reasonable to expect GNU-compatibility here.
AM_INIT_AUTOMAKE([-Wno-portability])

# If zlib is required, libz must be linked static, modprobe is in
# /sbin, libz is in /usr/lib and may not be available when it is run.
AC_ARG_ENABLE(zlib,
[  --enable-zlib               Handle gzipped modules],
[if test "$enableval" = "yes"; then
  AC_DEFINE(CONFIG_USE_ZLIB)
  zlib_flags="-Wl,-Bstatic -lz -Wl,-Bdynamic"
fi])

AC_ARG_ENABLE(zlib-dynamic,
[  --enable-zlib-dynamic       Handle gzipped modules, zlib will be linked
                               dynamically. Note that libz must be installed
                               in /lib for this to work.],
[if test "$enableval" = "yes"; then
  AC_DEFINE(CONFIG_USE_ZLIB)
  zlib_flags="-lz"
fi])

AC_PROG_CC
AC_PROG_RANLIB

AC_CHECK_PROGS(DOCBOOKTOMAN, docbook-to-man docbook2man, [no],)
if test x"$DOCBOOKTOMAN" = xno
then
	AC_MSG_WARN([docbook2man not found])
	# fail with a meaningfull error if $DOCBOOKTOMAN called by the makefile
	DOCBOOKTOMAN=docbook2man
fi
 
# Delay adding the zlib_flags until after AC_PROG_CC, so we can distinguish
# between a broken cc and a working cc but missing libz.a.
LDADD="$LDADD $zlib_flags"
AC_SUBST(LDADD)

case $target in
*-*-linux*) ;;
*) AC_MSG_ERROR([Linux only, dude!]);;
esac

# Thanks to Martin Pool
if test x"$GCC" = xyes 
then
    CFLAGS="$CFLAGS -Wunused -Wall"
    AC_MSG_NOTICE([Adding gcc options: $CFLAGS])
fi

AC_ARG_ENABLE(static-utils,
AS_HELP_STRING([--disable-static-utils], [Do not build static utils]),
[],[enable_static_utils=yes])
AM_CONDITIONAL(BUILD_STATIC_UTILS, test "$enable_static_utils" = "yes")

AC_ARG_WITH(moddir,
[  --with-moddir=path      Give the path for system modules directory.]
[                         [ Defaults to /lib/modules ]],
[if test ! -z "$withval"; then
  MODULE_DIR="\\\"$withval/\\\""
  CFLAGS="$CFLAGS -DMODULE_DIR=$MODULE_DIR"
fi])
AC_SUBST(MODULE_DIR)

AC_OUTPUT([Makefile])

