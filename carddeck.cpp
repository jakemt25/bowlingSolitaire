#include "carddeck.h"
#include "card.h"

#include <QDebug>
#include <QDir>
#include <time.h>


CardDeck::CardDeck()
  :mDeckSize(0)
{

}

CardDeck::~CardDeck()
{
}

void CardDeck::createCards(int numCards)
{
  //qDebug() << QDir::current().path();
  for(int i = 1; i <= numCards; i++){
    mDeckCards.append(new Card(QString(":/images/clubs/Club_%1_75x110.svg").arg(i), i, Card::Club));
  }
  for(int i = 1; i <= numCards; i++){
    mDeckCards.append(new Card(QString(":/images/spade/Spade_%1_75x110.svg").arg(i), i, Card::Spade));
  }
  mDeckSize = 2*numCards;
}

void CardDeck::printDeck()
{
  qDebug() << "Printing deck";
  for(int i = 0; i < mDeckCards.size(); i++){
    qDebug() << mDeckCards.at(i)-> getValue() << " " << mDeckCards.at(i)->getSuite();
  }
}

void CardDeck::addCard(Card *newCard)
{
  mDeckCards.append(newCard);
  mDeckSize += 1;
}

void CardDeck::removeCard()
{
  mDeckCards.pop_front();
  mDeckSize -= 1;
}

void CardDeck::shuffleCards(int numShuffles)
{
  qsrand(time(NULL));
  for(int i = 0; i < numShuffles; i++){
    mDeckCards.swap(qrand()%mDeckCards.size(), qrand()%mDeckCards.size());
  }
}

/*
 *
 *
*/

CardStack::CardStack()
  : mStackSize(0)
{

}

CardStack::~CardStack()
{

}


void CardStack::createStack(CardDeck &cardDeck, int cardsDrawn)
{
  int i = 0;
  for(i; i < cardsDrawn; i++)
  {
    mStackCards.append(cardDeck.getDeckCard(i));
  }
  for(i = 0; i < cardsDrawn; i++)
  {
    cardDeck.removeCard();
  }
  mStackSize = cardsDrawn;
}

void CardStack::addToStack(Card* newCard)
{
  mStackCards.push_back(newCard);
  mStackSize += 1;
}

void CardStack::removeFromStack()
{
  if(mStackCards.isEmpty());
  else
  {
    mStackCards.pop_back();
    mStackSize -= 1;
  }
}



void CardStack::printStack()
{
  qDebug() << "Printing the stack";
  for(int i = 0; i < mStackCards.size(); i++)
  {
    qDebug() << mStackCards[i]->getValue() << " " << mStackCards[i]->getSuite();
  }
}
