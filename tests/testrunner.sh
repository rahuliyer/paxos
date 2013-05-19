#!/bin/sh

echo $@

for i in $@
do
  ./$i
done
