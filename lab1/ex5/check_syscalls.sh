#!/bin/bash

####################
# Lab 1 Exercise 5
# Name: Kieron Koh
# Student No: A0206059H
# Lab Group: 13
####################

echo "Printing system call report"

# compile file
gcc -std=c99 pid_checker.c -o ex5

# use strace to get report
strace --summary-only ./ex5



