#include "affichage.hpp"

FullScreenLabel::FullScreenLabel(const Book& book, QWidget *parent)
    : QLabel(parent), bookRef(book), currentPageIndex(-1) {
    // On récupère les dimensions de l'écran
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->size();

    // On réduit la taille des images pour laisser de la place pour les boutons
    maxImageSize = screenSize / 1.06;

    Boutons();
}

void FullScreenLabel::Boutons() {
    // Définition des boutons
    BoutonPremierePage = new QPushButton("Première Page", this);
    BoutonPagePrecedente = new QPushButton("Page Précédente", this);
    BoutonPageSuivante = new QPushButton("Page Suivante", this);
    BoutonDernierePage = new QPushButton("Dernière Page", this);
    BoutonChoixComic = new QPushButton("Choix Comic Book", this);

    // Augmenter la taille de la police des boutons
    QFont PoliceBouton = BoutonPremierePage->font();
    PoliceBouton.setPointSize(20); // Taille de la police
    BoutonPremierePage->setFont(PoliceBouton);
    BoutonPagePrecedente->setFont(PoliceBouton);
    BoutonPageSuivante->setFont(PoliceBouton);
    BoutonDernierePage->setFont(PoliceBouton);
    BoutonChoixComic->setFont(PoliceBouton);

    // Taille des boutons
    int LargeurBouton = 260;
    int HauteurBouton = 40;
    int EspaceEntreBouton = 10;
    BoutonPremierePage->setFixedSize(LargeurBouton, HauteurBouton);
    BoutonPagePrecedente->setFixedSize(LargeurBouton, HauteurBouton);
    BoutonPageSuivante->setFixedSize(LargeurBouton, HauteurBouton);
    BoutonDernierePage->setFixedSize(LargeurBouton, HauteurBouton);
    BoutonChoixComic->setFixedSize(LargeurBouton, HauteurBouton);

    // Récupérer la taille de l'écran
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // Calculer la position x du premier bouton pour centrer les boutons
    int LargeurTotaleBoutons = 5 * LargeurBouton; // La somme des largeurs des boutons
    int Espaces = 4 * EspaceEntreBouton; // 4 espaces entre les 5 boutons
    int XPremierBouton = (screenGeometry.width() - LargeurTotaleBoutons - Espaces) / 2;

    // Positionner les boutons
    BoutonPremierePage->move(XPremierBouton, screenGeometry.height() - HauteurBouton - 10);
    BoutonPagePrecedente->move(BoutonPremierePage->x() + BoutonPremierePage->width() + EspaceEntreBouton, screenGeometry.height() - HauteurBouton - 10);
    BoutonPageSuivante->move(BoutonPagePrecedente->x() + BoutonPagePrecedente->width() + EspaceEntreBouton, screenGeometry.height() - HauteurBouton - 10);
    BoutonDernierePage->move(BoutonPageSuivante->x() + BoutonPageSuivante->width() + EspaceEntreBouton, screenGeometry.height() - HauteurBouton - 10);
    BoutonChoixComic->move(BoutonDernierePage->x() + BoutonDernierePage->width() + EspaceEntreBouton, screenGeometry.height() - HauteurBouton - 10);

    // On connecte les entités boutons à leur fonction
    connect(BoutonPremierePage, &QPushButton::clicked, this, [this]() { AllerPremierePage(); });
    connect(BoutonPagePrecedente, &QPushButton::clicked, this, [this]() { changePage(-1); });
    connect(BoutonPageSuivante, &QPushButton::clicked, this, [this]() { changePage(1); });
    connect(BoutonDernierePage, &QPushButton::clicked, this, [this]() { AllerDernierePage(); });
    connect(BoutonChoixComic, &QPushButton::clicked, this, [this]() { ChoixComicBook(); });
}

void FullScreenLabel::changePage(int delta) [[maybe_unused]] {
    currentPageIndex += delta; // Page actuelle qui doit être affichée
    currentPageIndex = std::max(0, std::min(currentPageIndex, static_cast<int>(bookRef.ObtenirPages().size()) - 1));
    afficherPageCourante();
}

void FullScreenLabel::AllerPremierePage() {
    currentPageIndex = 0;
    afficherPageCourante();
}

void FullScreenLabel::AllerDernierePage() {
    currentPageIndex = bookRef.ObtenirPages().size() - 1;
    afficherPageCourante();
}

void FullScreenLabel::ChoixComicBook() {
    // A compléter
}

void FullScreenLabel::keyPressEvent(QKeyEvent *event) [[maybe_unused]] {
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

        // Afficher les images de la page actuelle
        for (const auto& image : images) {
            // Charger l'image
            QImage img(QString::fromStdString(image.ObtenirNomFichier()));

            // Redimensionner l'image pour qu'elle tienne dans la taille maximale définie
            QSize scaledSize = img.size().scaled(maxImageSize, Qt::KeepAspectRatio);

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
