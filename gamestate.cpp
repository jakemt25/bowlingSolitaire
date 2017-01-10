#include "gamestate.h"

#include <QList>
#include <QGraphicsScene>



//creates a scoreboard to be used
void GameValues::createScoreBoard(){
  //create all frames but last
  for(int i = 0; i < mGameLength-1; i++){
    mScoreBoard.append(*new Frame(2));
  }
  //create last frame
  mScoreBoard.append(*new Frame(3));
}

void GameValues::updateBallScore(int ballScore, int ballNum, int curFrame){
  if(curFrame < mGameLength){
    mScoreBoard[curFrame].updateFrameScore(ballNum, ballScore);
    //qDebug() << mScoreBoard[curFrame].getFrameScore();
  }
}

//prints scoreboard with qdebug
void GameValues::printScoreBoard(){
  qDebug() << "Current Frame is:" << mCurFrame;
  for(int i = 0; i < mScoreBoard.size(); i++){
    qDebug() << "Frame: " << i;
    if(i == mScoreBoard.size()-1){
      qDebug() << mScoreBoard[i].getBallScore(0) << mScoreBoard[i].getBallScore(1) << mScoreBoard[i].getBallScore(2);
    }
    else{
      qDebug() << mScoreBoard[i].getBallScore(0) << mScoreBoard[i].getBallScore(1);
    }
    qDebug() << "Total for Frame" << i << "is " << mScoreBoard[i].getFrameScore();
    qDebug() << "Total up to here:" << mScoreBoard[i].getScoreToHere();
  }
}

//draws all of the boxes to be filled in later
void GameValues::drawScoreBoardSkeleton(QGraphicsScene *scene){
  int frameWidth = 75, frameHeight = 100, startX = 25, startY = 750;
  for(int i = 0; i < mGameLength-1; i++){
    QGraphicsRectItem* frame = new QGraphicsRectItem(startX+(frameWidth*i), startY, frameWidth, frameHeight);
    QBrush* frameBrush = new QBrush(Qt::transparent);
    frameBrush->setColor(QColor("white"));
    frame->setBrush(*frameBrush);
    scene->addItem(frame);

    QGraphicsRectItem* lilBox = new QGraphicsRectItem((startX+(2*frameWidth/3))+(frameWidth*i), startY, frameWidth/3, frameHeight/3);
    QBrush* lilBoxBrush = new QBrush(Qt::transparent);
    lilBoxBrush->setColor(QColor("white"));
    lilBox->setBrush(*lilBoxBrush);
    scene->addItem(lilBox);
  }
  int i = mGameLength-1;
  QGraphicsRectItem* frame = new QGraphicsRectItem(startX+(frameWidth*i), startY, frameWidth, frameHeight);
  QBrush* frameBrush = new QBrush(Qt::transparent);
  frameBrush->setColor(QColor("white"));
  frame->setBrush(*frameBrush);
  scene->addItem(frame);

  QGraphicsRectItem* lilBox = new QGraphicsRectItem((startX+(2*frameWidth/3))+(frameWidth*i), startY, frameWidth/3, frameHeight/3);
  QBrush* lilBoxBrush = new QBrush(Qt::transparent);
  lilBoxBrush->setColor(QColor("white"));
  lilBox->setBrush(*lilBoxBrush);
  scene->addItem(lilBox);

  QGraphicsRectItem* lilBox2 = new QGraphicsRectItem((startX+(1*frameWidth/3))+(frameWidth*i), startY, frameWidth/3, frameHeight/3);
  QBrush* lilBox2Brush = new QBrush(Qt::transparent);
  lilBox2Brush->setColor(QColor("white"));
  lilBox2->setBrush(*lilBox2Brush);
  scene->addItem(lilBox2);
}

