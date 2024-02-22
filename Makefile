all: clean decompress comic_book_reader

decompress:
	bash scripts/decompression_archives.sh

comic_book_reader: scripts/main.cpp scripts/comic_book.cpp
	g++ -std=c++11 -Wall -Wextra -o comic_book_reader scripts/main.cpp scripts/comic_book.cpp

clean:
	bash scripts/suppression_fichiers_temporaires.sh