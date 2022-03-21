#!/bin/bash
sed -i.bu '/<item /d' style/gui.xml

models_list=($(ls -l1 models/))
names_list=(${models_list[@]%.obj})

for (( i = ${#models_list[@]} - 1; i >= 0; i--)); do
    sed -i.bu '/<items>/a \
<item translatable="yes" id="obj_'${names_list[$i]}'">models/'${models_list[$i]}'</item>\
' style/gui.xml
done
