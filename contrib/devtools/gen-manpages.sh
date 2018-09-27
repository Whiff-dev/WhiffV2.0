#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

WHIFFD=${WHIFFD:-$SRCDIR/whiffd}
WHIFFCLI=${WHIFFCLI:-$SRCDIR/whiff-cli}
WHIFFTX=${WHIFFTX:-$SRCDIR/whiff-tx}
WHIFFQT=${WHIFFQT:-$SRCDIR/qt/whiff-qt}

[ ! -x $WHIFFD ] && echo "$WHIFFD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
WFFVER=($($WHIFFCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for whiffd if --version-string is not set,
# but has different outcomes for whiff-qt and whiff-cli.
echo "[COPYRIGHT]" > footer.h2m
$WHIFFD --version | sed -n '1!p' >> footer.h2m

for cmd in $WHIFFD $WHIFFCLI $WHIFFTX $WHIFFQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${WFFVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${WFFVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
