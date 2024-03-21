#ifndef COMICBOOK_HPP
#define COMICBOOK_HPP

#include <string>
#include <vector>
#include <memory>

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
    std::string cheminImages;

public:
    Book(TypeArchive typeArch, int imagesParPage, const std::string& cheminImages);
    int ObtenirImagesParPage() const;
    void AjouterPage(const Page& page);
    const std::vector<Page>& ObtenirPages() const;
    TypeArchive ObtenirTypeArchive() const;
    Book ChargerComicBook(const std::string& cheminImages,int nb_images_par_pages, TypeArchive typeArch);
    void ChangerNombreImagesParPage(int nombreImagesParPage);
    const std::string& ObtenirCheminImages() const;
};

void ChargementAsynchrone(const std::string& inputFolder, const TypeArchive type, int nb_images_par_pages, Book& book, std::atomic<bool>& imageLoaded);
#endif
