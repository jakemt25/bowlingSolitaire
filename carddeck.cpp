#include "carddeck.h"
#include "card.h"

#include <QDebug>
#include <time.h>


CardDeck::CardDeck()
{

}

CardDeck::~CardDeck()
{
}

void CardDeck::createCards(int numCards)
{
  for(int i = 1; i <= numCards; i++){
    mDeckCards.append(new Card(/*"",*/ i, Card::Club));
  }
  for(int i = 1; i <= numCards; i++){
    mDeckCards.append(new Card(/*"",*/ i, Card::Spade));
  }
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
}

void CardDeck::removeCard()
{
  mDeckCards.pop_front();
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
}

void CardStack::addToStack(Card* newCard)
{
  mStackCards.push_back(newCard);
}

void CardStack::removeFromStack()
{
  if(mStackCards.isEmpty());
  else
  {
    mStackCards.pop_back();
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
