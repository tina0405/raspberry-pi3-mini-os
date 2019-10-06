#!/bin/bash
cat System.map | grep -oP 'kservice_.*?\b' > symbol.txt

