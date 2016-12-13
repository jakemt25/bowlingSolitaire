#ifndef CARDDECK_H
#define CARDDECK_H

#include <QList>
#include <QObject>

#include "card.h"

class Card;

class CardDeck : public QObject
{
  Q_OBJECT

public:
  //Functions
  void createCards(int numCards);
  void printDeck();
  void addCard(Card* newCard);
  void removeCard();
  void shuffleCards(int numShuffles);
  Card* getDeckCard(int i){return mDeckCards.at(i);}

  CardDeck();
  ~CardDeck();

private:
  QList<Card*> mDeckCards;

};

class CardStack: public CardDeck
{
  Q_OBJECT

public:
  //Functions
  void createStack(CardDeck &cardDeck, int cardsDrawn);
  void addToStack(Card* newCard);
  void removeFromStack();
  void printStack();
  Card* getStackCard(int i){return mStackCards.at(i);}

  CardStack();
  ~CardStack();

private:
  QList<Card*> mStackCards;
};

#endif // CARDDECK_H
