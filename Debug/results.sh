#!/bin/bash
cat $1 | grep acc | awk '{print $7}' | sort | sed 's:^.\(.*\).$:\1:'