//only adds the individual ball scores to the drawn scoreboard
void GameValues::addToScoreBoard(QGraphicsScene *scene){
  int frameWidth = 75, frameHeight = 100, startX = 18, startY = 735;
  QGraphicsTextItem *scoreBoardText = new QGraphicsTextItem;
  scoreBoardText->setScale(3);
  if(mCurFrame != mGameLength-1){
    if(mCurBall == 0){
      int curBall = mScoreBoard[mCurFrame].getBallScore(0);
      scoreBoardText->setPos((startX+(1*frameWidth/3))+(frameWidth*mCurFrame), startY);
      if(curBall == 10){
        scoreBoardText->setPos((startX+(2*frameWidth/3))+(frameWidth*mCurFrame), startY);
        scoreBoardText->setDefaultTextColor(Qt::red);
        scoreBoardText->setPlainText("X");
      }
      else{
        if(curBall == 0){
          scoreBoardText->setPlainText("-");
        }
        else scoreBoardText->setPlainText(QString::number(curBall));
      }
      scoreBoardText->setZValue(1);
      scene->addItem(scoreBoardText);
    }
    else if (mCurBall == 1){
      int curBall = mScoreBoard[mCurFrame].getBallScore(1);
      scoreBoardText->setPos((startX+(2*frameWidth/3))+(frameWidth*mCurFrame), startY);
      if(mScoreBoard[mCurFrame].getFrameScore() == 10){
        scoreBoardText->setDefaultTextColor(Qt::blue);
        if(curBall != 0) scoreBoardText->setPlainText("/");
      }
      else{
        if(curBall == 0){
          scoreBoardText->setPlainText("-");
        }
        else scoreBoardText->setPlainText(QString::number(curBall));
      }
      scoreBoardText->setZValue(1);
      scene->addItem(scoreBoardText);
    }
  }
  else{ //handle last frame
    switch(mCurBall){
    case 0:
    {
      int curBall = mScoreBoard[mCurFrame].getBallScore(0);
      scoreBoardText->setPos((startX+(0*frameWidth/3))+(frameWidth*mCurFrame), startY);
      if(curBall == 10){
        scoreBoardText->setDefaultTextColor(Qt::red);
        scoreBoardText->setPlainText("X");
      }
      else{
        if(curBall == 0){
          scoreBoardText->setPlainText("-");
        }
        else scoreBoardText->setPlainText(QString::number(curBall));
      }
      scene->addItem(scoreBoardText);
      break;
    }
    case 1:
    {
      int curBall = mScoreBoard[mCurFrame].getBallScore(1);
      scoreBoardText->setPos((startX+(1*frameWidth/3))+(frameWidth*mCurFrame), startY);
      //second ball got spare
      if(mScoreBoard[mCurFrame].getFrameScore() == 10 && mScoreBoard[mCurFrame].getBallScore(0) != 10){
        scoreBoardText->setDefaultTextColor(Qt::blue);
        scoreBoardText->setPlainText("/");
      }
      else{
        //second ball was strike or number
        if(curBall == 10){
          scoreBoardText->setDefaultTextColor(Qt::red);
          scoreBoardText->setPlainText("X");
        }
        else{
          if(curBall == 0){
            scoreBoardText->setPlainText("-");
          }
          else scoreBoardText->setPlainText(QString::number(curBall));
        }
      scoreBoardText->setZValue(1);
      }
      scene->addItem(scoreBoardText);
      break;
    }
    case 2:
    {
      int ball1 = mScoreBoard[mCurFrame].getBallScore(0);
      int ball2 = mScoreBoard[mCurFrame].getBallScore(1);
      int curBall = mScoreBoard[mCurFrame].getBallScore(2);
      scoreBoardText->setPos((startX+(2*frameWidth/3))+(frameWidth*mCurFrame), startY);
      //if double strike   or spare
      //then this ball can be a strike
      if(ball1+ball2 == 20 || (ball1+ball2 == 10 && ball1 != 10)){
        if(curBall == 10){
          scoreBoardText->setDefaultTextColor(Qt::red);
          scoreBoardText->setPlainText("X");
        }
        else{
          if(curBall == 0){
            scoreBoardText->setPlainText("-");
          }
          else scoreBoardText->setPlainText(QString::number(curBall));
        }
      }
      //first ball was strike, second was not
      else{
        if(ball2 + curBall == 10){
          scoreBoardText->setDefaultTextColor(Qt::blue);
          scoreBoardText->setPlainText("/");
        }
        else{
          if(curBall == 0){
            scoreBoardText->setPlainText("-");
          }
          else scoreBoardText->setPlainText(QString::number(curBall));
        }
      }
      scene->addItem(scoreBoardText);
      break;
    }
    }
  }
}

