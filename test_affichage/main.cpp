#include <QApplication>
#include <QLabel>
#include <QKeyEvent>
#include <QScreen>
#include <QDesktopWidget>
#include <Magick++.h>
#include <string>


//permet de quitter la fenêtre en appuyant sur Echap
class FullScreenLabel : public QLabel {
public:
    FullScreenLabel(QWidget *parent = nullptr) : QLabel(parent) {}

protected:
    void keyPressEvent(QKeyEvent *event) override {
        // Si la touche "Echap" est pressée, ferme l'application
        if (event->key() == Qt::Key_Escape) {
            qApp->exit();
        }
    }
};

//permet de coller 2 images côte à côte pour former une double-page (le principe pourra servir plus tard à
//coller les images pour former une page)
int association(Magick::Image image1, Magick::Image image2, const std::string& sortie){
    
    int width1 = image1.columns();
    int width2 = image2.columns();
    int height1 = image1.rows();
    int height2 = image2.rows();

    // longueur et largeur de l'image finale
    int finalWidth = width1 + width2;
    int finalHeight = std::max(height1, height2);

    // image vide qui contiendra la sortie
    Magick::Image finalImage(Magick::Geometry(finalWidth, finalHeight), "white");

    // 1ere image à gauche
    finalImage.composite(image1, 0, 0, Magick::OverCompositeOp);

    // 2ème image à droite
    finalImage.composite(image2, width1, 0, Magick::OverCompositeOp);

    // Enregistrer l'image finale
    finalImage.write(sortie+".jpg");

    return 0;
}




int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QSize screenSize = qApp->primaryScreen()->geometry().size();
    int screenWidth = screenSize.width(); // Largeur de l'écran
    int screenHeight = screenSize.height(); // Hauteur de l'écran

    Magick::InitializeMagick(*argv);

    Magick::Image img1("ApacheTrail_n1_p01.jpg");
    Magick::Image img2("ApacheTrail_n1_p02gs.jpg");
    
    association(img1,img2,"sortie");  //peut-être il faut la modifier pour prendre en argument la taille de l'écran

    Magick::Image double_page("sortie.jpg");
    
    double_page.scale(std::to_string(screenWidth)+"x"+std::to_string(screenHeight));
    double_page.write("result.jpg");

    // Charger l'image
    QPixmap image("result.jpg");

    // Créer un QLabel pour afficher l'image
    FullScreenLabel label;
    label.setPixmap(image);
    label.setFixedSize(screenSize);
    label.setAlignment(Qt::AlignCenter);

    // Affichage fenêtre
    label.showFullScreen();

    return app.exec();
}
