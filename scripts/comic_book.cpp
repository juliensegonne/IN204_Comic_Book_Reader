#include "comic_book.hpp"
#include <fstream>
#include <stdexcept>
#include <boost/filesystem.hpp>

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

Book::Book(TypeArchive typeArch, int imagesParPage)
    : typeArch(typeArch), imagesParPage(imagesParPage) {}

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