//does the actual math(sets the frame scores) and adds total score to drawn scoreboard
void GameValues::updateScoreBoard(QGraphicsScene *scene, int pinCount){
  int frameWidth = 75, frameHeight = 100, startX = 18, startY = 735;
  Frame* curFrame = &mScoreBoard[mCurFrame];
  //always update frame
  curFrame->updateFrameScore(mCurBall, pinCount);
  if(mCurBall == 0){
    switch(mCurFrame){
    case 0:
    {
      //first ball, just update frame score
      break;
    }
    case 1:
    {
      //second frame, check if last was a spare, strike handled later
      Frame* prevFrame = &mScoreBoard[mCurFrame-1];
      if(prevFrame->getFrameScore() == 10){
        if(prevFrame->getBallScore(0) != 10){
          //previous frame had spare
          prevFrame->setScoreToHere(prevFrame->getFrameScore()+curFrame->getBallScore(mCurBall));
          QGraphicsTextItem *scoreBoardTotalText = new QGraphicsTextItem;
          scoreBoardTotalText->setScale(3);
          scoreBoardTotalText->setPos((startX+(1*frameWidth/6))+(frameWidth*(mCurFrame-1)), startY+40);
          scoreBoardTotalText->setPlainText(QString::number(mScoreBoard[mCurFrame-1].getScoreToHere()));
          scene->addItem(scoreBoardTotalText);
        }
        else{
          //was strike handled later
        }
      }
      break;
    }
    default: //greater than second frame, check for strikes two frames ago, spare one frame ago
    {
      Frame* fartherFrame;
      if(mCurFrame > 2)fartherFrame = &mScoreBoard[mCurFrame-3];
      Frame* farFrame = &mScoreBoard[mCurFrame-2];
      Frame* prevFrame = &mScoreBoard[mCurFrame-1];
      if(prevFrame->getFrameScore() == 10){
        if(prevFrame->getBallScore(0) == 10){
          //last frame had a strike
          if(farFrame->getBallScore(0) == 10){
            //2 frames ago was a strike
            if(mCurFrame == 2) farFrame->setScoreToHere(farFrame->getFrameScore()+prevFrame->getFrameScore()+curFrame->getBallScore(mCurBall));
            else farFrame->setScoreToHere(fartherFrame->getScoreToHere()+farFrame->getFrameScore()+prevFrame->getFrameScore()+curFrame->getBallScore(mCurBall));
            QGraphicsTextItem *scoreBoardTotalText = new QGraphicsTextItem;
            scoreBoardTotalText->setScale(3);
            scoreBoardTotalText->setPos((startX+(1*frameWidth/6))+(frameWidth*(mCurFrame-2)), startY+40);
            scoreBoardTotalText->setPlainText(QString::number(mScoreBoard[mCurFrame-2].getScoreToHere()));
            scene->addItem(scoreBoardTotalText);
          }
          else{
            //2 frames ago was not a strike
            //only 1 strike behind us, handled later
          }
        }
        else{
          //last frame was a spare
          prevFrame->setScoreToHere(farFrame->getScoreToHere() + prevFrame->getFrameScore()+curFrame->getBallScore(mCurBall));
          QGraphicsTextItem *scoreBoardTotalText = new QGraphicsTextItem;
          scoreBoardTotalText->setScale(3);
          scoreBoardTotalText->setPos((startX+(1*frameWidth/6))+(frameWidth*(mCurFrame-1)), startY+40);
          scoreBoardTotalText->setPlainText(QString::number(mScoreBoard[mCurFrame-1].getScoreToHere()));
          scene->addItem(scoreBoardTotalText);
        }
      }
      else{
        //normal frame
      }
      break;
    }
    }
  }
  else{ //mCurBall > 0
    //if first frame is strike or spare, gets handled later
    if(mCurFrame == 0){
      if(curFrame->getFrameScore() != 10){
        curFrame->setScoreToHere(curFrame->getFrameScore());
        QGraphicsTextItem *scoreBoardTotalText = new QGraphicsTextItem;
        scoreBoardTotalText->setScale(3);
        scoreBoardTotalText->setPos((startX+(1*frameWidth/6))+(frameWidth*(mCurFrame)), startY+40);
        scoreBoardTotalText->setPlainText(QString::number(mScoreBoard[mCurFrame].getScoreToHere()));
        scene->addItem(scoreBoardTotalText);
      }
    }
    else if(mCurFrame < mGameLength-1){
      Frame* prevFrame = &mScoreBoard[mCurFrame-1];
      Frame* farFrame;
      if(mCurFrame > 1)farFrame = &mScoreBoard[mCurFrame-2];
      else farFrame = prevFrame;
      if(prevFrame->getBallScore(0) == 10 && curFrame->getBallScore(0) != 10){
        //last frame was a strike and this wasn't
        prevFrame->setScoreToHere(farFrame->getScoreToHere()+prevFrame->getFrameScore()+curFrame->getFrameScore());
        QGraphicsTextItem *scoreBoardTotalText = new QGraphicsTextItem;
        scoreBoardTotalText->setScale(3);
        scoreBoardTotalText->setPos((startX+(1*frameWidth/6))+(frameWidth*(mCurFrame-1)), startY+40);
        scoreBoardTotalText->setPlainText(QString::number(mScoreBoard[mCurFrame-1].getScoreToHere()));
        scene->addItem(scoreBoardTotalText);
      }
      //handles curframe if not strike or spare
      if(curFrame->getFrameScore() < 10){
        curFrame->setScoreToHere(prevFrame->getScoreToHere()+curFrame->getFrameScore());
        QGraphicsTextItem *scoreBoardTotalText = new QGraphicsTextItem;
        scoreBoardTotalText->setScale(3);
        scoreBoardTotalText->setPos((startX+(1*frameWidth/6))+(frameWidth*(mCurFrame)), startY+40);
        scoreBoardTotalText->setPlainText(QString::number(mScoreBoard[mCurFrame].getScoreToHere()));
        scene->addItem(scoreBoardTotalText);
      }
    }
    else{ //last frame
      Frame* farFrame;
      if(mCurFrame > 1)farFrame = &mScoreBoard[mCurFrame-2];
      Frame* prevFrame = &mScoreBoard[mCurFrame-1];
      if(mCurBall == 1){
        //last frame had strike, handle now
        if(prevFrame->getBallScore(0) == 10){
          prevFrame->setScoreToHere(farFrame->getScoreToHere()+prevFrame->getFrameScore()+curFrame->getFrameScore());
          QGraphicsTextItem *scoreBoardTotalText = new QGraphicsTextItem;
          scoreBoardTotalText->setScale(3);
          scoreBoardTotalText->setPos((startX+(1*frameWidth/6))+(frameWidth*(mCurFrame-1)), startY+40);
          scoreBoardTotalText->setPlainText(QString::number(mScoreBoard[mCurFrame-1].getScoreToHere()));
          scene->addItem(scoreBoardTotalText);
        }
        if(!(mScoreBoard[mCurFrame].getFrameScore() >= 10)){
          curFrame->setScoreToHere(prevFrame->getScoreToHere()+curFrame->getFrameScore());
          QGraphicsTextItem *scoreBoardTotalText = new QGraphicsTextItem;
          scoreBoardTotalText->setScale(3);
          scoreBoardTotalText->setPos((startX+(1*frameWidth/6))+(frameWidth*(mCurFrame)), startY+40);
          scoreBoardTotalText->setPlainText(QString::number(mScoreBoard[mCurFrame].getScoreToHere()));
          scene->addItem(scoreBoardTotalText);
        }
      }
      else{ //mCurBall == 2
        int total, ball1, ball2, ball3;
        Frame* prevFrame = &mScoreBoard[mCurFrame-1];
        ball1 = curFrame->getBallScore(0);
        ball2 = curFrame->getBallScore(1);
        ball3 = curFrame->getBallScore(2);
        total = prevFrame->getScoreToHere() + ball1 + ball2 + ball3;
        curFrame->setScoreToHere(total);QGraphicsTextItem *scoreBoardTotalText = new QGraphicsTextItem;
        scoreBoardTotalText->setScale(3);
        scoreBoardTotalText->setPos((startX+(1*frameWidth/6))+(frameWidth*(mCurFrame)), startY+40);
        scoreBoardTotalText->setPlainText(QString::number(mScoreBoard[mCurFrame].getScoreToHere()));
        scene->addItem(scoreBoardTotalText);
      }
    }
  }
}

GameValues::GameValues(){
  mCurFrame = 0;
  mBallScore = 0;
  mGameLength = 10;
  mCurBall = 0;
}

GameValues::~GameValues(){}

//returns the part of frame called
int Frame::getBallScore(int ballNum){
  if(ballNum < mFrameSize) return mFrameScore.at(ballNum);
  return -1;
}

//adds onto the ball score
void Frame::updateFrameScore(int ballNum, int ballScore){
  if(ballNum < mFrameSize){
    if(mFrameScore[ballNum] == -1) mFrameScore[ballNum] = 0;
    mFrameScore[ballNum] += ballScore;
  }
}

//returns the score of the frame
int Frame::getFrameScore(){
  int totalFrameScore = 0;
  for(int i = 0; i < mFrameSize; i++){
    if(getBallScore(i) != -1){
      totalFrameScore += getBallScore(i);
    }
  }
  return totalFrameScore;
}

Frame::Frame(int frameSize)
  : mFrameSize(frameSize)
{
  for(int i = 0; i < frameSize; i++){
    mFrameScore.append(-1);
  }
  mScoreToHere = 0;
}

Frame::~Frame(){}
