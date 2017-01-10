#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QList>
#include "card.h"

//knows each ball score and total score at that frame
class Frame
{
public:
  int getFrameSize(){return mFrameSize;}
  int getBallScore(int ballNum);
  void updateFrameScore(int ballNum, int ballScore);
  int getFrameScore();
  int getScoreToHere(){return mScoreToHere;}
  void setScoreToHere(int scoreToHere){mScoreToHere = scoreToHere;}

  Frame(int frameSize);
  ~Frame();

private:
  int mFrameSize;
  int mScoreToHere;
  QList<int> mFrameScore;
};

class GameValues
{
public:
  void createScoreBoard();
  QList<Card*> getNeededCards(){return mNeededCards;}
  Frame getFrame(int frame){return (frame >= 0 && frame < mGameLength) ? mScoreBoard[frame]:0;}
  int getCurFrame(){return mCurFrame;}
  void setCurFrame(int curFrame){mCurFrame = curFrame;}
  QList<Frame> getScoreBoard(){return mScoreBoard;}
  int getBallScore(){return mBallScore;}
  void updateBallScore(int ballScore, int ballNum, int curFrame);
  void setNeededCards(QList<Card*> neededCards){mNeededCards = neededCards;}
  int getCurBall(){return mCurBall;}
  void setCurBall(int ballNum){mCurBall = ballNum;}
  int getGameLength(){return mGameLength;}
  void printScoreBoard();
  void updateScoreBoard(QGraphicsScene *scene, int pinCount);
  void drawScoreBoardSkeleton(QGraphicsScene *scene);
  void addToScoreBoard(QGraphicsScene *scene);
  GameValues();
  ~GameValues();

private:
  QList<Card*> mNeededCards;
  int mCurFrame;
  QList<Frame> mScoreBoard;
  int mBallScore;
  int mGameLength;
  int mCurBall;

};

#endif // GAMESTATE_H
