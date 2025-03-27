#!/bin/bash


for f in *.png ; do 
  echo "$f"
  convert "$f" -background white -alpha remove -alpha off white.png
  mv white.png "$f"
done
