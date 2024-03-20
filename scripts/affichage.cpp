#include "affichage.hpp"
#include <cmath>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>

FullScreenLabel::FullScreenLabel(Book& book, QWidget *parent)
    : QLabel(parent), bookRef(book), currentPageIndex(-1) {
    // On récupère les dimensions de l'écran
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->size();

    // On réduit la taille des images pour laisser de la place pour les boutons
    maxImageSize = screenSize / 1.06;

    Boutons();
    setFocus();
}

void FullScreenLabel::Boutons() {
    // Définition des boutons
    BoutonPremierePage = new QPushButton("Première Page", this);
    BoutonPagePrecedente = new QPushButton("Page Précédente", this);
    BoutonPageSuivante = new QPushButton("Page Suivante", this);
    BoutonDernierePage = new QPushButton("Dernière Page", this);
    BoutonChoixComic = new QPushButton("Choix Comic Book", this);
    BoutonChoixFiltre= new QPushButton("Choix Filtre", this);
    BoutonChangerNombreImagesParPage = new QPushButton("Choix Images/Page", this);

    // Créer une feuille de style pour les boutons
    QString styleSheet = "QPushButton {"
                         "    background-color: #1E90FF;" // Bleu foncé
                         "    border: 2px solid darkblue;"
                         "    border-radius: 5px;"
                         "    color: white;"
                         "    font-size: 20px;"
                         "    font-weight: bold;" // Mettre la police en gras
                         "}"
                         "QPushButton:hover {"
                         "    background-color: #4682B4;" // Bleu légèrement plus clair au survol
                         "}";

    // Appliquer la feuille de style à tous les boutons
    BoutonPremierePage->setStyleSheet(styleSheet);
    BoutonPagePrecedente->setStyleSheet(styleSheet);
    BoutonPageSuivante->setStyleSheet(styleSheet);
    BoutonDernierePage->setStyleSheet(styleSheet);
    BoutonChoixComic->setStyleSheet(styleSheet);
    BoutonChoixFiltre->setStyleSheet(styleSheet);
    BoutonChangerNombreImagesParPage->setStyleSheet(styleSheet);

    // Augmenter la taille de la police des boutons
    QFont PoliceBouton = BoutonPremierePage->font();
    PoliceBouton.setPointSize(20); // Taille de la police
    PoliceBouton.setWeight(QFont::Bold);
    BoutonPremierePage->setFont(PoliceBouton);
    BoutonPagePrecedente->setFont(PoliceBouton);
    BoutonPageSuivante->setFont(PoliceBouton);
    BoutonDernierePage->setFont(PoliceBouton);
    BoutonChoixComic->setFont(PoliceBouton);
    BoutonChoixFiltre->setFont(PoliceBouton);
    BoutonChangerNombreImagesParPage->setFont(PoliceBouton);

    // Taille des boutons
    int LargeurBouton = 260;
    int HauteurBouton = 40;
    int EspaceEntreBouton = 10;
    BoutonPremierePage->setFixedSize(LargeurBouton, HauteurBouton);
    BoutonPagePrecedente->setFixedSize(LargeurBouton, HauteurBouton);
    BoutonPageSuivante->setFixedSize(LargeurBouton, HauteurBouton);
    BoutonDernierePage->setFixedSize(LargeurBouton, HauteurBouton);
    BoutonChoixComic->setFixedSize(LargeurBouton, HauteurBouton);
    BoutonChoixFiltre->setFixedSize(LargeurBouton, HauteurBouton);
    BoutonChangerNombreImagesParPage->setFixedSize(LargeurBouton, HauteurBouton);

    // Récupérer la taille de l'écran
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // Calculer la position x du premier bouton pour centrer les boutons
    int LargeurTotaleBoutons = 7 * LargeurBouton; // La somme des largeurs des boutons
    int Espaces = 6 * EspaceEntreBouton; // 4 espaces entre les 5 boutons
    int XPremierBouton = (screenGeometry.width() - LargeurTotaleBoutons - Espaces) / 2;

    // Positionner les boutons
    BoutonPremierePage->move(XPremierBouton, screenGeometry.height() - HauteurBouton - 10);
    BoutonPagePrecedente->move(BoutonPremierePage->x() + BoutonPremierePage->width() + EspaceEntreBouton, screenGeometry.height() - HauteurBouton - 10);
    BoutonPageSuivante->move(BoutonPagePrecedente->x() + BoutonPagePrecedente->width() + EspaceEntreBouton, screenGeometry.height() - HauteurBouton - 10);
    BoutonDernierePage->move(BoutonPageSuivante->x() + BoutonPageSuivante->width() + EspaceEntreBouton, screenGeometry.height() - HauteurBouton - 10);
    BoutonChoixComic->move(BoutonDernierePage->x() + BoutonDernierePage->width() + EspaceEntreBouton, screenGeometry.height() - HauteurBouton - 10);
    BoutonChoixFiltre->move(BoutonChoixComic->x() + BoutonChoixComic->width() + EspaceEntreBouton, screenGeometry.height() - HauteurBouton - 10);
    BoutonChangerNombreImagesParPage->move(BoutonChoixFiltre->x() + BoutonChoixFiltre->width() + EspaceEntreBouton, screenGeometry.height() - HauteurBouton - 10);

    // On connecte les entités boutons à leur fonction
    connect(BoutonPremierePage, &QPushButton::clicked, this, [this]() { AllerPremierePage(); });
    connect(BoutonPagePrecedente, &QPushButton::clicked, this, [this]() { changePage(-1); });
    connect(BoutonPageSuivante, &QPushButton::clicked, this, [this]() { changePage(1); });
    connect(BoutonDernierePage, &QPushButton::clicked, this, [this]() { AllerDernierePage(); });
    connect(BoutonChoixComic, &QPushButton::clicked, this, [this]() { ChoixComicBook(); });
    connect(BoutonChoixFiltre, &QPushButton::clicked, this, [this]() { ChoixFiltre(); });
    connect(BoutonChangerNombreImagesParPage, &QPushButton::clicked, this,[this]() { ChangerNombreImagesParPage(); });
}

