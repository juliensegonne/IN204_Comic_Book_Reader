#!/bin/bash
cd ..
cd archives 
mkdir extractions #contiendra tous les fichiers extraits des archives

# Renomme tous les fichiers .cbz en fichiers .zip
for file in *.cbz; do
    cp "$file" "${file%.cbz}.zip"
done

# Renomme tous les fichiers .cbr en fichiers .rar
for file in *.cbr; do
    cp "$file" "${file%.cbr}.rar"
done

# Décompresse les fichiers .zip 
for file in *.zip; do
    unzip "$file" -d "extractions/extraction_${file%.zip}"
done

# Décompresse les fichiers .rar 
for file in *.rar; do
    mkdir extractions/extraction_${file%.rar}
    unrar x "$file" -d "extractions/extraction_${file%.rar}"
done

cd ..