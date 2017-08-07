#!/bin/sh
# offlineimap.sh - offlineimap wrapper
# Copyright (C) 2015-2017  Andrea Bolognani <andrea.bolognani@roundhousecode.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

BASE="$HOME/.offlineimap"

function notify() {
  typeset priority=$1
  typeset message=$2

  case "$priority" in

    "info")
      urgency="normal"
      icon="dialog-information"
      ;;

    "warning")
      urgency="normal"
      icon="dialog-warning"
      ;;

    "error")
      urgency="critical"
      icon="dialog-error"
      ;;
  esac

  $NOTIFY_SEND \
    --urgency "$urgency" \
    --icon "$icon" \
    --app-name "offlineimap" \
    "offlineimap" \
    "$message"
}

function main() {
  CFG="$BASE/cfg"
  LOG="$BASE/log"
  LCK="$BASE/lck"
  ENV="$BASE/$$.env"

  OFFLINEIMAP=$(which offlineimap)
  NOTIFY_SEND=$(which notify-send)

  # Import environment from gnome-shell

  ENV_IMPORT='DISPLAY XAUTHORITY DBUS_SESSION_BUS_ADDRESS'
  ps ux | while read _ pid _ _ _ _ _ _ _ _ exe _
  do
    exe=${exe##*/}
    test "$exe" = "gnome-shell" || continue

    for var in $ENV_IMPORT
    do
      var=$(grep -az "^$var=" "/proc/$pid/environ" | tr -d '\0')
      echo "export $var"
    done
  done >"$ENV"
  . "$ENV"
  rm -f "$ENV"

  # Acquire a lock

  mkdir "$LCK" >/dev/null 2>&1 || {
    notify \
      "warning" \
      "Another instance is running"
    return 0
  }

  # Run offlineimap

  output=$($OFFLINEIMAP -c "$CFG" "$@" 2>&1)

  case "$output" in

    *"service busy"*)
      # Server busy
      notify \
        "warning" \
        "Server busy"
      rc=1
      ;;

    *"Could not resolve name"*)
      # Not connected to the network
      notify \
        "warning" \
        "Unable to connect to server"
      rc=1
      ;;

    *"AUTHENTICATE GSSAPI"*)
      # Forgot to login this morning? :)
      notify \
        "warning" \
        "Kerberos ticket missing"
      rc=1
      ;;

    *"ERROR"*)
      # Should never happen
      notify \
        "error" \
        "Unknown error. Check log file for details"
      rc=2
      ;;

    *)
      # Sync succesful, everything's fine
      rc=0
        ;;
  esac

  echo "
===============================
 $(date)
===============================
$OFFLINEIMAP -c \"$CFG\" $@
---
$output
---
rc=$rc
" >>"$LOG"

  rm -rf "$LCK"
  return $rc
}

APP=${0##*/}
main "$@"
exit $?
