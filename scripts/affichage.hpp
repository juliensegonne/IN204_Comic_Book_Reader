#ifndef AFFICHAGE_HPP
#define AFFICHAGE_HPP

#include <iostream>
#include <cmath>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>
#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include <QGraphicsView>
#include "comic_book.hpp"

enum class FiltreChoisi {
    AucunFiltre,
    FiltreBilineaire,
    FiltreConvolution,
    FiltreLanczos
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
    void zoom_fct(qreal scaleFactor);

public:
    FullScreenLabel(Book& book, QWidget *parent = nullptr);
    void Boutons();
    void changePage(int delta);
    void AllerPremierePage();
    void AllerDernierePage();
    void ChoixComicBook();
    void ChoixFiltre();
    void ChangerNombreImagesParPage();
    QImage zoomImage(const QImage& image, qreal scaleFactor);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void EvenementMolette(QWheelEvent *event);
    void afficherPageCourante();
};

QImage FiltreLanczos(const QImage& image, int newWidth, int newHeight);
QImage FiltreBilinear(const QImage& image, int newWidth, int newHeight);
QImage FiltreConvolution(const QImage& image, int newWidth, int newHeight);

#endif // AFFICHAGE_HPP
