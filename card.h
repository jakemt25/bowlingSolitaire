/*
 * Card Class:
 * Contains everything about a card
 * Heavy inspiration from: https://github.com/microsoft-mobile/solitaire-qt
 * Jake Thomas 12/9/2016
*/

#ifndef CARD_H
#define CARD_H

//SVG files used for the images
#include <QGraphicsSvgItem>

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

  //Constructor needs: image location, card value, card suite
  Card(const QString &fileName, int value, CardSuite cardSuite);
  ~Card();

private:
  //Data Values
  CardSuite cardSuite;
  int cardValue;
  bool cardRevealed;

};

#endif // CARD_H
