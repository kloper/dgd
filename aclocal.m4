
AC_DEFUN([DG_GUESS_LOCAL_LIB],
[cat >&AS_MESSAGE_LOG_FD <<EOF
------------
Package tests.
------------
EOF

AC_MSG_CHECKING([where is $1])

eval cont=\$$2
if test "X$cont" = "X"; then
   for dir in . .. ../.. ../..; do 
       candidates=`echo $srcdir/$dir/$1*`
       if test "$candidates" != "$srcdir/$dir/$1*" ; then
          for d in $candidates; do
              if test -d "$d"; then
                 eval $2=$d
              fi
          done
          eval cont=\$$2
          if test "X$2" != "X"; then
             echo "final $2 $cont" >&AS_MESSAGE_LOG_FD
             break
          fi
       fi
   done
fi

AC_MSG_RESULT($cont)
])

AC_DEFUN([DG_VERSION_FROM_CVS],
[cat >&AS_MESSAGE_LOG_FD <<EOF
------------
Verstion tests.
------------
EOF

AC_MSG_CHECKING([package version in CVS...])

DGD_VERSION="Unknown"
if test "X$CVS" != "X"; then
   changequote((, ))
   out=`$CVS log configure.ac 2>&1 | grep -m1 'dgd_[0-9_-]\+[a-zA-Z]*'`
   v=`$CVS log configure.ac 2>&1 | grep -m1 'dgd_[0-9_-]\+[a-zA-Z]*' | sed 's/:.*$//'`
   echo "retrieved tag: $v" >&AS_MESSAGE_LOG_FD
   if test "X$v" != "X"; then
      DGD_VERSION="`echo $v | sed 's/_/-/' | sed 's/_/./g'` `date`"
      DGD_SHORT_VERSION=`echo $v | sed 's/dgd_//' | sed 's/_/./g'`
      echo "version=$DGD_VERSION" >&AS_MESSAGE_LOG_FD
   fi
   changequote([, ])
fi

AC_SUBST(DGD_VERSION)
AC_SUBST(DGD_SHORT_VERSION)
AC_MSG_RESULT($DGD_VERSION)
])



