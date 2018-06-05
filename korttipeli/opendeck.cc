#include "opendeck.hh"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>
#include <QLabel>

OpenDeck::OpenDeck(bool oneCard, QWidget *parent):
    QFrame(parent),
    layout_(new QStackedLayout(this)),
    oneCardOnly_(oneCard)
{
    setMinimumSize(180, 260);
    setMaximumSize(180, 260);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setLayout(layout_);
    setAcceptDrops(true);
}

bool OpenDeck::addCard(Card *card)
{
    Card *cardInHand = static_cast<Card*>(layout_->currentWidget());
    if(cardInHand == nullptr || oneCardOnly_ == false) {
        layout()->addWidget(card);
        layout_->setCurrentWidget(card);
        card->turn();
        return true;
    }
    return false;
}

// Suoritetaan, kun avoimen pakan päällimmäinen kortti raahataan jonnekin.
void OpenDeck::mousePressEvent(QMouseEvent *event)
{
    // 1) Metodin alkuosa suoritetaan, kun avopakkaa klikataan.

    // Otetaan pakan sisältämä kortti osoittimen päähän.
    Card *card = static_cast<Card*>(layout_->currentWidget());

    // Tyhjästä pakasta ei voi raahata, eli lopetetaan.
    if (card == nullptr || gameEnd_ == true){
        return;
    }

    // Otetaan raahattavan kortin kuva valmiiksi muuttujaan pixmap.
    QPixmap pixmap = *card->getCurrentSideLabel()->pixmap();

    // Luodaan mimeData-olio, jonka avulla raahattavan kortin tiedot (=merkkijono)
    // välitetään sinne, minne kortti raahataan.
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(QString::fromStdString(card->getCardData()));

    // Luodaan drag-olio, jonka avulla raahaaminen tapahtuu. Drag-oliolle annetaan
    // raahauksen aikana näytettävä kuva, sekä tiedot raahattavasta oliosta.
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - card->pos());  // Se kohta raahattavasta kuvasta, jonka pitää osua kohteeseen.

    // Asetetaan harmautettu korttikuva väliaikaiseksi korttikuvaksi raahaamisen ajaksi.
    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();
    card->getCurrentSideLabel()->setPixmap(tempPixmap);

    // 2) Aloitetaan raahaus ja tarkastellaa onnistuiko.

    if (drag->exec( Qt::MoveAction) == Qt::MoveAction) {
        // Tämä suoritetaan, jos raahaus 4,2,1onnistui.
        card->getCurrentSideLabel()->setPixmap(pixmap);
        layout_->removeWidget(layout_->currentWidget());
        layout_->setCurrentIndex(layout_->count()-1);
        card->setAttribute(Qt::WA_DeleteOnClose);
        card->close();

    } else {
        // Tämä suoritetaan, jos raahaus epäonnistui.
        card->show();
        card->getCurrentSideLabel()->setPixmap(pixmap);
    }
}

// Suoritetaan, kun jotakin raahataan tämän CardSlotin päälle.
void OpenDeck::dragEnterEvent(QDragEnterEvent *event)
{
    // Jos haluat testailla ohjelmaa niin, että näet konsolista, koska tämä
    // suoritetaan, poista kommenttimerkit seuraavalta riviltä.
    //std::cout << "CardSlot::dragEnterEvent" << std::endl;

    if (event->mimeData()->hasFormat("text/plain")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();

    } else {
        event->ignore();
    }
}

// Suoritetaan, kun jotakin liikutetaan tämän CardSlotin rajojen sisällä.
void OpenDeck::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

// Suoritetaan, kun jotain pudotetaan cardslotiin (raahauksen päättyessä).
void OpenDeck::dropEvent(QDropEvent *event)
{
    // Tarkastetaan, onko sisältääkö pudotettu elementti tekstiä (korttidataa) ja
    // onko raahauksen aloituspaikka eri kuin pudotuspaikka.
    if (event->mimeData()->hasFormat("text/plain") && event->source() != this ) {

        // Cardslotilla on funktio-osoitin checkFunction_, jonka osoittama funktio
        // tarkastaa, voiko pelisääntöjen mukaan pudotettavan/t kortin/t pudottaa slotiin.
        // Ko. funktiolle annetaan parametrina kaksi stringiä, joista ensimmäinen
        // sisältää korttipaikassa päälimmäisenä olevan kortin tiedot ja toinen
        // lisättävistä korteista sen tiedot, joka olisi tulossa päälimmäisenä
        // olevan kortin päälle.

        // Pyydetään eventiltä tiedot siitä, mitä kortteja ollaan pudottamassa.
        //QStringList newCardData = event->mimeData()->text().split(";");

        // Muodostetaan QStringList-olio, johon tulevat tiedot slotissa olevista korteista.
        /*QStringList existingCardsData;
        if (topCard_ != nullptr){
            existingCardsData = QString::fromStdString(topCard_->getCardData()).split(";");
        } else {
            existingCardsData.append("");
        }*/

        QStringList splitted = event->mimeData()->text().split(",");
        QString value = splitted.at(0);
        QString suit = splitted.at(1);

        Card* card = new Card(static_cast<CardSuit> (suit.toUInt()), value.toUInt(), this);

        // Tarkastetaan checkFunction_ -osoittimen päässä olevaa funktiota käyttäen, onko
        // pudotus sallittu pelisääntöjen mukaan.
        if(addCard(card)){
            // Jos pudotus halutaan hyväksyä, lisätään raahatut kortit tähän slotiin.
            event->acceptProposedAction();
        }
        else {
            // Jos pudotusta ei haluta hyväksyä, niin se hylätään näin.
            event->ignore();
        }
    } else {
        event->ignore();
    }
}

void OpenDeck::endGame()
{
    gameEnd_ = true;
}

void OpenDeck::clearDeck()
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
    gameEnd_ = false;
}

std::vector<int> OpenDeck::getCardData()
{
    Card* card = static_cast<Card*>(layout_->currentWidget());
    std::string data = card->getCardData();
    std::vector<int> card_data = {};
    std::string::size_type start = 0;

    for(int i = 0; i < 2; ++i) {
        std::string::size_type end = data.find(",", start);
        int value = stoi(data.substr(start, end));
        card_data.push_back(value);
        start = end+1;
    }

    return card_data;
}
