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
  //Types used with card
  enum CardSuite{
    Club = 1,
    Spade,
    Diamond,
    Heart
  };

  //Functions
  CardSuite getSuite(){return mCardSuite;}
  int getValue() {return mCardValue;}
  void setTouchedCards(QList<int> inTouchedCards){mTouchedCards = inTouchedCards;}
  void printTouchedCards(){qDebug()<<"Printing Touched Cards\n"<<mTouchedCards;}

  //Constructor needs: image location, card value, card suite
  Card(const QString &fileName, int mValue, CardSuite mSuite);
  ~Card();

private:
  //Data Values
  CardSuite mCardSuite;
  int mCardValue;
  bool mCardRevealed;
  QList<int> mTouchedCards;
};

#endif // CARD_H
