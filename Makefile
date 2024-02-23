all: clean decompress comic_book_reader

decompress:
	bash scripts/decompression_archives.sh

comic_book_reader: scripts/main.cpp scripts/comic_book.cpp
	g++ -Wall -Wextra -o comic_book_reader scripts/main.cpp scripts/comic_book.cpp -lboost_filesystem -lboost_system

clean:
	bash scripts/suppression_fichiers_temporaires.sh