#!/bin/bash

WC=$(pwd)

cd ${1}
find  -L ./ -type f -exec md5sum {} \; | md5sum | cut -d' ' -f1 > ${WC}/sgmeteo.md5
echo MD5: $(cat ${WC}/install.md5)
cd ${WC}