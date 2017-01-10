#include <QApplication>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBrush>
#include <QDebug>
#include <QVector>
#include <QPushButton>

#include "card.h"
#include "carddeck.h"
#include "gamestate.h"

void clearScene(QGraphicsScene *scene, CardStack &pinStack,  CardStack &ballOneStack, CardStack &ballTwoStack, CardStack &ballThreeStack, Card* selectedBallCard);
void drawPins(QGraphicsScene *scene, CardStack &cardStack, int cardWidth, int cardHeight, int cardGap);
void drawBalls(QGraphicsScene *scene, CardStack &ballOneStack, CardStack &ballTwoStack, CardStack &ballThreeStack, int cardWidth, int cardHeight, int cardGap);
void setPinTouchedCards(CardStack &cardStack);
void printPinTouchedCards(CardStack &cardStack);
bool pinSelectionCheck(QGraphicsScene *scene, CardStack &pinStack, int maxSelection, int cardWidth, int cardHeight);
Card* ballSelectionCheck(QGraphicsScene *scene, CardStack &ballOneStack, CardStack &ballTwoStack, CardStack &ballThreeStack, Card* selectedBallCard, int cardWidth, int cardHeight);
int validMoveCheck(Card* selectedBallCard, int pinTotal, bool validCheck);
void drawThings(QGraphicsScene *scene, myButton* applyMoveButton, int validMove, bool validCheck, Card* selectedBallCard, CardStack &pinStack, int pinTotal, int cardGap, int cardWidth, int cardHeight);
int applyPinMove(QGraphicsScene *scene, CardStack &pinStack, bool endMove);
void applySelectedBallMove(QGraphicsScene *scene, Card* selectedBallCard, CardStack *ballStack, bool endMove);
void popBallTops(QGraphicsScene *scene, CardStack &ballOneStack, CardStack &ballTwoStack, CardStack &ballThreeStack);
void handleButton();

