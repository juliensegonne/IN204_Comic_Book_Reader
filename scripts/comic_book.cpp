#include "comic_book.hpp"
#include <fstream>
#include <stdexcept>

// Construceur de Image
Image::Image(const std::string& nomFichier) : nomFichier(nomFichier) {
    ChargerFichier(nomFichier);
}

// Classe image
void Image::ChargerFichier(const std::string& nomFichier) {
    // Ouvre le fichier image en mode binaire
    std::ifstream file(nomFichier, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible de charger l'image.");
    }

    // Stocke le contenu de file dans data
    data.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

    // Détermine le format de l'image
    size_t pos = nomFichier.find_last_of('.');// Position du dernier point dans le nom du fichier
    if (pos != std::string::npos) {
        std::string extension = nomFichier.substr(pos + 1);// Extension désigne la partie après le point
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

// Classe Page
void Page::AjouterImage(const Image& image) {
    images.push_back(image);
}

const std::vector<Image>& Page::ObtenirImages() const {
    return images;
}


Book::Book(TypeArchive typeArch) : typeArch(typeArch) {}

void Book::AjouterPage(const Page& page) {
    pages.push_back(page);
}

const std::vector<Page>& Book::ObtenirPages() const {
    return pages;
}

TypeArchive Book::ObtenirTypeArchive() const {
    return typeArch;
}