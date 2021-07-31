#!/bin/bash

content=`cat include/sqlpp11/mysql/mysql.h`

pattern='#define SQLPP11_MYSQL_VERSION_MAJOR *([0-9a-zA-Z_]*)'
[[ $content =~ $pattern ]]
major=${BASH_REMATCH[1]}

pattern='#define SQLPP11_MYSQL_VERSION_MINOR *([0-9a-zA-Z_]*)'
[[ $content =~ $pattern ]]
minor=${BASH_REMATCH[1]}

version="$major.$minor"

git tag -a $version -m "Version $version"
