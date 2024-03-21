#!/bin/bash

cd archives
# Supprime les fichiers .zip existants
for file in *.zip; do
    if [ -e "$file" ]; then
        rm "$file"
    fi
done

# Supprime les fichiers .rar existants
for file in *.rar; do
    if [ -e "$file" ]; then
        rm "$file"
    fi
done

# Supprime les images extraites 
rm -r extractions 
cd ..