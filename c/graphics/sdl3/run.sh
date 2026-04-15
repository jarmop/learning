#!/bin/bash

time cmake -S . -B build
time cmake --build build
build/hello
