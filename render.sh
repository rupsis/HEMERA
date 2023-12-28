#! /bin/bash

./hemera > $1.ppm

ffmpeg -i $1.ppm $1.png

rm $1.ppm