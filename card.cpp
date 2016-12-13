#include "card.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSvgItem>
#include <QList>
#include <vector>

#include "carddeck.h"

Card::Card(/*const QString &fileName,*/ int value, CardSuite suite)
  : /*QGraphicsSvgItem(fileName),*/
    mCardValue(value),
    mCardSuite(suite),
    mCardRevealed(false)

{
  setFlag(ItemIsSelectable);
}

Card::~Card()
{

}
