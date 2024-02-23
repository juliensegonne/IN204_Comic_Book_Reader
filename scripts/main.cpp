#include "comic_book.hpp"
#include <iostream>
#include <boost/filesystem.hpp>

using namespace std::filesystem;

int main() {
    int nb_images_par_pages = 3;
    // Initialisation d'un Book avec nb_images_par_page
    Book book(TypeArchive::CBZ, nb_images_par_pages);

    // Chemin du répertoire contenant les images
    std::string cheminImages = "archives/extractions/extraction_archive_jpg_cbz/";

    // Parcourir les fichiers dans le répertoire
    for (const auto& entry : boost::filesystem::directory_iterator(cheminImages)) {
        if (boost::filesystem::status(entry).type() == boost::filesystem::file_type::regular_file) {
            std::string nomFichier = entry.path().string();
            Image nouvelleImage(nomFichier);

            // Ajouter une nouvelle page si le vecteur de pages est vide
            if (book.ObtenirPages().empty()) {
                Page nouvellePage;
                nouvellePage.AjouterImage(nouvelleImage);
                book.AjouterPage(nouvellePage);
            } else {
                // Ajouter l'image à la dernière page
                const auto& dernierePage = book.ObtenirPages().back();
                if (dernierePage.ObtenirImages().size() < static_cast<size_t>(book.ObtenirImagesParPage())) {
                    dernierePage.AjouterImage(nouvelleImage);
                } else {
                    // Créer une nouvelle page si la dernière est pleine
                    Page nouvellePage;
                    nouvellePage.AjouterImage(nouvelleImage);
                    book.AjouterPage(nouvellePage);
                }
            }
        }
    }
    // Afficher les pages avec leur contenu
    const auto& pages = book.ObtenirPages();
    int numeroPage = 1;
    for (const auto& page : pages) {
        std::cout << "Page " << numeroPage++ << ":" << std::endl;
        const auto& images = page.ObtenirImages();
        int numeroImage = 1;
        for (const auto& image : images) {
            std::cout << "   Image " << numeroImage++ << ": " << image.ObtenirNomFichier() << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}