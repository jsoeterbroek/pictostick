#!/bin/bash

for f in 283x283/* ; do 
  fbname=$(basename "$f")
  echo "$fbname"
  #echo "convert 283x283/$fbname -resize 100x100 100x100/$fbname"
  convert 283x283/$fbname -resize 100x100 100x100/$fbname
done