int main(int argc, char **argv)
{
  QApplication app (argc, argv);
  int cardWidth = 75, cardHeight = 110, cardGap = 10, maxSelection = 3;
  int winWidth = 800, winHeight = 995;
  QGraphicsScene scene(0, 0, winWidth, winHeight);
  //scene.setSceneRect(0, 0, 2000, 2000);
  QGraphicsView view(&scene);
  view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  view.setBackgroundBrush(QColor(0,255, 0));
  view.setWindowTitle("Bowling Solitaire");
  view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view.setFixedSize(winWidth, winHeight+5);
  GameValues scoreBoard;
  scoreBoard.createScoreBoard();
  scoreBoard.drawScoreBoardSkeleton(&scene);
  //apply move button
  myButton* applyMoveButton = new myButton(200, 600, 100, 100);
  QBrush* applyMoveButtonBrush = new QBrush(Qt::transparent);
  applyMoveButtonBrush->setColor(QColor("orange"));
  applyMoveButton->setBrush(*applyMoveButtonBrush);
  scene.addItem((QGraphicsItem*)applyMoveButton);
  QGraphicsTextItem *applyButtonText = new QGraphicsTextItem;
  applyButtonText->setScale(3);
  applyButtonText->setPos(200, 600);
  applyButtonText->setPlainText("Apply\nMove");
  applyButtonText->setDefaultTextColor(QColor("white"));
  scene.addItem(applyButtonText);
  //end move button
  myButton* endMoveButton = new myButton(50, 600, 100, 100);
  QBrush* endMoveButtonBrush = new QBrush(Qt::transparent);
  endMoveButtonBrush->setColor(QColor("purple"));
  endMoveButton->setBrush(*endMoveButtonBrush);
  scene.addItem((QGraphicsItem*)endMoveButton);
  QGraphicsTextItem *endButtonText = new QGraphicsTextItem;
  endButtonText->setScale(3);
  endButtonText->setPos(50, 600);
  endButtonText->setPlainText("End\nBall");
  endButtonText->setDefaultTextColor(QColor("white"));
  scene.addItem(endButtonText);

  //strike button
  myButton* strikeButton = new myButton(600, 50, 50, 50);
  QBrush* strikeButtonBrush = new QBrush(Qt::transparent);
  strikeButtonBrush->setColor(QColor("red"));
  strikeButton->setBrush(*strikeButtonBrush);
  scene.addItem((QGraphicsItem*)strikeButton);
  QGraphicsTextItem *strikeButtonText = new QGraphicsTextItem;
  strikeButtonText->setScale(3);
  strikeButtonText->setPos(600, 40);
  strikeButtonText->setPlainText("X");
  strikeButtonText->setDefaultTextColor(QColor("white"));
  scene.addItem(strikeButtonText);

  enum State{
    STATE_CREATESCREEN,
    STATE_USERINPUT,
    STATE_APPLIEDMOVE,
    STATE_ENDMOVE,
    STATE_ENDGAME
  };
  State nextState;
  State state = STATE_CREATESCREEN;
  CardDeck cardDeck;
  CardStack pinStack;
  CardStack ballOneStack;
  CardStack ballTwoStack;
  CardStack ballThreeStack;
  Card* selectedBallCard = 0;
  int pinCount = 0;
  bool gameOver = false;
  while(!gameOver){
    switch(state){
    case STATE_CREATESCREEN:
    {
      clearScene(&scene, pinStack, ballOneStack, ballTwoStack, ballThreeStack, selectedBallCard);
      //add screen clear at beginning
      cardDeck.createCards(10);
      cardDeck.shuffleCards(100);
      pinStack.createStack(cardDeck, 10, Card::Pin);
      setPinTouchedCards(pinStack);
      ballOneStack.createStack(cardDeck, 5, Card::Ball1);
      ballTwoStack.createStack(cardDeck, 3, Card::Ball2);
      ballThreeStack.createStack(cardDeck, 2, Card::Ball3);
      drawPins(&scene, pinStack, cardWidth, cardHeight, cardGap);
      drawBalls(&scene, ballOneStack, ballTwoStack, ballThreeStack, cardWidth, cardHeight, cardGap);
      view.show();
      //set all cards in nonclicked initially
      for(int i = 0; i < pinStack.getStackCards().size(); i++){
        pinStack.appendNonSelectedCards(pinStack.getStackCard(i));
      }
      selectedBallCard = 0;
      nextState = STATE_USERINPUT;
      state = nextState;
      QCoreApplication::processEvents();
      break;
    }
    case STATE_USERINPUT:
    {
      bool validCheck = pinSelectionCheck(&scene, pinStack, maxSelection, cardWidth, cardHeight);
      selectedBallCard = ballSelectionCheck(&scene, ballOneStack, ballTwoStack, ballThreeStack, selectedBallCard, cardWidth, cardHeight);
      //get pin total
      int pinTotal = 0;
      for(int i = 0; i < pinStack.getSelectedCards().size(); i++){
        pinTotal += pinStack.getSelectedCard(i)->getValue();
      }
      int validMove = validMoveCheck(selectedBallCard, pinTotal, validCheck);
      drawThings(&scene, applyMoveButton, validMove, validCheck, selectedBallCard, pinStack, pinTotal, cardGap, cardWidth, cardHeight);
      if(applyMoveButton->mIsClicked || endMoveButton->mIsClicked){
        if(validMove == 1 && applyMoveButton->mIsClicked)  nextState = STATE_APPLIEDMOVE;
        else applyMoveButton->mIsClicked = false;
        if(endMoveButton->mIsClicked) nextState = STATE_ENDMOVE;
        else endMoveButton->mIsClicked = false;
      }
      else nextState = STATE_USERINPUT;
      state = nextState;
      if(nextState == STATE_APPLIEDMOVE || nextState == STATE_ENDMOVE){
        if(strikeButton->mIsClicked){
          pinCount = 10;
          strikeButton->mIsClicked = false;
        }
      }
      QCoreApplication::processEvents();
      break;
    }
    case STATE_APPLIEDMOVE:
    {
      applyMoveButton->mIsClicked = false;
      pinCount = applyPinMove(&scene, pinStack, false);
      //update the frame score
      scoreBoard.updateBallScore(pinCount, scoreBoard.getCurBall(), scoreBoard.getCurFrame());
      CardStack* ballStack = new CardStack;
      switch(selectedBallCard->getStackLocation()){
      case Card::Ball1:
        ballStack = &ballOneStack;
        break;
      case Card::Ball2:
        ballStack = &ballTwoStack;
        break;
      case Card::Ball3:
        ballStack = &ballThreeStack;
        break;
      }
      applySelectedBallMove(&scene, selectedBallCard, ballStack, false);
      selectedBallCard = 0;
      pinCount = 0;
      //set cardsNeeded based on cards removed
      nextState = STATE_USERINPUT;
      state = nextState;
      QCoreApplication::processEvents();
      break;
    }
    case STATE_ENDMOVE:
    {
      Card* curCard = 0;
      QGraphicsItem* item = 0;
      endMoveButton->mIsClicked = false;
      if(scoreBoard.getCurBall() == 0){
        scoreBoard.updateScoreBoard(&scene, pinCount);
        scoreBoard.addToScoreBoard(&scene);
        scoreBoard.setCurBall(1);
        applyPinMove(&scene, pinStack, true);
        CardStack* ballStack = new CardStack;
        if(selectedBallCard){
          switch(selectedBallCard->getStackLocation()){
          case Card::Ball1:
            ballStack = &ballOneStack;
            break;
          case Card::Ball2:
            ballStack = &ballTwoStack;
            break;
          case Card::Ball3:
            ballStack = &ballThreeStack;
            break;
          }
        }
        applySelectedBallMove(&scene, selectedBallCard, ballStack, true);
        selectedBallCard = 0;
        popBallTops(&scene, ballOneStack, ballTwoStack, ballThreeStack);
        //if first ball was strike and last frame
        if(scoreBoard.getFrame(scoreBoard.getCurFrame()).getBallScore(0) == 10 &&
           scoreBoard.getCurFrame() == scoreBoard.getGameLength()-1){
          nextState = STATE_CREATESCREEN;
        }
        //first ball was strike any other time
        else if(scoreBoard.getFrame(scoreBoard.getCurFrame()).getBallScore(0) == 10){
          scoreBoard.setCurBall(0);
          scoreBoard.setCurFrame(scoreBoard.getCurFrame()+1);
          nextState = STATE_CREATESCREEN;
        }
        else nextState = STATE_USERINPUT;
        state = nextState;
      }
      else{//ending the frame
        scoreBoard.updateScoreBoard(&scene, pinCount);
        scoreBoard.addToScoreBoard(&scene);
        if(scoreBoard.getCurFrame() < scoreBoard.getGameLength()-1){
          scoreBoard.setCurBall(0);
          scoreBoard.setCurFrame(scoreBoard.getCurFrame()+1);
          nextState = STATE_CREATESCREEN;
        }
        //last frame and first two balls had strike or spare
        else if(scoreBoard.getCurFrame() == scoreBoard.getGameLength()-1 &&
           scoreBoard.getFrame(scoreBoard.getCurFrame()).getFrameScore() >= 10){
          if(scoreBoard.getCurBall() == 2){
            nextState = STATE_ENDGAME;
          }
          else{
            scoreBoard.setCurBall(2);
            if(scoreBoard.getFrame(scoreBoard.getCurFrame()).getBallScore(1) == 10) nextState = STATE_CREATESCREEN;
            else{
              popBallTops(&scene, ballOneStack, ballTwoStack, ballThreeStack);
              nextState = STATE_USERINPUT;
            }
          }
        }
        else{
          nextState = STATE_ENDGAME;
        }
      }
      state = nextState;
      pinCount = 0;
      scoreBoard.printScoreBoard();
      QCoreApplication::processEvents();
      break;
    }
    case STATE_ENDGAME:
    {
      gameOver = true;
      break;
    }
    }
  }
  return app.exec();
}


