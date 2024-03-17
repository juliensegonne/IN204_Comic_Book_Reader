#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <QApplication>
#include "comic_book.hpp"
#include "affichage.hpp"

using namespace std::filesystem;

int main(int argc, char *argv[]) {
    int nb_images_par_pages = 2;

    // Chemin pour accéder aux images
    std::string cheminImages = "archives/extractions/extraction_archive_jpg_cbz/";
    Book book(TypeArchive::CBZ, nb_images_par_pages);
    book=book.ChargerComicBook(cheminImages,nb_images_par_pages, TypeArchive::CBZ);

    // Affichage fenêtre
    QApplication app(argc, argv);
    FullScreenLabel label(book);
    label.showFullScreen();

    return app.exec();
}