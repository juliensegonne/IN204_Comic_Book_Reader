#include "comic_book.hpp"
#include <fstream>
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <QFileDialog>
#include <iostream>

namespace fs = std::filesystem;

Image::Image(const std::string& nomFichier) : nomFichier(nomFichier) {
    ChargerFichier(nomFichier);
}

void Image::ChargerFichier(const std::string& nomFichier) {
    std::ifstream file(nomFichier, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible de charger l'image.");
    }

    data.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

    size_t pos = nomFichier.find_last_of('.');
    if (pos != std::string::npos) {
        std::string extension = nomFichier.substr(pos + 1);
        if (extension == "jpg" || extension == "jpeg") {
            format = FormatImage::JPG;
        } else if (extension == "png") {
            format = FormatImage::PNG;
        } else if (extension == "bmp") {
            format = FormatImage::BMP;
        }
    }
}

std::string Image::ObtenirNomFichier() const {
    return nomFichier;
}

const std::vector<char>& Image::ObtenirData() const {
    return data;
}

FormatImage Image::ObtenirFormat() const {
    return format;
}

void Page::AjouterImage(const Image& image) const {
    const_cast<std::vector<Image>&>(images).push_back(image);
}

const std::vector<Image>& Page::ObtenirImages() const {
    return images;
}

Book::Book(TypeArchive typeArch, int imagesParPage, const std::string& cheminImages)
    : typeArch(typeArch), imagesParPage(imagesParPage), cheminImages(cheminImages) {}

int Book::ObtenirImagesParPage() const { 
    return imagesParPage; 
}

void Book::AjouterPage(const Page& page) {
    pages.push_back(page);
}

const std::vector<Page>& Book::ObtenirPages() const {
    return pages;
}

TypeArchive Book::ObtenirTypeArchive() const {
    return typeArch;
}

Book Book::ChargerComicBook(const std::string& cheminImages,int nb_images_par_pages, TypeArchive typeArch) {
    // Initialisation du book avec le bon nombre d'images par page
    Book book(typeArch, nb_images_par_pages, cheminImages);

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
    return book;
}

void Book::ChangerNombreImagesParPage(int nombreImagesParPage) {
    imagesParPage = nombreImagesParPage;
}

const std::string& Book::ObtenirCheminImages() const {
    return cheminImages; 
}