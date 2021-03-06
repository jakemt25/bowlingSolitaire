/*
 * Card Class:
 * Contains everything about a card
 * Heavy inspiration from: https://github.com/microsoft-mobile/solitaire-qt
 * Jake Thomas 12/9/2016
*/

#ifndef CARD_H
#define CARD_H

#include <QGraphicsSvgItem>
#include <QDebug>

class Card : public QGraphicsSvgItem
{
  Q_OBJECT
public:
  enum {Type = UserType + 1};

  //Types used with card
  enum CardSuite{
    Club = 1,
    Spade,
    Diamond,
    Heart
  };

  enum WhichStack{
    Pin = 0,
    Ball1,
    Ball2,
    Ball3
  };

  //Functions
  CardSuite getSuite(){return mCardSuite;}
  int type() const {return Type;}
  int getValue() {return mCardValue;}
  bool getClicked() {return mIsClicked;}
  void clearClicked(){mIsClicked = false;}
  void setTouchedCards(QList<int> inTouchedCards){mTouchedCards = inTouchedCards;}
  QList<int> getTouchedCards(){return mTouchedCards;}
  void printTouchedCards(){qDebug()<<"Printing Touched Cards\n"<<mTouchedCards;}
  void Card::mousePressEvent(QGraphicsSceneMouseEvent *event){ mIsClicked = true;}
  void setStackLocation(WhichStack stackLocation){mStackLocation = stackLocation;}
  WhichStack getStackLocation(){return mStackLocation;}
  //Constructor needs: image location, card value, card suite
  Card(const QString &fileName, int mValue, CardSuite mSuite);
  ~Card();

private:
  //Data Values
  CardSuite mCardSuite;
  int mCardValue;
  bool mCardRevealed;
  bool mIsClicked;
  QList<int> mTouchedCards;
  WhichStack mStackLocation;
};

#endif // CARD_H
