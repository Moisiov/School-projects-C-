#ifndef AIHAND_HH
#define AIHAND_HH
#include <QFrame>
#include <QStackedLayout>
#include <vector>

#include "card.hh"
class AIHand : public QFrame
{
public:
    AIHand(QWidget* parent = 0);
    void clearDeck();
    void turnCard();
    std::vector<int> getCardData();
public slots:
    bool addCard(Card* card);
private:
    QStackedLayout* layout_;  // Sisältää ne kortti-widgetit, jotka vastustajan kädessä on.
};

#endif // AIHAND_HH
