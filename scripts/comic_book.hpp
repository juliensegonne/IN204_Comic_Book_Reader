#ifndef COMICBOOK_HPP
#define COMICBOOK_HPP

#include <string>
#include <vector>

// Enumération pour les types d'archive
enum class TypeArchive {
    CBZ,
    CBR,
    CB7,
    CBT
};

// Enumération pour les formats d'image
enum class FormatImage {
    JPG,
    BMP,
    PNG
};

// Classe Image
class Image {
private:
    std::string nomFichier;
    std::vector<char> data;
    FormatImage format;

public:
    Image(const std::string& nomFichier);
    std::string ObtenirNomFichier() const;
    const std::vector<char>& ObtenirData() const;
    FormatImage ObtenirFormat() const;
    void ChargerFichier(const std::string& nomFichier);
};

// Classe Page
class Page {
private:
    std::vector<Image> images;

public:
    void AjouterImage(const Image& image) const;
    const std::vector<Image>& ObtenirImages() const;
};

// Classe Book
class Book {
private:
    std::vector<Page> pages;
    TypeArchive typeArch;
    int imagesParPage;

public:
    Book(TypeArchive typeArch,int imagesParPage);
    int ObtenirImagesParPage() const;
    void AjouterPage(const Page& page);
    const std::vector<Page>& ObtenirPages() const;
    TypeArchive ObtenirTypeArchive() const;
};

#endif