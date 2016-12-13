#ifndef CARDSTACK_H
#define CARDSTACK_H

#include <QObject>
#include "carddeck.h"

class CardStack : public QObject
{
  Q_OBJECT

public:
  //Functions
  void createStack(QList<Card*> cardDeck, int cardsDrawn);
  void addToStack(Card* newCard);
  void removeFromStack();
  void printStack();

  CardStack();
  ~CardStack();

private:
  QList<Card*> stackCards;

};

#endif // CARDSTACK_H
