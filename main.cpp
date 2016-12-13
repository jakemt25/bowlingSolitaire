#include <QApplication>
#include <QPushButton>
#include <QDebug>

#include "card.h"
#include "carddeck.h"

void setPinTouchedCards(CardStack &cardStack);
void printPinTouchedCards(CardStack &cardStack);

int main(int argc, char **argv)
{
  QApplication app (argc, argv);
  CardDeck cardDeck;
  CardStack pinStack;
  CardStack ballOneStack;
  CardStack ballTwoStack;
  CardStack ballThreeStack;
  cardDeck.createCards(10);
  //cardDeck.printDeck();
  cardDeck.shuffleCards(100);
  //cardDeck.printDeck();
  pinStack.createStack(cardDeck, 10);
  setPinTouchedCards(pinStack);
  printPinTouchedCards(pinStack);
  //pinStack.printStack();
  ballOneStack.createStack(cardDeck, 5);
  //ballOneStack.printStack();
  ballTwoStack.createStack(cardDeck, 3);
  //ballTwoStack.printStack();
  ballThreeStack.createStack(cardDeck, 2);
  //ballThreeStack.printStack();
  return app.exec();
}


void setPinTouchedCards(CardStack &pinStack){
  pinStack.getStackCard(0)->setTouchedCards({1, 2});
  pinStack.getStackCard(1)->setTouchedCards({0, 2, 3, 4});
  pinStack.getStackCard(2)->setTouchedCards({0, 1, 4, 5});
  pinStack.getStackCard(3)->setTouchedCards({1, 4, 6, 7});
  pinStack.getStackCard(4)->setTouchedCards({1, 2, 3, 5, 7, 8});
  pinStack.getStackCard(5)->setTouchedCards({2, 4, 8, 9});
  pinStack.getStackCard(6)->setTouchedCards({3, 7});
  pinStack.getStackCard(7)->setTouchedCards({3, 4, 6, 8});
  pinStack.getStackCard(8)->setTouchedCards({4, 5, 7, 9});
  pinStack.getStackCard(9)->setTouchedCards({5, 8});
}

void printPinTouchedCards(CardStack &pinStack){
  pinStack.getStackCard(0)->printTouchedCards();
  pinStack.getStackCard(1)->printTouchedCards();
  pinStack.getStackCard(2)->printTouchedCards();
  pinStack.getStackCard(3)->printTouchedCards();
  pinStack.getStackCard(4)->printTouchedCards();
  pinStack.getStackCard(5)->printTouchedCards();
  pinStack.getStackCard(6)->printTouchedCards();
  pinStack.getStackCard(7)->printTouchedCards();
  pinStack.getStackCard(8)->printTouchedCards();
  pinStack.getStackCard(9)->printTouchedCards();
}