void FullScreenLabel::changePage(int delta) [[maybe_unused]] {
    currentPageIndex += delta; // Page actuelle qui doit être affichée
    currentPageIndex = std::max(0, std::min(currentPageIndex, static_cast<int>(bookRef.ObtenirPages().size()) - 1));
    afficherPageCourante();
    setFocus();
}

void FullScreenLabel::AllerPremierePage() {
    currentPageIndex = 0;
    afficherPageCourante();
    setFocus();
}

void FullScreenLabel::AllerDernierePage() {
    currentPageIndex = bookRef.ObtenirPages().size() - 1;
    afficherPageCourante();
    setFocus();
}

void FullScreenLabel::ChoixComicBook() {
    int nb_images_par_pages = bookRef.ObtenirImagesParPage();
    // Ouvrir une boîte de dialogue pour sélectionner un dossier contenant des comic books
    QString dossierSelectionne = QFileDialog::getExistingDirectory(this, "Choisir un dossier", "archives/extractions");

    if (!dossierSelectionne.isEmpty()) {
        // Récupérer les trois derniers caractères du nom du dossier sélectionné et les mettre en majuscules
        QString typeArchiveString = dossierSelectionne.right(3).toUpper();

        // Convertir la chaîne de caractères en enum TypeArchive
        TypeArchive typeArchive;
        if (typeArchiveString == "CBZ") {
            typeArchive = TypeArchive::CBZ;
        } else if (typeArchiveString == "CBR") {
            typeArchive = TypeArchive::CBR;
        } else if (typeArchiveString == "CB7") {
            typeArchive = TypeArchive::CB7;
        } else if (typeArchiveString == "CBT") {
            typeArchive = TypeArchive::CBT;
        } else {
            // Afficher un avertissement si le type d'archive n'est pas reconnu
            QMessageBox::warning(this, "Type d'archive non reconnu", "Le type d'archive correspondant aux trois derniers caractères du dossier sélectionné n'est pas reconnu.");
            return;
        }

        // Crée un nouveau livre à partir du dossier sélectionné
        bookRef = bookRef.ChargerComicBook(dossierSelectionne.toStdString(), nb_images_par_pages, typeArchive);
        // Si le livre chargé est vide, affiche un avertissement
        if (bookRef.ObtenirPages().empty()) {
            QMessageBox::warning(this, "Aucune image trouvée", "Aucune image au format trouvée dans le dossier sélectionné.");
            return;
        }

        // Réinitialiser l'affichage pour montrer la première page
        currentPageIndex = 0;
        afficherPageCourante();
    }
    setFocus();
}

