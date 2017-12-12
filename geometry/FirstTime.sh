#/bin/bash

for file in `ls *.gdml`; do
    perl -p -i -e 's/PWD/$ENV{'PWD'}\//g' ${file}
done
