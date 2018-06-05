#ifndef OPENDECK_HH
#define OPENDECK_HH
#include <QFrame>
#include <string>
#include <list>
#include <vector>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

#include <QStackedLayout>

#include "card.hh"
class OpenDeck : public QFrame
{
public:
    OpenDeck(bool oneCard, QWidget* parent = 0);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void endGame();
    void clearDeck();
    std::vector<int> getCardData();
public slots:
    bool addCard(Card* card);
protected:
    void mousePressEvent(QMouseEvent *event);
private:
    QStackedLayout* layout_;  // Sisältää ne kortti-widgetit, jotka avopakassa on.
    bool oneCardOnly_;
    bool gameEnd_ = false;
};

#endif // OPENDECK_HH
