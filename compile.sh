#!/usr/bin/env bash

cmake ./
make
mv playground Debug/
chmod u+x Debug/run.sh
