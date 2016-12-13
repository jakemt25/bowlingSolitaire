#include <QDebug>

#include "cardstack.h"
#include "carddeck.h"
#include "card.h"

CardStack::CardStack()
{

}

CardStack::~CardStack()
{

}


void CardStack::createStack(QList<Card*> cardDeck, int cardsDrawn)
{
  int i = 0;
  for(i; i < cardsDrawn; i++)
  {
    stackCards.append(cardDeck.at(i));
  }

}

void CardStack::addToStack(Card* newCard)
{
  stackCards.push_back(newCard);
}

void CardStack::removeFromStack()
{
  if(stackCards.isEmpty());
  else
  {
    stackCards.pop_back();
  }
}


void CardStack::printStack()
{
  qDebug() << "Printing the stack";
  for(int i = 0; i < stackCards.size(); i++)
  {
    qDebug() << stackCards[i]->getValue() << " " << stackCards[i]->getSuite();
  }
}
