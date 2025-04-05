#!/bin/bash

#    order = models.IntegerField()
#    picto = models.CharField(max_length = 20)
#    name_nl = models.CharField(max_length = 20)
#    name_en = models.CharField(max_length = 20)
#    activities = models.ForeignKey(Activities, on_delete=models.CASCADE)

i=0
for f in pictostick/static/images/picto/*.png; do
  basename=$(basename "$f")
  basename_no_file_ext=$(basename -- "$f" .png)
  echo "- model: main.activity"
  echo "  pk: $i"
  echo "  fields:"
  echo "    order: $i"
  echo "    picto: $basename"
  echo "    name_nl: $basename_no_file_ext"
  echo "    name_en: $basename_no_file_ext"

  ((i++))

done
