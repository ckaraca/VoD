#!/bin/bash
# $Id: template_generator.sh,v 1.2 2003/03/08 12:48:45 lem9 Exp $
#
# Shell script that adds a new mimetype with transform function.
#
# The filename should contain either 'mimetype_subtype' or 'mimetype'.
# The suffix '.inc.php3' is appended automatically!
#
# The 'description' parameter will add a new entry in the language file. Watch out for
# special escaping.
#
# Example:  template_generator.sh 'filename' 'description'
# 
if [ $# == 0 ]
then
  echo "Usage: template_generator.sh 'filename' 'description'"
  echo ""
  exit 65
fi



./generator.sh 'TEMPLATE' "$1" "$2"
echo " "
echo "New TRANSFORM FUNCTION $1.inc.php3 added."
