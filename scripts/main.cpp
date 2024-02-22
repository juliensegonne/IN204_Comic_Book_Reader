#include "comic_book.hpp"
#include <iostream>

int main() {
    // Initialisation d'un Book
    Book book(TypeArchive::CBZ);

    // Initialisation de plusieurs images
    Image image1("archives/extractions/extraction_archive_jpg_cbz/Apache Trail 001-01.jpg");
    Image image2("archives/extractions/extraction_archive_jpg_cbz/Apache Trail 001-02.jpg");
    Image image3("archives/extractions/extraction_archive_jpg_cbz/Apache Trail 001-03.jpg");

    // Création d'une page et ajout des images
    Page page1;
    page1.AjouterImage(image1);
    page1.AjouterImage(image2);

    // Création d'une deuxième page et ajout des images
    Page page2;
    page2.AjouterImage(image3);

    // Ajout des pages au book
    book.AjouterPage(page1);
    book.AjouterPage(page2);

    const std::vector<Page>& pages = book.ObtenirPages();
    for (const Page& page : pages) {
        const std::vector<Image>& images = page.ObtenirImages();
        for (const Image& image : images) {
            std::cout << "Nom de l'image : " << image.ObtenirNomFichier() << std::endl;
        }
    }

    return 0;
}
