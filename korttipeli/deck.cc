#include "deck.hh"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>
#include <QLabel>

Deck::Deck(QWidget *parent) : QFrame(parent)
{
    setMinimumSize(180, 260);
    setMaximumSize(180, 260);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);

    initializeDeck();
    shuffle();
}

void Deck::initializeDeck()
{
    cards_ = {};
    for (int suit = SPADE; suit != SUIT_COUNT; ++suit){

        for (unsigned value = 2; value <= 14; ++value){
            auto card = new Card(static_cast<CardSuit>(suit), value, this);
            int xOffset = (this->width() - card->width()) / 2;
            int yOffset = (this->height() - card->height()) / 2;
            card->move(xOffset, yOffset);
            cards_.push_back(card);
        }
    }
}

void Deck::shuffle()
{
    std::vector<Card*> shuffledDeck = {};
    unsigned int deckSize = cards_.size();
    for (unsigned int i = 0; i < deckSize; ++i){
        unsigned int card = rand() % deckSize;
        shuffledDeck.push_back(cards_.at(card));
        cards_.erase(cards_.begin()+card);
        --deckSize;
    }
    cards_ = shuffledDeck;
}

Card* Deck::pickCard()
{
    if (cards_.empty()){
        return nullptr;
    } else {
        Card* card = cards_.back();
        cards_.pop_back();
        return card;
    }
}

void Deck::dealCards(std::vector<OpenDeck*> playerHand,
                      std::vector<AIHand*> aiHand)
{
    for(int i = 0; i < 5; ++i) {
        Card* card = cards_.back();
        if(aiHand.at(i)->addCard(card)) {
            cards_.pop_back();
            card = cards_.back();
        }
        if(playerHand.at(i)->addCard(card)) {
            cards_.pop_back();
        }
    }
}

// Suoritetaan, kun pakkaa klikataan.
void Deck::mousePressEvent(QMouseEvent *event)
{
    // Ohjelma ei saa kaatua tyhjän pakan klikkaukseen.
    if (cards_.empty()){
        return;
    }

    // Otetaan osoittimen päähän pakan päällimmäinen kortti ja poistetaan se pakasta.
    Card* card = cards_.back();
    cards_.pop_back();

    // Ohjelma ei saa kaatua, jos korttia ei olekaan.
    if (!card){
        return;
    }

    // Ilmoitetaan, että pakasta otettiin kortti.
    emit cardPicked(card);

    // Hyväksytään klikkaus, koska se käsitellään aina tässä metodissa.
    event->accept();
}