void FullScreenLabel::ChoixFiltre() {
    // Ouvrir une boîte de dialogue pour permettre à l'utilisateur de choisir le filtre
    QStringList choixFiltres;
    choixFiltres << "Aucun Filtre" << "Filtre adapté au texte" << "Filtre adapté aux images" << "Filtre Lanczos"; // Liste des filtres disponibles, à remplacer par vos choix
    QString choix = QInputDialog::getItem(this, "Choisir un filtre", "Filtre :", choixFiltres, 0, false);

    // Mettre à jour le filtre actuel en fonction du choix de l'utilisateur
    if (choix == "Aucun Filtre") {
        filtreActuel = FiltreChoisi::AucunFiltre;
    } else if (choix == "Filtre adapté au texte") {
        filtreActuel = FiltreChoisi::FiltreBilineaire;
    } else if (choix == "Filtre adapté aux images") {
        filtreActuel = FiltreChoisi::FiltreConvolution;
    } else if (choix == "Filtre Lanczos") {
        filtreActuel = FiltreChoisi::FiltreLanczos;
    } 
    // Actualiser l'affichage avec le nouveau filtre choisi
    afficherPageCourante();
    setFocus();
}

void FullScreenLabel::ChangerNombreImagesParPage() {
    // Ouvrir une boîte de dialogue pour choisir le nombre d'images par page
    QStringList choixNombreImagesParPage;
    choixNombreImagesParPage << "1 Image/Page" << "2 Images/Page";
    QString choix = QInputDialog::getItem(this, "Choisir le nombre d'images par page", "Nombre d'images par page :", choixNombreImagesParPage, 0, false);

    // Mettre à jour le nombre d'images par page
    if (choix == "1 Image/Page") {
        bookRef.ChangerNombreImagesParPage(1);
    } else if (choix == "2 Images/Page") {
        bookRef.ChangerNombreImagesParPage(2);
    }
    bookRef = bookRef.ChargerComicBook(bookRef.ObtenirCheminImages(), bookRef.ObtenirImagesParPage(), bookRef.ObtenirTypeArchive());
    afficherPageCourante();
    setFocus();
}

// QImage FullScreenLabel::zoomImage(const QImage& image, qreal scaleFactor) {
//     // Crée une nouvelle QImage pour stocker l'image zoomée
//     QImage zoomed(image.size() * scaleFactor, QImage::Format_RGB32);
//     // Initialise un QPainter pour dessiner sur l'image zoomée
//     QPainter painter(&zoomed);
//     // Effectue la transformation de zoom sur l'image originale
//     painter.scale(scaleFactor, scaleFactor);
//     // Dessine l'image originale sur l'image zoomée
//     painter.drawImage(0, 0, image);
//     return zoomed;
// }

// void FullScreenLabel::zoom_fct(qreal zoomFactor) {
//     const auto& pages = bookRef.ObtenirPages();
//     // Vérifie que l'indice de page courante ne dépasse pas le nombre total de pages
//     if (currentPageIndex < static_cast<int>(pages.size())) {
//         // Récupère les informations de la page à afficher
//         const auto& page = pages[currentPageIndex];
//         const auto& images = page.ObtenirImages();
//         QPixmap pagePixmap;

//         // Créer une image vide pour la page
//         QImage pageImage(size(), QImage::Format_RGB32);
//         QPainter painter(&pageImage);

//         // Réinitialiser le fond de l'image en blanc pour ne plus afficher l'image précédente
//         painter.fillRect(pageImage.rect(), Qt::white);

//         // Calculer la largeur totale des images
//         int totalImageWidth = 0;
//         for (const auto& image : images) {
//             QImage img(QString::fromStdString(image.ObtenirNomFichier()));
//             QSize scaledSize = img.size().scaled(maxImageSize, Qt::KeepAspectRatio);
//             totalImageWidth += scaledSize.width();
//         }

//         // Calculer la position x pour centrer les images
//         int dim = (pageImage.width() - totalImageWidth) / 2;

//         // Afficher les images de la page actuelle
//         for (const auto& image : images) {
//             // Charger l'image
//             QImage img(QString::fromStdString(image.ObtenirNomFichier()));

//             // Redimensionner l'image pour qu'elle tienne dans la taille maximale définie
//             QSize scaledSize = img.size().scaled(maxImageSize, Qt::KeepAspectRatio);
//             // img = resizeWithLanczos(img, scaledSize.width(), scaledSize.height());

//             // Dessiner l'image sur la pageImage à la position calculée
//             painter.drawImage(QPoint(dim, 0), img.scaled(scaledSize));

//             // Mettre à jour la position horizontale pour la prochaine image
//             dim += scaledSize.width();
//         }

//         // Appliquer le zoom sur une copie de l'image originale
//         QImage zoomedPageImage = zoomImage(pageImage, zoomFactor);

//         // Convertir l'image en QPixmap et l'afficher
//         pagePixmap = QPixmap::fromImage(zoomedPageImage);
//         setPixmap(pagePixmap);
//     }
// }

