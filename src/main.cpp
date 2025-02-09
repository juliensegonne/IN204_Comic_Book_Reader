#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "comic_book.hpp"
#include "affichage.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
    // Paramètres pour le chargement des images
    std::string inputFolder = "archives/extractions/extraction_True_Comic_png_cbr/";
    TypeArchive type = TypeArchive::CBZ;
    int nb_images_par_pages = 1;
    Book book(type, nb_images_par_pages,inputFolder);

    std::atomic<bool> imageLoaded(false); // Indicateur de chargement d'image

    // Lancement du chargement asynchrone des images
    std::thread loadImageThread(ChargementAsynchrone, inputFolder, type, nb_images_par_pages, std::ref(book), std::ref(imageLoaded));

    // Attendre que le chargement soit terminé (ou un certain temps)
    while (!imageLoaded.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Attendre un court moment avant de vérifier à nouveau
    }

    // Si le chargement des images a réussi, afficher les images
    if (!book.ObtenirPages().empty()) {
        // Afficher les images
        QApplication app(argc, argv);
        FullScreenLabel label(book,1.0);
        label.showFullScreen();
        return app.exec();
    } else {
        std::cerr << "Aucune image chargée. Arrêt du programme." << std::endl;
        return 1;
    }
}
