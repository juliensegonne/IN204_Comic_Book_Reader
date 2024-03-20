#ifndef AFFICHAGE_HPP
#define AFFICHAGE_HPP

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>
#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include "comic_book.hpp"
#include <thread>

enum class FiltreChoisi {
    AucunFiltre,
    FiltreBilineaire,
    FiltreConvolution
};

class FullScreenLabel : public QLabel {
    Q_OBJECT

private:
    int currentPageIndex;
    Book& bookRef;
    FiltreChoisi filtreActuel;
    QPushButton *BoutonPremierePage;
    QPushButton *BoutonPagePrecedente;
    QPushButton *BoutonPageSuivante;
    QPushButton *BoutonDernierePage;
    QPushButton *BoutonChoixComic;
    QPushButton* BoutonChoixFiltre; 
    QPushButton* BoutonChangerNombreImagesParPage;
    QSize maxImageSize;

public:
    FullScreenLabel(Book& book, QWidget *parent = nullptr);
    void Boutons();
    void changePage(int delta);
    void AllerPremierePage();
    void AllerDernierePage();
    void ChoixComicBook();
    void ChoixFiltre();
    void ChangerNombreImagesParPage();

protected:
    void keyPressEvent(QKeyEvent *event) override [[maybe_unused]];
    void mousePressEvent(QMouseEvent *event);
    void afficherPageCourante();

};

#endif // AFFICHAGE_HPP
