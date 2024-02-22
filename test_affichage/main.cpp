#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Charger l'image
    QPixmap image("ApacheTrail_n1_p01.jpg");

    // Créer un QLabel pour afficher l'image
    QLabel label;
    label.setPixmap(image);

    // Affichage fenêtre
    label.show();

    return app.exec();
}
