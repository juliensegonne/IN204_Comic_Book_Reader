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

class FullScreenLabel : public QLabel {
    Q_OBJECT

private:
    int currentPageIndex;
    Book& bookRef;
    QPushButton *BoutonPremierePage;
    QPushButton *BoutonPagePrecedente;
    QPushButton *BoutonPageSuivante;
    QPushButton *BoutonDernierePage;
    QPushButton *BoutonChoixComic;
    QSize maxImageSize;

public:
    FullScreenLabel(Book& book, QWidget *parent = nullptr);
    void Boutons();
    void changePage(int delta);
    void AllerPremierePage();
    void AllerDernierePage();
    void ChoixComicBook();

protected:
    void keyPressEvent(QKeyEvent *event) override [[maybe_unused]];
    void afficherPageCourante(Book& bookRef);

};

#endif // AFFICHAGE_HPP
