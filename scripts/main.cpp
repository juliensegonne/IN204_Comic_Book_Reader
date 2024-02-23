#include "comic_book.hpp"
#include <iostream>
#include <boost/filesystem.hpp>
#include <QApplication>
#include <QLabel>
#include <QKeyEvent>
#include <QScreen>
#include <QDesktopWidget>
#include <Magick++.h>
#include <string>
#include <QPainter> // Ajout de l'inclusion de QPainter

using namespace std::filesystem;

// Permet de quitter la fenêtre en appuyant sur Echap
class FullScreenLabel : public QLabel {
private:
    int currentPageIndex;
    const Book& bookRef;

public:
    FullScreenLabel(const Book& book, QWidget *parent = nullptr) 
        : QLabel(parent), bookRef(book), currentPageIndex(0) {}

    void changePage(int delta) {
        currentPageIndex += delta;
        currentPageIndex = std::max(0, std::min(currentPageIndex, static_cast<int>(bookRef.ObtenirPages().size()) - 1));
        afficherPageCourante();
    }

protected:
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
        if (currentPageIndex < static_cast<int>(pages.size())) {
            const auto& page = pages[currentPageIndex];
            const auto& images = page.ObtenirImages();
            QPixmap pagePixmap;

            // Calculer la taille maximale pour afficher toutes les images
            QSize maxSize = size();
            maxSize.scale(maxSize.width(), maxSize.height(), Qt::KeepAspectRatio);

            // Afficher les images de la page actuelle
            int imageIndex = 0;
            for (const auto& image : images) {
                QImage img(QString::fromStdString(image.ObtenirNomFichier()));
                // Redimensionner l'image pour qu'elle s'adapte à la taille maximale disponible dans la fenêtre
                QSize scaledSize = img.size().scaled(maxSize, Qt::KeepAspectRatio);
                pagePixmap = QPixmap::fromImage(img.scaled(scaledSize));
                // Afficher l'image ici
                QRect targetRect(imageIndex * maxSize.width(), 0, maxSize.width(), maxSize.height());
                QPainter painter(&pagePixmap); // Créer l'objet QPainter ici
                painter.drawImage(targetRect, img.scaled(scaledSize));
                setPixmap(pagePixmap);
                imageIndex++;
            }
        }
    }
};

int main(int argc, char *argv[]) {
    int nb_images_par_pages = 2;
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
    
    // Affichage fenêtre
    QApplication app(argc, argv);
    QSize screenSize = qApp->primaryScreen()->geometry().size();
    FullScreenLabel label(book);
    label.setFixedSize(screenSize);
    label.setAlignment(Qt::AlignCenter);
    label.showFullScreen();

    return app.exec();
}
