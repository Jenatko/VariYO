#!/bin/sh
# find . -name '*.h' -exec ./generator.sh '{}' ';'

cat << EOF
<Compile Include="$( printf '%s' "$1" | sed 's#^./##g' | sed 's#/#\\#g' )">
    <SubType>compile</SubType>
</Compile>
EOF
