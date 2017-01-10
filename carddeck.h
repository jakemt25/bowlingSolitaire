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
  void updateSelectedCards();
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
  void createStack(CardDeck &cardDeck, int cardsDrawn, enum Card::WhichStack whichStack);
  void addToStack(Card* newCard);
  void removeFromStack();
  void printStack();

  QList<Card*> getStackCards(){ return mStackCards;}
  Card* getStackCard(int i){return mStackCards.at(i);}

  QList<Card*> getSelectedCards(){ return mSelectedCards;}
  Card* getSelectedCard(int i){return mSelectedCards.at(i);}
  void appendSelectedCards(Card* card){ mSelectedCards.append(card);}
  void removeSelectedCards(int i){ mSelectedCards.removeAt(i);}

  QList<Card*> getNonSelectedCards(){ return mNonSelectedCards;}
  Card* getNonSelectedCard(int i){return mNonSelectedCards.at(i);}
  void appendNonSelectedCards(Card* card){ mNonSelectedCards.append(card);}
  void removeNonSelectedCards(int i){ mNonSelectedCards.removeAt(i);}

  CardStack();
  ~CardStack();

private:
  QList<Card*> mStackCards;
  QList<Card*> mSelectedCards;
  QList<Card*> mNonSelectedCards;
};

class myButton : public QGraphicsRectItem
{
public:
  void myButton::mousePressEvent(QGraphicsSceneMouseEvent *event){ mIsClicked = true;}
  bool mIsClicked;

  myButton(int x, int y, int h, int w);
  ~myButton();
};

#endif // CARDDECK_H
