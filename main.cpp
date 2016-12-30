#include <QApplication>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBrush>
#include <QDebug>
#include <QVector>

#include "card.h"
#include "carddeck.h"

void drawPins(QGraphicsScene *scene, CardStack &cardStack, int cardWidth, int cardHeight, int cardGap);
void drawBalls(QGraphicsScene *scene, CardStack &ballOneStack, CardStack &ballTwoStack, CardStack &ballThreeStack, int cardWidth, int cardHeight, int cardGap);
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

  CardDeck cardDeck;
  CardStack pinStack;
  CardStack ballOneStack;
  CardStack ballTwoStack;
  CardStack ballThreeStack;
  int maxSelection = 3;
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
  drawPins(&scene, pinStack, cardWidth, cardHeight, cardGap);
  drawBalls(&scene, ballOneStack, ballTwoStack, ballThreeStack, cardWidth, cardHeight, cardGap);
  //drawScoreBoard(&scene, cardWidth, cardHeight, cardGap);
  view.show();
  QList<Card*> savedPins;
  //set all cards in nonclicked initially
  for(int i = 0; i < pinStack.getStackCards().size(); i++){
    pinStack.appendNonSelectedCards(pinStack.getStackCard(i));
  }

  while(true){
    int selectedSize = pinStack.getSelectedCards().size();
    //go through all of selected, if clicked then goes to nonselected and highlight removed
    for(int i = 0; i < pinStack.getSelectedCards().size(); i++){
      Card* curCard = pinStack.getSelectedCard(i);
      if(curCard->getClicked()){
        pinStack.appendNonSelectedCards(curCard);
        pinStack.removeSelectedCards(i);
        QGraphicsItem* item = scene.itemAt(curCard->pos(), QTransform());
        if(item->type() != curCard->type()){
          scene.removeItem(item);
        }
        //clear clicked
        curCard->clearClicked();
      }
    }
    //go through all of the nonselected
    for(int i = 0; i < pinStack.getNonSelectedCards().size(); i++){
      //if clicked, check size of selected
      Card* curCard = pinStack.getNonSelectedCard(i);
      if(curCard->getClicked()){
        //if able to go to selected. move and give highlight
        if(selectedSize < maxSelection){
          pinStack.appendSelectedCards(curCard);
          pinStack.removeNonSelectedCards(i);
          scene.addRect(curCard->pos().x(), curCard->pos().y(), cardWidth, cardHeight, QPen(QColor("red")));
        }
        //clear clicked
        curCard->clearClicked();
      }
    }
    QCoreApplication::processEvents();
  }

  /*while(true){
    QList<Card*> selectedPins, selectedBalls, nonSelectedPins;
    for(int i = 0; i < pinStack.getStackSize(); i++){
      if(pinStack.getStackCard(i)->getSelected()){
        selectedPins.append(pinStack.getStackCard(i));
      }
      else{
        nonSelectedPins.append(pinStack.getStackCard(i));
      }
    }
    for(int i = 0; i < ballOneStack.getStackSize(); i++){
      if(ballOneStack.getStackCard(i)->getSelected()){
        selectedBalls.append(ballOneStack.getStackCard(i));
      }
    }
    for(int i = 0; i < ballTwoStack.getStackSize(); i++){
      if(ballTwoStack.getStackCard(i)->getSelected()){
        selectedBalls.append(ballTwoStack.getStackCard(i));
      }
    }
    for(int i = 0; i < ballThreeStack.getStackSize(); i++){
      if(ballThreeStack.getStackCard(i)->getSelected()){
        selectedBalls.append(ballThreeStack.getStackCard(i));
      }
    }
    //make sure only 3 pins are selected
    if(selectedPins.size() > 3){
      //have to reset the mSelected values of cards removed
      for(int i = 0; i < selectedPins.size(); i++){
        for(int j = 0; j < savedPins.size(); j++){
          //remove cards from savedPins from selected
          if(selectedPins.at(i) == savedPins.at(j)){
            selectedPins.removeAt(i);
          }
        }
      }
      //set mSelected back to false
      for(int i = 0; i < selectedPins.size(); i++){
        selectedPins.at(i)->setIsSelected(false);
      }
      //revert the selected to what they were
      selectedPins = savedPins;
    }
    else savedPins = selectedPins;
    int pinTotal = 0;
    for(int i = 0; i < selectedPins.size(); i++){
      Card* curCard = selectedPins.at(i);
      scene.addRect(curCard->pos().x(), curCard->pos().y(), cardWidth, cardHeight, QPen(QColor("red")));
      pinTotal += curCard->getValue();
    }
    int ballTotal = 0;
    for(int i = 0; i < selectedBalls.size(); i++){
      Card* curCard = selectedBalls.at(i);
      scene.addRect(curCard->pos().x(), curCard->pos().y(), cardWidth, cardHeight, QPen(QColor("blue")));
      ballTotal += curCard->getValue();
    }
    //qDebug() << scene.items();
    //qDebug() << pinTotal << ballTotal;
    //draw selected rectangles
    for(int i = 0; i < pinStack.getStackSize(); i++){
      Card* curCard = pinStack.getStackCard(i);
      QGraphicsItem* item = scene.itemAt(curCard->pos(), QTransform());
      if(item->type() ==  curCard->type() && curCard->getSelected()){
        scene.addRect(curCard->pos().x(), curCard->pos().y(), cardWidth, cardHeight, QPen(QColor("red")));
      }
    }
    for(int i = 0; i < selectedPins.size(); i++){
      Card* curCard = pinStack.getStackCard(i);
      QGraphicsItem* item = scene.itemAt(curCard->pos(), QTransform());
      if(item->type() != curCard->type() && !curCard->getSelected()){
        scene.removeItem(item);
      }
    }
    QCoreApplication::processEvents();
  }*/
  return app.exec();
}

