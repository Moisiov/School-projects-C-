#include <QFrame>
#include <QHBoxLayout>
#include <QString>

#include "mainwindow.hh"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setupLayout();
    connect(readyButton_, SIGNAL(clicked(bool)), this, SLOT(ready()));
    connect(newGameButton_, SIGNAL(clicked(bool)), this, SLOT(initializeGame()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::ready()
{
    deck_->dealCards(pickedCards_, aiCards_);
    dumpCards_->clearDeck();
    --drawCount_;
    if(drawCount_ == 0) {
        gameEnd();
    } else {
        QString draws = QString::number(drawCount_);
        QString labelText = "Draws left: " + draws;
        textLabel_->setText(labelText);
    }
}

void MainWindow::initializeGame()
{
    deck_->initializeDeck();
    deck_->shuffle();
    for(int i = 0; i < 5; ++i) {
        pickedCards_.at(i)->clearDeck();
        aiCards_.at(i)->clearDeck();
    }
    dumpCards_->clearDeck();
    drawCount_ = 3;
    readyButton_->setEnabled(true);
    QString draws = QString::number(drawCount_);
    QString labelText = "Draws left: " + draws;
    textLabel_->setText(labelText);
}

void MainWindow::setupLayout()
{
    // Pääikkunan isoimpana rakenteena on frame, joka sisältää...
    QFrame* frame = new QFrame;
    QVBoxLayout* frameLayout = new QVBoxLayout(frame);

    // ... alarivin, keskirivin ja ylärivin.
    QHBoxLayout* bottomRowLayout = new QHBoxLayout();
    QHBoxLayout* middleRowLayout = new QHBoxLayout();
    QHBoxLayout* topRowLayout = new QHBoxLayout();
    frameLayout->addLayout(topRowLayout);
    frameLayout->addLayout(middleRowLayout);
    frameLayout->addLayout(bottomRowLayout);

    // Luodaan pakkaoliot.
    deck_ = new Deck(this);
    //pickedCards_ = new OpenDeck(this);
    // Luodaan oliot pelaajan ja tekoälyn korteille.
    for(int i = 0; i < 5; ++i) {
        pickedCards_.push_back(new OpenDeck(true));
        aiCards_.push_back(new AIHand);
    }

    // Lisätään keskiriville suljettu pakka.
    middleRowLayout->addWidget(deck_);
    //topRowLayout->addWidget(pickedCards_);
    // Lisätään yläriville tekoälyn käsi ja alariville pelaajan käsi...
    for(int i = 0; i < 5; ++i) {
        bottomRowLayout->addWidget(pickedCards_.at(i));
        topRowLayout->addWidget(aiCards_.at(i));
    }

    // ... ja keskiriville tekstilabel, pushbutton ja cardslot.
    textLabel_ = new QLabel(this);
    textLabel_->setText("Super basic poker");
    middleRowLayout->addWidget(textLabel_);

    // Lisätään keskiriville buttonlayout.
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    middleRowLayout->addLayout(buttonLayout);

    newGameButton_ = new QPushButton(this);
    readyButton_ = new QPushButton(this);
    newGameButton_->setText("New game");
    readyButton_->setText("Deal cards");
    readyButton_->setEnabled(false);
    buttonLayout->addWidget(newGameButton_);
    buttonLayout->addWidget(readyButton_);

    dumpCards_ = new OpenDeck(false);
    middleRowLayout->addWidget(dumpCards_);
    setCentralWidget(frame);
}

void MainWindow::gameEnd()
{
    readyButton_->setEnabled(false);

    std::vector<std::vector<int>> playerHandData = {};
    std::vector<std::vector<int>> aiHandData = {};
    for(int i = 0; i < 5; ++i) {
        pickedCards_.at(i)->endGame();
        aiCards_.at(i)->turnCard();
        playerHandData.push_back(pickedCards_.at(i)->getCardData());
        aiHandData.push_back(aiCards_.at(i)->getCardData());
    }

    int winner = GameRules::getWinner(playerHandData, aiHandData);

    if(winner == 0) {
        textLabel_->setText("You lost. Better luck next time! \n"
                            "Press \"New game\" to play again.");
    } else if(winner == 1) {
        textLabel_->setText("Congratulations! You won! \n"
                            "Press \"New game\" to play again.");
    } else if(winner == 2) {
        textLabel_->setText("That was close! It's a draw! \n"
                            "Press \"New game\" to play again.");
    }
}
