#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <algorithm>
#include <chrono>
#include <boost/filesystem.hpp>
#include "comic_book.hpp"
#include "affichage.hpp"
#include <QApplication>

using namespace std::filesystem;

// Fonction pour charger les images en arrière-plan
void loadImagesAsync(const std::string& inputFolder, const TypeArchive type, int nb_images_par_pages, Book& book, std::atomic<bool>& imageLoaded) {
    try {
        // Charger les images depuis inputFolder
        book = book.ChargerComicBook(inputFolder, nb_images_par_pages, type);
        std::cout << "Chargement des images terminé." << std::endl;
        imageLoaded.store(true); // Indiquer que l'image est chargée
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement des images : " << e.what() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    // Paramètres pour le chargement des images
    std::string inputFolder = "archives/extractions/extraction_True_Comic_jpg_cbz/";
    TypeArchive type = TypeArchive::CBZ;
    int nb_images_par_pages = 1;
    Book book(type, nb_images_par_pages,inputFolder);

    std::atomic<bool> imageLoaded(false); // Indicateur de chargement d'image

    // Lancement du chargement asynchrone des images
    std::thread loadImageThread(loadImagesAsync, inputFolder, type, nb_images_par_pages, std::ref(book), std::ref(imageLoaded));

    // Attendre que le chargement soit terminé (ou un certain temps)
    while (!imageLoaded.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Attendre un court moment avant de vérifier à nouveau
    }

    // Si le chargement des images a réussi, afficher les images
    if (!book.ObtenirPages().empty()) {
        // Afficher les images
        QApplication app(argc, argv);
        FullScreenLabel label(book);
        label.showFullScreen();
        return app.exec();
    } else {
        std::cerr << "Aucune image chargée. Arrêt du programme." << std::endl;
        return 1;
    }
}