void drawPins(QGraphicsScene *scene, CardStack &cardStack, int cardWidth, int cardHeight, int cardGap){
  int evenOffset = (cardWidth+3*cardGap)/2;
  int col1 = cardGap, col2 = evenOffset, col3 = 2*cardGap + cardWidth, col4 = evenOffset+cardWidth+cardGap;
  int col5 = 3*cardGap + 2*cardWidth, col6 = evenOffset+2*cardWidth+2*cardGap;
  int col7 = 4*cardGap + 3*cardWidth; //col8 = secondOffset+3*cardWidth+3*cardGap, col9 = 5*cardGap + 4*cardWidth;
  //QVector<int> col = {col0, col1, col2, col3, col4, col5, col6};
  int row1 = cardGap, row2 = 2*cardGap + cardHeight, row3 = 3*cardGap + 2*cardHeight;
  int row4 = 4*cardGap + 3*cardHeight;
  //QVector<int> row = {row0, row1, row2, row3};
  //QBrush cardFill(QColor(255,255,255));
  //first row
  /*scene->addRect(col1, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col3, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col5, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col7, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col9, row1, cardWidth, cardHeight, QPen(),cardFill);*/
  cardStack.getStackCard(0)->setPos(col1, row1);
  scene->addItem(cardStack.getStackCard(0));
  cardStack.getStackCard(1)->setPos(col3, row1);
  scene->addItem(cardStack.getStackCard(1));
  cardStack.getStackCard(2)->setPos(col5, row1);
  scene->addItem(cardStack.getStackCard(2));
  cardStack.getStackCard(3)->setPos(col7, row1);
  scene->addItem(cardStack.getStackCard(3));
  //second row
  /*scene->addRect(col2, row2, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col4, row2, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col6, row2, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col8, row2, cardWidth, cardHeight, QPen(),cardFill);*/
  cardStack.getStackCard(4)->setPos(col2, row2);
  scene->addItem(cardStack.getStackCard(4));
  cardStack.getStackCard(5)->setPos(col4, row2);
  scene->addItem(cardStack.getStackCard(5));
  cardStack.getStackCard(6)->setPos(col6, row2);
  scene->addItem(cardStack.getStackCard(6));

  //third row
  /*scene->addRect(col3, row3, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col5, row3, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col7, row3, cardWidth, cardHeight, QPen(),cardFill);*/
  cardStack.getStackCard(7)->setPos(col3, row3);
  scene->addItem(cardStack.getStackCard(7));
  cardStack.getStackCard(8)->setPos(col5, row3);
  scene->addItem(cardStack.getStackCard(8));
  //final row
  /*scene->addRect(col4, row4, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col6, row4, cardWidth, cardHeight, QPen(),cardFill);*/
  cardStack.getStackCard(9)->setPos(col4, row4);
  scene->addItem(cardStack.getStackCard(9));
}

void drawBalls(QGraphicsScene *scene, CardStack &ballOneStack, CardStack &ballTwoStack, CardStack &ballThreeStack, int cardWidth, int cardHeight, int cardGap){
  int row1 = 4*cardGap + 3*cardHeight, row2 = row1+cardHeight/3, row3 = row2+cardHeight/3, row4 = row3+cardHeight/3, row5 = row4+cardHeight/3;
  int col1 = scene->width()-3*cardGap-3*cardWidth, col2 =scene->width()-2*cardGap-2*cardWidth , col3 = scene->width()-cardGap-cardWidth;
  //QBrush cardFill(QColor(255,255,255));
  //first ball (5 cards)
  /*scene->addRect(col1, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col1, row2, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col1, row3, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col1, row4, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col1, row5, cardWidth, cardHeight, QPen(),cardFill);*/
  ballOneStack.getStackCard(0)->setPos(col1, row1);
  //ballOneStack.getStackCard(0)->setElementId("C:\\Users\\Jake\\Documents\\bowlingSolitaire\\images\\card_background_75x110.svg");
  scene->addItem(ballOneStack.getStackCard(0));
  ballOneStack.getStackCard(1)->setPos(col1, row2);
  scene->addItem(ballOneStack.getStackCard(1));
  ballOneStack.getStackCard(2)->setPos(col1, row3);
  scene->addItem(ballOneStack.getStackCard(2));
  ballOneStack.getStackCard(3)->setPos(col1, row4);
  scene->addItem(ballOneStack.getStackCard(3));
  ballOneStack.getStackCard(4)->setPos(col1, row5);
  scene->addItem(ballOneStack.getStackCard(4));
  //second ball (3 cards)
  /*scene->addRect(col2, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col2, row2, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col2, row3, cardWidth, cardHeight, QPen(),cardFill);*/
  ballTwoStack.getStackCard(0)->setPos(col2, row1);
  scene->addItem(ballTwoStack.getStackCard(0));
  ballTwoStack.getStackCard(1)->setPos(col2, row2);
  scene->addItem(ballTwoStack.getStackCard(1));
  ballTwoStack.getStackCard(2)->setPos(col2, row3);
  scene->addItem(ballTwoStack.getStackCard(2));
  //third ball (2 cards)
  /*scene->addRect(col3, row1, cardWidth, cardHeight, QPen(),cardFill);
  scene->addRect(col3, row2, cardWidth, cardHeight, QPen(),cardFill);*/
  ballThreeStack.getStackCard(0)->setPos(col3, row1);
  scene->addItem(ballThreeStack.getStackCard(0));
  ballThreeStack.getStackCard(1)->setPos(col3, row2);
  scene->addItem(ballThreeStack.getStackCard(1));
}

//void drawScoreBoard(QGraphicsScene *scene, int cardWidth, int cardHeight, int cardGap){}

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
