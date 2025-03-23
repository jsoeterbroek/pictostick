#!/bin/bash
# remove _24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24
# from filename

re='^(.*)\(([0-9]+)\.[0-9]+\).*$'

for f in 24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24/* ; do 
  fbname=$(basename "$f")
  _str=${fbname%_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.png}
  __str="$_str.png"
  echo "$__str"
  cp "$f" short_names/"$__str"
done
