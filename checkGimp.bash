#!/bin/bash
hash gimp 2>/dev/null || { echo >&2 "no"; exit 1; }
echo "yes";
