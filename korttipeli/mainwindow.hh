#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QPushButton>
#include "deck.hh"
#include "opendeck.hh"
#include "gamerules.hh"
#include "aihand.hh"
#include "cardslot.hh"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void ready();
    void initializeGame();

private:
    void setupLayout();
    void gameEnd();

    // Talletetaan attribuuteiksi ne widgetit, joita saattaa tarvita käsitellä pelin aikana.
    // Ohjelmakoodipohjassa näitä ei käytetä mihinkään.
    Deck* deck_;
    //OpenDeck* pickedCards_;
    std::vector<OpenDeck*> pickedCards_;
    std::vector<AIHand*> aiCards_;

    QLabel* textLabel_;
    OpenDeck* dumpCards_;
    QPushButton* readyButton_;
    QPushButton* newGameButton_;
    int drawCount_ = 3;
};

#endif // MAINWINDOW_HH