void FullScreenLabel::keyPressEvent(QKeyEvent *event) {
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

void FullScreenLabel::mousePressEvent(QMouseEvent *event) {
    // Récupérer les coordonnées x du clic de souris
    int mouseX = event->pos().x();

    // Récupérer la largeur de l'écran
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();

    // Si le clic est sur la partie gauche de l'écran (en dehors des boutons)
    if (mouseX < screenWidth / 2) {
        // Passer à la page précédente
        changePage(-1);
    }
    else {
        // Passer à la page suivante
        changePage(1);
    }
}

void FullScreenLabel::EvenementMolette(QWheelEvent  *event) {
    // Vérifier si la souris est au-dessus de la zone d'affichage
    if (rect().contains(event->position().toPoint())) {
        // Récupérer le facteur de zoom en fonction du déplacement de la molette
        double zoomFactor = (event->angleDelta().y() > 0) ? 1.1 : 1/1.1;
        // zoom_fct(zoomFactor);
        std::cout << "Mon zoom =" << zoomFactor << std::endl;
    }
}

// filtre de Lanczos
double lanczos(double x) {
    if (std::abs(x) < 1e-16) return 1.0;
    if (std::abs(x) > 3.0) return 0.0;
    return std::sin(M_PI * x) * std::sin(M_PI * x / 3.0) / (M_PI * M_PI * x * x);
}

// Redimensionnement image avec le filtre de Lanczos
QImage FiltreLanczos(const QImage& image, int newWidth, int newHeight) {
    QImage resizedImage(newWidth, newHeight, image.format());

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            double srcX = static_cast<double>(x) / newWidth * image.width();
            double srcY = static_cast<double>(y) / newHeight * image.height();

            double r = 0.0, g = 0.0, b = 0.0;
            double totalWeight = 0.0;

            for (int j = std::floor(srcY) - 2; j <= std::floor(srcY) + 2; ++j) {
                for (int i = std::floor(srcX) - 2; i <= std::floor(srcX) + 2; ++i) {
                    if (i >= 0 && i < image.width() && j >= 0 && j < image.height()) {
                        double weight = lanczos(srcX - i) * lanczos(srcY - j);
                        totalWeight += weight;

                        QRgb pixel = image.pixel(i, j);
                        r += qRed(pixel) * weight;
                        g += qGreen(pixel) * weight;
                        b += qBlue(pixel) * weight;
                    }
                }
            }

            r = std::max(0.0, std::min(255.0, r / totalWeight));
            g = std::max(0.0, std::min(255.0, g / totalWeight));
            b = std::max(0.0, std::min(255.0, b / totalWeight));

            resizedImage.setPixel(x, y, qRgb(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b)));
        }
    }

    return resizedImage;
}

// Redimensionnement image avec interpolation bilinéaire
QImage FiltreBilinear(const QImage& image, int newWidth, int newHeight) {
    QImage resizedImage(newWidth, newHeight, image.format());

    // Facteurs de conversion entre les dimensions de l'image originale et de la nouvelle image
    double widthRatio = static_cast<double>(image.width() - 1) / newWidth;
    double heightRatio = static_cast<double>(image.height() - 1) / newHeight;

    // Parcourir tous les pixels de la nouvelle image
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            // Coordonnées des pixels dans l'image originale
            double srcX = x * widthRatio;
            double srcY = y * heightRatio;

            // Coordonnées des pixels voisins dans l'image originale
            int x0 = static_cast<int>(srcX);
            int y0 = static_cast<int>(srcY);
            int x1 = std::min(x0 + 1, image.width() - 1);
            int y1 = std::min(y0 + 1, image.height() - 1);

            // Coefficients de pondération pour l'interpolation bilinéaire
            double dx = srcX - x0;
            double dy = srcY - y0;
            double w00 = (1.0 - dx) * (1.0 - dy);
            double w01 = (1.0 - dx) * dy;
            double w10 = dx * (1.0 - dy);
            double w11 = dx * dy;

            // Couleurs des pixels voisins
            QRgb pixel00 = image.pixel(x0, y0);
            QRgb pixel01 = image.pixel(x0, y1);
            QRgb pixel10 = image.pixel(x1, y0);
            QRgb pixel11 = image.pixel(x1, y1);

            // Calcul de la couleur interpolée
            int r = static_cast<int>(qRed(pixel00) * w00 + qRed(pixel01) * w01 +
                                     qRed(pixel10) * w10 + qRed(pixel11) * w11);
            int g = static_cast<int>(qGreen(pixel00) * w00 + qGreen(pixel01) * w01 +
                                     qGreen(pixel10) * w10 + qGreen(pixel11) * w11);
            int b = static_cast<int>(qBlue(pixel00) * w00 + qBlue(pixel01) * w01 +
                                     qBlue(pixel10) * w10 + qBlue(pixel11) * w11);

            // Définition du pixel dans l'image redimensionnée
            resizedImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    return resizedImage;
}

