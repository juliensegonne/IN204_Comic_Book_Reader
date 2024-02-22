all: decompress clean

decompress:
	bash scripts/decompression_archives.sh

clean:
	bash scripts/suppression_fichiers_temporaires.sh
