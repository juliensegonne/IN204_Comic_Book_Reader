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

    // Initialisation du book avec le bon nombre d'images par page
    Book book(TypeArchive::CBZ, nb_images_par_pages);

    // Chemin pour accéder aux images
    std::string cheminImages = "archives/extractions/extraction_archive_jpg_cbz/";

    // Pour stocker les noms des images
    std::vector<std::string> nomsImages;

    // Parcourir les images dans le dossier et stocker les noms des images
    for (const auto& entry : boost::filesystem::directory_iterator(cheminImages)) {
        if (boost::filesystem::status(entry).type() == boost::filesystem::file_type::regular_file) {
            std::string nomFichier = entry.path().string();
            nomsImages.push_back(nomFichier);
        }
    }

    // Tri des noms des images par ordre alphabétique
    std::sort(nomsImages.begin(), nomsImages.end());

    // Ajouter les images stockées dans le book
    for (const auto& nomFichier : nomsImages) {
        Image nouvelleImage(nomFichier);

        // Si il n'y aucune page de définie
        if (book.ObtenirPages().empty()) {
            Page nouvellePage;
            nouvellePage.AjouterImage(nouvelleImage);
            book.AjouterPage(nouvellePage);
        } else {
            const auto& dernierePage = book.ObtenirPages().back();
            // Si la dernière page n'est pas pleine
            if (dernierePage.ObtenirImages().size() < static_cast<size_t>(book.ObtenirImagesParPage())) {
                dernierePage.AjouterImage(nouvelleImage);
            } else {
                // Sinon on crée une nouvelle page
                Page nouvellePage;
                nouvellePage.AjouterImage(nouvelleImage);
                book.AjouterPage(nouvellePage);
            }
        }
    }

    // Affichage fenêtre
    QApplication app(argc, argv);
    FullScreenLabel label(book);
    label.showFullScreen();

    return app.exec();
}