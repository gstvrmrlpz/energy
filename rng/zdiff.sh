#!/usr/bin/env bash

LANG=C

OLD=$IFS
IFS=';'

while read -r e p c s; do
    printf '%s;%s;%s;%s;%s\n' $e $p $c $s $(bc <<< "$p - $s")
done < $1

IFS=$OLD
