#!/bin/bash

if [ -z $1 ]
then
   echo 'provide executable name'
   exit 1
fi

pid=$(ps ux | grep -m 1 $1 | awk '{print $2}')

echo "IPv4 connections for $pid:"
echo

lsof -p $pid | grep IPv4