// Redimensionnement image avec un filtre de convolution pour améliorer la qualité
QImage FiltreConvolution(const QImage& image, int newWidth, int newHeight) {
    // Matrice de convolution pour le lissage
    static const std::vector<std::vector<double>> convolutionMatrix = {
        {1.0 / 16, 2.0 / 16, 1.0 / 16},
        {2.0 / 16, 4.0 / 16, 2.0 / 16},
        {1.0 / 16, 2.0 / 16, 1.0 / 16}
    };

    QImage resizedImage = image.scaled(newWidth, newHeight, Qt::KeepAspectRatio);

    // Parcourir tous les pixels de la nouvelle image
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            // Appliquer le filtre de convolution au pixel (x, y)
            double r = 0.0, g = 0.0, b = 0.0;

            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int srcX = std::min(std::max(x + j, 0), image.width() - 1);
                    int srcY = std::min(std::max(y + i, 0), image.height() - 1);

                    QRgb pixel = image.pixel(srcX, srcY);
                    double weight = convolutionMatrix[i + 1][j + 1]; // Indices corrects pour la matrice de convolution

                    r += qRed(pixel) * weight;
                    g += qGreen(pixel) * weight;
                    b += qBlue(pixel) * weight;
                }
            }

            r = std::max(0.0, std::min(255.0, r));
            g = std::max(0.0, std::min(255.0, g));
            b = std::max(0.0, std::min(255.0, b));

            // Définition du pixel dans l'image redimensionnée
            resizedImage.setPixelColor(x, y, QColor(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b)));
        }
    }

    return resizedImage;
}


void FullScreenLabel::afficherPageCourante() {
    const auto& pages = bookRef.ObtenirPages();
    // Vérifie que l'indice de page courante ne dépasse pas le nombre total de pages
    if (currentPageIndex < static_cast<int>(pages.size())) {
        // Récupère les informations de la page à afficher
        const auto& page = pages[currentPageIndex];
        const auto& images = page.ObtenirImages();
        QPixmap pagePixmap;
        // Créer une image vide pour la page
        QImage pageImage(size(), QImage::Format_RGB32);
        QPainter painter(&pageImage);

        // Réinitialiser le fond de l'image en blanc pour ne plus afficher l'image précédente
        painter.fillRect(pageImage.rect(), Qt::white);
        // Calculer la largeur totale des images
        int totalImageWidth = 0;
        for (const auto& image : images) {
            QImage img(QString::fromStdString(image.ObtenirNomFichier()));
            QSize scaledSize = img.size().scaled(maxImageSize, Qt::KeepAspectRatio);
            totalImageWidth += scaledSize.width();
        }
        // Calculer la position x pour centrer les images
        int dim = (pageImage.width() - totalImageWidth) / 2;

        // Afficher les images de la page actuelle avec une taille adaptée à l'écran
        for (const auto& image : images) {
            // Charger l'image
            QImage img(QString::fromStdString(image.ObtenirNomFichier()));
            // Redimensionner l'image pour s'adapter à la taille de l'écran
            QSize scaledSize = img.size().scaled(maxImageSize, Qt::KeepAspectRatio);
            if (filtreActuel == FiltreChoisi::AucunFiltre) {
                img = img.scaled(scaledSize);
            } else if (filtreActuel == FiltreChoisi::FiltreBilineaire) {
                img = FiltreBilinear(img, scaledSize.width(), scaledSize.height());
            } else if (filtreActuel == FiltreChoisi::FiltreConvolution) {
                img = FiltreConvolution(img, scaledSize.width(), scaledSize.height());
            } else if (filtreActuel == FiltreChoisi::FiltreLanczos) {
                img = FiltreLanczos(img, scaledSize.width(), scaledSize.height());
            }
            // Dessiner l'image sur la pageImage à la position calculée
            painter.drawImage(QPoint(dim, 0), img.scaled(scaledSize));

            // Mettre à jour la position horizontale pour la prochaine image
            dim += scaledSize.width();
        }

        // Convertir l'image en QPixmap et l'afficher
        pagePixmap = QPixmap::fromImage(pageImage);
        setPixmap(pagePixmap);
    }
}