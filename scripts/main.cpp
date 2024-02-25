#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <Magick++.h>
#include "comic_book.hpp"

#include <QApplication>
#include <QLabel>
#include <QKeyEvent>
#include <QScreen>
#include <QDesktopWidget>
#include <QPainter>

using namespace std::filesystem;

// Définition de la fenêtre affichée
class FullScreenLabel : public QLabel {
private:
    int currentPageIndex;
    const Book& bookRef;

public:
    FullScreenLabel(const Book& book, QWidget *parent = nullptr) 
        : QLabel(parent), bookRef(book), currentPageIndex(-1) {}

    void changePage(int delta) {
        currentPageIndex += delta; // Page actuelle qui doit être affichée
        currentPageIndex = std::max(0, std::min(currentPageIndex, static_cast<int>(bookRef.ObtenirPages().size()) - 1));
        afficherPageCourante();
    }

protected:
    // Pour gérer les événements au clavier
    void keyPressEvent(QKeyEvent *event) override {
        switch (event->key()) {
            case Qt::Key_Escape:
                qApp->exit();
                break;
            case Qt::Key_Left:
                changePage(-1);
                break;
            case Qt::Key_Right:
                changePage(1);
                break;
            default:
                QLabel::keyPressEvent(event);
        }
    }

private:
    void afficherPageCourante() {
        const auto& pages = bookRef.ObtenirPages();
        // Vérifie que l'indice de page courante ne dépasse pas le nombre total de pages
        if (currentPageIndex < static_cast<int>(pages.size())) {
            // Récupère les informations de la page à afficher
            const auto& page = pages[currentPageIndex];
            const auto& images = page.ObtenirImages();
            QPixmap pagePixmap;

            QSize maxSize = size(); //dimensions de la fenêtre
            maxSize.scale(maxSize.width(), maxSize.height(), Qt::KeepAspectRatio);// la taille maximale est modifiée 
            //pour s'adapter à la largeur et à la hauteur actuelles de la fenêtre

            QImage pageImage(maxSize, QImage::Format_RGB32);// Crée une nouvelle image
            QPainter painter(&pageImage); // Objet qui permet d'écrire sur pageImage

            // Réinitialiser le fond de l'image en blanc pour ne plus afficher l'image affichée précédemment
            painter.fillRect(pageImage.rect(), Qt::white);

            // Afficher les images de la page actuelle
            int décalage_horizontal = 0;
            for (const auto& image : images) {
                // Charge l'image et la stocke dans img
                QImage img(QString::fromStdString(image.ObtenirNomFichier()));

                // Redimensionne l'image à la taille maximale disponible dans la fenêtre
                QSize scaledSize = img.size().scaled(maxSize, Qt::KeepAspectRatio);
                img = img.scaled(scaledSize);

                // Dessiner l'image sur img
                QRect targetRect(décalage_horizontal, 0, scaledSize.width(), scaledSize.height()); 
                painter.drawImage(targetRect, img);
                décalage_horizontal += scaledSize.width();
            }

            // Convertir l'image de la page en QPixmap et l'afficher
            pagePixmap = QPixmap::fromImage(pageImage);
            setPixmap(pagePixmap);
        }
    }
};

int main(int argc, char *argv[]) {
    int nb_images_par_pages = 2;
    // Initialisation d'un Book avec nb_images_par_page
    Book book(TypeArchive::CBZ, nb_images_par_pages);

    // Chemin du dossier contenant les images
    std::string cheminImages = "archives/extractions/extraction_archive_jpg_cbz/";

    // Pour stocker les noms d'images
    std::vector<std::string> nomsImages;

    // Parcourir les images dans le dossier et stocker les noms des images
    for (const auto& entry : boost::filesystem::directory_iterator(cheminImages)) {
        // Vérifie si le type correspond à un fichier régulier 
        if (boost::filesystem::status(entry).type() == boost::filesystem::file_type::regular_file) {
            std::string nomFichier = entry.path().string(); // Convertit en chaine de caractères
            nomsImages.push_back(nomFichier);
        }
    }

    // Tri des noms des images par ordre alphabétique
    std::sort(nomsImages.begin(), nomsImages.end());

    // Ajouter les images stockées dans le book
    for (const auto& nomFichier : nomsImages) {
        Image nouvelleImage(nomFichier);

        // Ajouter une nouvelle page si le vecteur de pages est vide
        if (book.ObtenirPages().empty()) {
            Page nouvellePage;
            nouvellePage.AjouterImage(nouvelleImage);
            book.AjouterPage(nouvellePage);
        } else {
            // Ajouter l'image à la dernière page
            const auto& dernierePage = book.ObtenirPages().back();
            // Si la dernière page n'est pas pleine
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

    // Afficher la composition du book dans le terminal
    const auto& pages = book.ObtenirPages();
    for (size_t i = 0; i < pages.size(); ++i) {
        std::cout << "Page " << i + 1 << ":" << std::endl;
        const auto& images = pages[i].ObtenirImages();
        for (size_t j = 0; j < images.size(); ++j) {
            std::cout << "    Image " << j + 1 << ": " << images[j].ObtenirNomFichier() << std::endl;
        }
    }

    // Affichage fenêtre
    QApplication app(argc, argv);
    QSize screenSize = qApp->primaryScreen()->geometry().size(); // taille de l'écran
    FullScreenLabel label(book); // Initialisation de la fenêtre
    label.setFixedSize(screenSize); // fixe la taille de la fenêtre à celle de l'écran
    label.setAlignment(Qt::AlignCenter); // Centre la fenêtre
    label.showFullScreen();

    return app.exec();
}