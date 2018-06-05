#ifndef DECK_HH
#define DECK_HH

#include <vector>
#include <QFrame>
#include "card.hh"
#include "aihand.hh"
#include "opendeck.hh"

class Deck : public QFrame
{
    Q_OBJECT

public:
    Deck(QWidget *parent = 0);

    void dealCards(std::vector<OpenDeck*> playerHand,
                   std::vector<AIHand*> aiHand);

    // Korttipakasta voi nostaa päällimmäisen kortin.
    Card* pickCard();
    void initializeDeck();
    void shuffle();

    // TODO: Lisää tähän uusia toimintoja, joita korttipakalle voi suorittaa.

signals:
    // Signaali, jonka pakka lähettää, kun kortti nostetaan.
    void cardPicked(Card* card);

protected:
    // Itse toteutettu Qt:n widgetin klikkaamiseen liittyvä metodi.
    void mousePressEvent(QMouseEvent *event);

private:
    // Korttipakan sisältämät kortit.
    std::vector<Card*> cards_;

};

#endif // DECK_HH
