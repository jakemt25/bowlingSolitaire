#include <QApplication>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBrush>
#include <QDebug>

#include "card.h"
#include "carddeck.h"

void drawPins(QGraphicsScene *scene, int cardWidth, int cardHeight, int cardGap);
void drawBalls(QGraphicsScene *scene, int cardWidth, int cardHeight, int cardGap);
void drawScoreBoard(QGraphicsScene *scen, int cardWidth, int cardHeight, int cardGape);
void setPinTouchedCards(CardStack &cardStack);
void printPinTouchedCards(CardStack &cardStack);

int main(int argc, char **argv)
{
  QApplication app (argc, argv);
  int cardWidth = 75, cardHeight = 110, cardGap = 10;
  QGraphicsScene scene(0, 0, 800, 995);
  //scene.setSceneRect(0, 0, 2000, 2000);
  QGraphicsView view(&scene);
  view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  view.setBackgroundBrush(QColor(0,255, 0));
  view.setWindowTitle("Bowling Solitaire");
  view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view.setFixedSize(800, 1000);
  drawPins(&scene, cardWidth, cardHeight, cardGap);
  drawBalls(&scene, cardWidth, cardHeight, cardGap);
  drawScoreBoard(&scene, cardWidth, cardHeight, cardGap);
  view.show();
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
  //printPinTouchedCards(pinStack);
  //pinStack.printStack();
  ballOneStack.createStack(cardDeck, 5);
  //ballOneStack.printStack();
  ballTwoStack.createStack(cardDeck, 3);
  //ballTwoStack.printStack();
  ballThreeStack.createStack(cardDeck, 2);
  //ballThreeStack.printStack();
  return app.exec();
}

void drawPins(QGraphicsScene *scene, int cardWidth, int cardHeight, int cardGap){
  int secondOffset = (cardWidth+3*cardGap)/2;
  int col1 = cardGap, col2 = secondOffset, col3 = 2*cardGap + cardWidth, col4 = secondOffset+cardWidth+cardGap;
  int col5 = 3*cardGap + 2*cardWidth, col6 = secondOffset+2*cardWidth+2*cardGap;
  int col7 = 4*cardGap + 3*cardWidth, col8 = secondOffset+3*cardWidth+3*cardGap, col9 = 5*cardGap + 4*cardWidth;
  int row1 = cardGap, row2 = 2*cardGap + cardHeight, row3 = 3*cardGap + 2*cardHeight;
  int row4 = 4*cardGap + 3*cardHeight, row5 = 5*cardGap + 4*cardHeight;
  QBrush cardFill(QColor(255,255,255));
  //first row
  scene->addRect(col1, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col3, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col5, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col7, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col9, row1, cardWidth, cardHeight, QPen(),cardFill);
  //second row
  scene->addRect(col2, row2, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col4, row2, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col6, row2, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col8, row2, cardWidth, cardHeight, QPen(),cardFill);
  //third row
  scene->addRect(col3, row3, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col5, row3, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col7, row3, cardWidth, cardHeight, QPen(),cardFill);
  //fourth row
  scene->addRect(col4, row4, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col6, row4, cardWidth, cardHeight, QPen(),cardFill);
  //final row
  scene->addRect(col5, row5, cardWidth, cardHeight, QPen(),cardFill);
}

void drawBalls(QGraphicsScene *scene, int cardWidth, int cardHeight, int cardGap){
  int row1 = 5*cardGap + 4*cardHeight, row2 = row1+2*cardGap, row3 = row2+2*cardGap, row4 = row3+2*cardGap, row5 = row4+2*cardGap;
  int col1 = scene->width()-3*cardGap-3*cardWidth, col2 =scene->width()-2*cardGap-2*cardWidth , col3 = scene->width()-cardGap-cardWidth;
  QBrush cardFill(QColor(255,255,255));
  //first ball (5 cards)
  QGraphicsRectItem* card11 = new QGraphicsRectItem(col1, row1, cardWidth, cardHeight);
  scene->addRect(col1, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col1, row2, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col1, row3, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col1, row4, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col1, row5, cardWidth, cardHeight, QPen(),cardFill);
  //second ball (3 cards)
  scene->addRect(col2, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col2, row2, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col2, row3, cardWidth, cardHeight, QPen(),cardFill);
  //third ball (2 cards)
  scene->addRect(col3, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col3, row2, cardWidth, cardHeight, QPen(),cardFill);

}

void drawScoreBoard(QGraphicsScene *scene, int cardWidth, int cardHeight, int cardGap){

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
