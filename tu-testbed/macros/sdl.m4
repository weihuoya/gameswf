# Configure paths for SDL
# Sam Lantinga 9/21/99
# stolen from Manish Singh
# stolen back from Frank Belew
# stolen from Manish Singh
# Shamelessly stolen from Owen Taylor

dnl AM_PATH_SDL([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for SDL, and define SDL_CFLAGS and SDL_LIBS
dnl
AC_DEFUN([AM_PATH_SDL],
[dnl 
dnl Get the cflags and libraries from the sdl-config script
dnl
AC_ARG_WITH(sdl-prefix,[  --with-sdl-prefix=PFX   Prefix where SDL is installed (optional)],
            sdl_prefix="$withval", sdl_prefix="")
AC_ARG_WITH(sdl-exec-prefix,[  --with-sdl-exec-prefix=PFX Exec prefix where SDL is installed (optional)],
            sdl_exec_prefix="$withval", sdl_exec_prefix="")
AC_ARG_ENABLE(sdltest, [  --disable-sdltest       Do not try to compile and run a test SDL program],
		    , enable_sdltest=yes)

  if test x$sdl_exec_prefix != x ; then
     sdl_args="$sdl_args --exec-prefix=$sdl_exec_prefix"
     if test x${SDL_CONFIG+set} != xset ; then
        SDL_CONFIG=$sdl_exec_prefix/bin/sdl-config
     fi
  fi
  if test x$sdl_prefix != x ; then
     sdl_args="$sdl_args --prefix=$sdl_prefix"
     if test x${SDL_CONFIG+set} != xset ; then
        SDL_CONFIG=$sdl_prefix/bin/sdl-config
     fi
  fi

  AC_REQUIRE([AC_CANONICAL_TARGET])
  PATH="$prefix/bin:$prefix/usr/bin:$PATH"
  AC_PATH_PROG(SDL_CONFIG, sdl-config, no, [$PATH])
  min_sdl_version=ifelse([$1], ,0.11.0,$1)
  AC_MSG_CHECKING(for SDL - version >= $min_sdl_version)
  no_sdl=""
  if test "$SDL_CONFIG" = "no" ; then
    no_sdl=yes
  else
    SDL_CFLAGS=`$SDL_CONFIG $sdlconf_args --cflags`
    SDL_LIBS=`$SDL_CONFIG $sdlconf_args --libs`

    sdl_major_version=`$SDL_CONFIG $sdl_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    sdl_minor_version=`$SDL_CONFIG $sdl_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    sdl_micro_version=`$SDL_CONFIG $sdl_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_sdltest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $SDL_CFLAGS"
      LIBS="$LIBS $SDL_LIBS"
dnl
dnl Now check if the installed SDL is sufficiently new. (Also sanity
dnl checks the results of sdl-config to some extent
dnl
      rm -f conf.sdltest
      AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"

char*
my_strdup (char *str)
{
  char *new_str;
  
  if (str)
    {
      new_str = (char *)malloc ((strlen (str) + 1) * sizeof(char));
      strcpy (new_str, str);
    }
  else
    new_str = NULL;
  
  return new_str;
}

int main (int argc, char *argv[])
{
  int major, minor, micro;
  char *tmp_version;

  /* This hangs on some systems (?)
  system ("touch conf.sdltest");
  */
  { FILE *fp = fopen("conf.sdltest", "a"); if ( fp ) fclose(fp); }

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_sdl_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_sdl_version");
     exit(1);
   }

   if (($sdl_major_version > major) ||
      (($sdl_major_version == major) && ($sdl_minor_version > minor)) ||
      (($sdl_major_version == major) && ($sdl_minor_version == minor) && ($sdl_micro_version >= micro)))
    {
      return 0;
    }
  else
    {
      printf("\n*** 'sdl-config --version' returned %d.%d.%d, but the minimum version\n", $sdl_major_version, $sdl_minor_version, $sdl_micro_version);
      printf("*** of SDL required is %d.%d.%d. If sdl-config is correct, then it is\n", major, minor, micro);
      printf("*** best to upgrade to the required version.\n");
      printf("*** If sdl-config was wrong, set the environment variable SDL_CONFIG\n");
      printf("*** to point to the correct copy of sdl-config, and remove the file\n");
      printf("*** config.cache before re-running configure\n");
      return 1;
    }
}

],, no_sdl=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_sdl" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$SDL_CONFIG" = "no" ; then
       echo "*** The sdl-config script installed by SDL could not be found"
       echo "*** If SDL was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the SDL_CONFIG environment variable to the"
       echo "*** full path to sdl-config."
     else
       if test -f conf.sdltest ; then
        :
       else
          echo "*** Could not run SDL test program, checking why..."
          CFLAGS="$CFLAGS $SDL_CFLAGS"
          LIBS="$LIBS $SDL_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
#include "SDL.h"

int main(int argc, char *argv[])
{ return 0; }
#undef  main
#define main K_and_R_C_main
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding SDL or finding the wrong"
          echo "*** version of SDL. If it is not finding SDL, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means SDL was incorrectly installed"
          echo "*** or that you have moved SDL since it was installed. In the latter case, you"
          echo "*** may want to edit the sdl-config script: $SDL_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     SDL_CFLAGS=""
     SDL_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(SDL_CFLAGS)
  AC_SUBST(SDL_LIBS)
  rm -f conf.sdltest
])


AC_DEFUN([AM_PATH_SDL_MIXER],
[
  dnl Lool for the header
  AC_ARG_WITH(sdl_mixer_incl, [  --with-sdl_mixer-incl        directory where sdl_mixer header is], with_sdl_mixer_incl=${withval})
  AC_CACHE_VAL(ac_cv_path_sdl_mixer_incl,[
  if test x"${with_sdl_mixer_incl}" != x ; then
    if test -f ${with_sdl_mixer_incl}/SDL_mixer.h ; then
      ac_cv_path_sdl_mixer_incl=`(cd ${with_sdl_mixer_incl}; pwd)`
    elif test -f ${with_sdl_mixer_incl}/SDL_mixer.h ; then
      ac_cv_path_sdl_mixer_incl=`(cd ${with_sdl_mixer_incl}; pwd)`
    else
      AC_MSG_ERROR([${with_sdl_mixer_incl} directory doesn't contain SDL_mixerlib.h])
    fi
  fi
  ])

  if test x"${ac_cv_path_sdl_mixer_incl}" = x ; then
    AC_MSG_CHECKING([for SDL_mixer header])
    incllist="/sw/include /sw/include /usr/local/include /home/latest/include /opt/include /usr/include .. ../.."

    for i in $incllist; do
      if test -f $i/SDL/SDL_mixer.h; then
        ac_cv_path_sdl_mixer_incl=$i/SDL
        break
      fi
    done

    SDL_MIXER_CFLAGS=""
    if test x"${ac_cv_path_sdl_mixer_incl}" = x ; then
      AC_MSG_RESULT(none)
      AC_CHECK_HEADERS(SDL_mixer.h, [ac_cv_path_sdl_mixer_incl=""])
    else
      AC_MSG_RESULT(${ac_cv_path_sdl_mixer_incl})
      if test x"${ac_cv_path_sdl_mixer_incl}" != x"/usr/include"; then
        ac_cv_path_sdl_mixer_incl="-I${ac_cv_path_sdl_mixer_incl}"
      else
        ac_cv_path_sdl_mixer_incl=""
      fi
    fi
  fi

  if test x"${ac_cv_path_sdl_mixer_incl}" != x ; then
    SDL_MIXER_CFLAGS="${ac_cv_path_sdl_mixer_incl}"
  fi

  dnl Look for the library
  AC_ARG_WITH(sdl_mixer_lib, [  --with-sdl_mixer-lib         directory where sdl_mixer library is], with_sdl_mixer_lib=${withval})
  AC_MSG_CHECKING([for sdl_mixer library])
  AC_CACHE_VAL(ac_cv_path_sdl_mixer_lib,[
  if test x"${with_sdl_mixer_lib}" != x ; then
    if test -f ${with_sdl_mixer_lib}/libSDL_mixer.a ; then
      ac_cv_path_sdl_mixer_lib=`(cd ${with_sdl_mixer_lib}; pwd)`
    elif test -f ${with_sdl_mixer_lib}/libSDL_mixer.a -o -f ${with_sdl_mixer_lib}/libSDL_mixer.so; then
      ac_cv_path_sdl_mixer_lib=`(cd ${with_sdl_mixer_incl}; pwd)`
    else
      AC_MSG_ERROR([${with_sdl_mixer_lib} directory doesn't contain libsdl_mixer.a])
    fi
  fi
  ])

  if test x"${ac_cv_path_sdl_mixer_lib}" = x ; then
    liblist="/sw/lib /usr/local/lib /home/latest/lib /opt/lib /usr/lib .. ../.."

    for i in $liblist; do
    if test -f $i/libSDL_mixer.a -o -f $i/libSDL_mixer.so -o -f $i/libSDL_mixer.dylib; then
       ac_cv_path_sdl_mixer_lib=$i
       break
    fi
    done

    SDL_MIXER_LIBS=""
    if test x"${ac_cv_path_sdl_mixer_lib}" = x ; then
      AC_MSG_RESULT(none)
      dnl if we can't find libsdl_mixer via the path, see if it's in the compiler path
      AC_CHECK_LIB(SDL_mixer, Mix_Linked_Version, SDL_MIXER_LIBS="-lSDL_mixer")
    else
      AC_MSG_RESULT(${ac_cv_path_sdl_mixer_lib})
      if test x"${ac_cv_path_sdl_mixer_lib}" != x"/usr/lib"; then
        ac_cv_path_sdl_mixer_lib="-L${ac_cv_path_sdl_mixer_lib} -lSDL_mixer"
      else
        ac_cv_path_sdl_mixer_lib="-lSDL_mixer"
      fi
    fi
  fi

  if test x"${ac_cv_path_sdl_mixer_lib}" != x ; then
    SDL_MIXER_LIBS="${ac_cv_path_sdl_mixer_lib}"
  fi

  AC_SUBST(SDL_MIXER_CFLAGS)
  AC_SUBST(SDL_MIXER_LIBS)
])
