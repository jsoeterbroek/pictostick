#!/bin/bash


for f in *.png ; do 
  echo "$f"
  #convert $f -resize 96x96 resized/$f
  convert $f -colorspace Gray grey/$f
done
