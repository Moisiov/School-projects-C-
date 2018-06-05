#include "aihand.hh"

#include <QMimeData>
#include <QPainter>
#include <QLabel>

AIHand::AIHand(QWidget *parent):
    QFrame(parent),
    layout_(new QStackedLayout(this))
{
    setMinimumSize(180, 260);
    setMaximumSize(180, 260);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setLayout(layout_);
}

bool AIHand::addCard(Card *card)
{
    Card *cardInHand = static_cast<Card*>(layout_->currentWidget());
    if(cardInHand == nullptr) {
        layout()->addWidget(card);
        layout_->setCurrentWidget(card);
        return true;
    }
    return false;
}

void AIHand::clearDeck()
{
    while(true) {
        Card *card = static_cast<Card*>(layout_->currentWidget());
        if(card != nullptr) {
            layout_->removeWidget(layout_->currentWidget());
            layout_->setCurrentIndex(layout_->count()-1);
            card->setAttribute(Qt::WA_DeleteOnClose);
            card->close();
        } else {
            break;
        }
    }
}

void AIHand::turnCard()
{
    Card *card = static_cast<Card*>(layout_->currentWidget());
    card->turn();
}

std::vector<int> AIHand::getCardData()
{
    Card *card = static_cast<Card*>(layout_->currentWidget());
    std::string data = card->getCardData();
    std::vector<int> card_data = {};
    std::string::size_type start = 0;

    for(int i = 0; i < 2; ++i) {
        std::string::size_type end = data.find(",", start);
        int value = stoi(data.substr(start, end));
        card_data.push_back(value);
        start = end + 1;
    }

    return card_data;
}