void clearScene(QGraphicsScene *scene, CardStack &pinStack,  CardStack &ballOneStack, CardStack &ballTwoStack, CardStack &ballThreeStack, Card* selectedBallCard){
  Card* curCard;
  QGraphicsItem* item;
  //remove selections
  while(!pinStack.getSelectedCards().isEmpty()){
    //remove selectedPinRects
    curCard = pinStack.getSelectedCard(0);
    pinStack.appendNonSelectedCards(curCard);
    pinStack.removeSelectedCards(0);
    item = scene->itemAt(curCard->pos(), QTransform());
    if(item && item->type() != curCard->type()){
      scene->removeItem(item);
    }
  }
  //remove pin cards from scene
  while(!pinStack.getStackCards().isEmpty()){
    curCard = pinStack.getStackCard(pinStack.getStackCards().size()-1);
    item = scene->itemAt(curCard->pos(), QTransform());
    if(item && item->type() == curCard->type()){
      scene->removeItem(item);
    }
    pinStack.removeFromStack();
  }

  //remove selectedBallRect
  if(selectedBallCard){
    item = scene->itemAt(selectedBallCard->pos(), QTransform());
    if(item && item->type() != selectedBallCard->type()){
      scene->removeItem(scene->itemAt(selectedBallCard->pos(), QTransform()));
    }
  }
  selectedBallCard = 0;
  //remove ball cards from scene
  while(!ballOneStack.getStackCards().isEmpty()) {
    curCard = ballOneStack.getStackCard(ballOneStack.getStackCards().size()-1);
    item = scene->itemAt(curCard->pos(), QTransform());
    if(item && item->type() == curCard->type()){
      scene->removeItem(item);
    }
    ballOneStack.removeFromStack();
  }
  while(!ballTwoStack.getStackCards().isEmpty()) {
    curCard = ballTwoStack.getStackCard(ballTwoStack.getStackCards().size()-1);
    item = scene->itemAt(curCard->pos(), QTransform());
    if(item && item->type() == curCard->type()){
      scene->removeItem(item);
    }
    ballTwoStack.removeFromStack();
  }
  while(!ballThreeStack.getStackCards().isEmpty()){
    curCard = ballThreeStack.getStackCard(ballThreeStack.getStackCards().size()-1);
    item = scene->itemAt(curCard->pos(), QTransform());
    if(item && item->type() == curCard->type()){
      scene->removeItem(item);
    }
    ballThreeStack.removeFromStack();
  }
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

void setPinTouchedCards(CardStack &pinStack){
  pinStack.getStackCard(0)->setTouchedCards({1, 4});
  pinStack.getStackCard(1)->setTouchedCards({0, 2, 4, 5});
  pinStack.getStackCard(2)->setTouchedCards({1, 3, 5, 6});
  pinStack.getStackCard(3)->setTouchedCards({2, 6});
  pinStack.getStackCard(4)->setTouchedCards({0, 1, 5, 7});
  pinStack.getStackCard(5)->setTouchedCards({1, 2, 4, 6, 7, 8});
  pinStack.getStackCard(6)->setTouchedCards({2, 3, 5, 8});
  pinStack.getStackCard(7)->setTouchedCards({4, 5, 8, 9});
  pinStack.getStackCard(8)->setTouchedCards({5, 6, 7, 9});
  pinStack.getStackCard(9)->setTouchedCards({7, 8});
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

bool pinSelectionCheck(QGraphicsScene *scene, CardStack &pinStack, int maxSelection, int cardWidth, int cardHeight){
  int selectedSize = pinStack.getSelectedCards().size();
  //go through all of selected, if clicked then goes to nonselected and highlight removed
  for(int i = 0; i < pinStack.getSelectedCards().size(); i++){
    Card* curCard = pinStack.getSelectedCard(i);
    if(curCard->getClicked()){
      pinStack.appendNonSelectedCards(curCard);
      pinStack.removeSelectedCards(i);
      QGraphicsItem* item = scene->itemAt(curCard->pos(), QTransform());
      if(item && item->type() != curCard->type()){
        scene->removeItem(item);
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
        scene->addRect(curCard->pos().x(), curCard->pos().y(), cardWidth, cardHeight, QPen(QColor("blue")));
      }
      //clear clicked
      curCard->clearClicked();
    }
  }

  //check if valid selection
  QList<int> selectedIndices;
  //selection cards places in the pins
  for(int i = 0; i < pinStack.getSelectedCards().size(); i++){
    for(int j = 0; j < pinStack.getStackCards().size(); j++){
      if(pinStack.getSelectedCard(i) == pinStack.getStackCard(j)){
        selectedIndices.append(j);
      }
    }
  }
  int count = 0;
  bool validCheck = false;
  QList<bool> isValid = {false, false, false};
  //check using algorithm
  switch(pinStack.getSelectedCards().size()){
  //do nothing
  case 0:
  {
    break;
  }
  //one selection is valid
  case 1:
  {
    //return isValid;
    validCheck = true;
  //if two or 3 then use function
    break;
  }
  case 2:
  {
    //check if 0 relates to 1
    for(int i = 0; i < pinStack.getSelectedCard(1)->getTouchedCards().size(); i++){
      if(selectedIndices.at(0) == pinStack.getSelectedCard(1)->getTouchedCards().at(i)){
        isValid.replace(0, true);
      }
    }
    //check if 1 relates to 0
    for(int i = 0; i < pinStack.getSelectedCard(0)->getTouchedCards().size(); i++){
      if(selectedIndices.at(1) == pinStack.getSelectedCard(0)->getTouchedCards().at(i)){
        isValid.replace(1, true);
      }
    }
    for(int i = 0; i < isValid.size(); i++){
      if(isValid.at(i)) count++;
    }
    if(count > 0){
      validCheck = true;
    }
    break;
  }
  case 3:
  {
    //check if 0 relates to 1
    for(int i = 0; i < pinStack.getSelectedCard(1)->getTouchedCards().size(); i++){
      if(selectedIndices.at(0) == pinStack.getSelectedCard(1)->getTouchedCards().at(i)){
        isValid.replace(0, true);
      }
    }
    //check if 1 relates to 2
    for(int i = 0; i < pinStack.getSelectedCard(2)->getTouchedCards().size(); i++){
      if(selectedIndices.at(1) == pinStack.getSelectedCard(2)->getTouchedCards().at(i)){
        isValid.replace(1, true);
      }
    }
    //check if 2 relates to 0
    for(int i = 0; i < pinStack.getSelectedCard(0)->getTouchedCards().size(); i++){
      if(selectedIndices.at(2) == pinStack.getSelectedCard(0)->getTouchedCards().at(i)){
        isValid.replace(2, true);
      }
    }
    for(int i = 0; i < isValid.size(); i++){
      if(isValid.at(i)) count++;
    }
    if(count > 1){
      validCheck = true;
    }
    break;
  }
  }
  return validCheck;
}

Card* ballSelectionCheck(QGraphicsScene *scene, CardStack &ballOneStack, CardStack &ballTwoStack, CardStack &ballThreeStack, Card* selectedBallCard, int cardWidth, int cardHeight){
  //get ball selection
  Card* newSelectedBallCard = 0;
  if(ballOneStack.getStackCards().size() != 0){
    Card* curCard = ballOneStack.getStackCards().last();
    if(curCard->getClicked()/* && selectedBallCard != 0*/){
      newSelectedBallCard = curCard;
      curCard->clearClicked();
    }
  }
  if(ballTwoStack.getStackCards().size() != 0){
    Card* curCard = ballTwoStack.getStackCards().last();
    if(curCard->getClicked()/* && selectedBallCard != 0*/){
      newSelectedBallCard = curCard;
      curCard->clearClicked();
    }
  }
  if(ballThreeStack.getStackCards().size() != 0){
    Card* curCard = ballThreeStack.getStackCards().last();
    if(curCard->getClicked()/* && selectedBallCard != 0*/){
      newSelectedBallCard = curCard;
      curCard->clearClicked();
    }
  }
  //toggling the already selected one
  if(selectedBallCard == newSelectedBallCard){
    if(selectedBallCard != 0){
      QGraphicsItem* item = scene->itemAt(selectedBallCard->pos(), QTransform());
      if(item && item->type() != selectedBallCard->type()){
        scene->removeItem(item);
      }
      selectedBallCard = 0;
    }
  }
  //add/remove selected ball rectangles
  else{
    if(newSelectedBallCard != 0){
      if(selectedBallCard != 0){
        QGraphicsItem* item = scene->itemAt(selectedBallCard->pos(), QTransform());
        if(item && item->type() != selectedBallCard->type()){
          scene->removeItem(item);
        }
      }
      scene->addRect(newSelectedBallCard->pos().x(), newSelectedBallCard->pos().y(), cardWidth, cardHeight, QPen(QColor("blue")));
      selectedBallCard = newSelectedBallCard;
    }
  }
  return selectedBallCard;
}

int validMoveCheck(Card* selectedBallCard, int pinTotal, bool validCheck){
  //see if pin and ball selection are compatible
  int validMove = 0;
  int pinChoice =-1;
  if(pinTotal != 0) pinChoice = pinTotal%10;
  int ballChoice = -1;
  if(selectedBallCard != 0) ballChoice = selectedBallCard->getValue()%10;
  if(pinChoice == ballChoice && pinChoice != -1 && validCheck){
    validMove = 1;
  }
  else if((ballChoice == -1 && validCheck)|| pinChoice == -1){
    validMove = 0;
  }
  else{
    validMove = -1;
  }
  return validMove;
}

void drawThings(QGraphicsScene *scene, myButton* applyMoveButton, int validMove, bool validCheck, Card* selectedBallCard, CardStack &pinStack, int pinTotal, int cardGap, int cardWidth, int cardHeight){
  //draw things each loop (rectangles, pin selection count, buttons)
  int selectedTotX, selectedTotY;
  selectedTotX = cardGap+2*cardWidth;
  selectedTotY = cardHeight*4+cardGap*6;
  if(scene->itemAt(selectedTotX, selectedTotY, QTransform())) scene->removeItem(scene->itemAt(selectedTotX, selectedTotY, QTransform()));
  if(pinTotal > 0){
    QGraphicsSimpleTextItem* pinText = new QGraphicsSimpleTextItem;
    QPen textPen;
    textPen.setWidth(0);
    pinText->setPos(selectedTotX, selectedTotY);
    pinText->setText(QString::number(pinTotal));
    if(validMove == 1) pinText->setBrush(QColor("blue"));
    else if(validMove == 0) pinText->setBrush(QColor("orange"));
    else pinText->setBrush(QColor("red"));
    pinText->setPen(textPen);
    pinText->setScale(3);
    scene->addItem(pinText);
  }

  QPen ballRectPen;
  ballRectPen.setWidth(3);
  if(validCheck && validMove == 1) ballRectPen.setColor("blue");
  else if(validMove == 0) ballRectPen.setColor("orange");
  else ballRectPen.setColor("red");
  if(selectedBallCard != 0){
    QGraphicsItem* ballItem = scene->itemAt(selectedBallCard->pos(), QTransform());
    QGraphicsRectItem* ballItemRect = (QGraphicsRectItem*) ballItem;
    ballItemRect->setPen(ballRectPen);
  }

  QPen pinRectPen;
  pinRectPen.setWidth(3);
  for(int i = 0; i < pinStack.getSelectedCards().size(); i++){
    Card* curPinCard = pinStack.getSelectedCard(i);
    QGraphicsItem* pinItem = scene->itemAt(curPinCard->pos(), QTransform());
    QGraphicsRectItem* pinItemRect = (QGraphicsRectItem*) pinItem;
    //make sure it's a rectangle that was selected
    if(pinItem && pinItem->type() != curPinCard->type()){
      if(validCheck && validMove == 1){
        pinRectPen.setColor("blue");
        pinItemRect->setPen(pinRectPen);
      }
      else if(validCheck && validMove == 0){
        pinRectPen.setColor("orange");
        pinItemRect->setPen(pinRectPen);
      }
      else{
        pinRectPen.setColor("red");
        pinItemRect->setPen(pinRectPen);
      }
    }
  }

  QPen buttonPen;
  if(validMove == 1) buttonPen.setColor("blue");
  else if(validMove == 0) buttonPen.setColor("orange");
  else buttonPen.setColor("red");
  buttonPen.setWidth(5);
  applyMoveButton->setPen(buttonPen);
}

int applyPinMove(QGraphicsScene *scene, CardStack &pinStack, bool endMove){
  Card* curCard = 0;
  QGraphicsItem* item = 0;
  int pinCount = 0;
  //remove selections while updating frame score
  while(!pinStack.getSelectedCards().isEmpty()){
    //remove selectedPinRects
    curCard = pinStack.getSelectedCard(0);
    pinStack.appendNonSelectedCards(curCard);
    pinStack.removeSelectedCards(0);
    item = scene->itemAt(curCard->pos(), QTransform());
    if(item && item->type() != curCard->type()){
      scene->removeItem(item);
    }
    if(!endMove){
      //get rid of actual card from screen
      item = scene->itemAt(curCard->pos(), QTransform());
      if(item && item->type() == curCard->type()){
        scene->removeItem(item);
      }
    }
    curCard->clearClicked();
    pinCount++;
  }
  return pinCount;
}

void applySelectedBallMove(QGraphicsScene *scene, Card* selectedBallCard, CardStack *ballStack, bool endMove){
  QGraphicsItem* item = 0;
  //remove selectedBallRect
  if(selectedBallCard){
    item = scene->itemAt(selectedBallCard->pos(), QTransform());
    if(item && item->type() != selectedBallCard->type()){
      scene->removeItem(scene->itemAt(selectedBallCard->pos(), QTransform()));
    }
    if(!endMove){
      //remove actual card
      item = scene->itemAt(selectedBallCard->pos(), QTransform());
      if(item && item->type() == selectedBallCard->type()){
        scene->removeItem(item);
      }
      ballStack->removeFromStack();
    }
  }
}

void popBallTops(QGraphicsScene *scene, CardStack &ballOneStack, CardStack &ballTwoStack, CardStack &ballThreeStack){
  //pop top off every ball stack and remove cards from screen
  Card* curCard = 0;
  QGraphicsItem* item = 0;
  if(!ballOneStack.getStackCards().isEmpty()) {
    curCard = ballOneStack.getStackCard(ballOneStack.getStackCards().size()-1);
    item = scene->itemAt(curCard->pos(), QTransform());
    if(item && item->type() == curCard->type()){
      scene->removeItem(item);
    }
    ballOneStack.removeFromStack();
  }
  if(!ballTwoStack.getStackCards().isEmpty()) {
    curCard = ballTwoStack.getStackCard(ballTwoStack.getStackCards().size()-1);
    item = scene->itemAt(curCard->pos(), QTransform());
    if(item && item->type() == curCard->type()){
      scene->removeItem(item);
    }
    ballTwoStack.removeFromStack();
  }
  if(!ballThreeStack.getStackCards().isEmpty()){
    curCard = ballThreeStack.getStackCard(ballThreeStack.getStackCards().size()-1);
    item = scene->itemAt(curCard->pos(), QTransform());
    if(item && item->type() == curCard->type()){
      scene->removeItem(item);
    }
    ballThreeStack.removeFromStack();
  }
}
