/*
  Copyright (c) 2012 The KoRE Project

  This file is part of KoRE.

  KoRE is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

  KoRE is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with KoRE.  If not, see <http://www.gnu.org/licenses/>.
*/

/************************************************************************/
/* \author Dominik Ospelt                                               */
/************************************************************************/

#include "KoRE_GUI/ShaderDataItem.h"
#include <QPainter>
#include <QCursor>
#include <QStaticText>
#include <QToolTip>

koregui::ShaderDataItem::ShaderDataItem(const kore::ShaderData* data,
                                        NodeItem* nodeItem,
                                        QGraphicsItem* parent)
                                      : _data(data),
                                        _nodeitem(nodeItem),
                                        _mouseover(false),
                                        QGraphicsItem(parent) {
  setAcceptHoverEvents(true);
  setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
  setData(0, "SHADERDATA");
}

koregui::ShaderDataItem::~ShaderDataItem(void) {
}

void koregui::ShaderDataItem::refresh(void) {

}

QRectF koregui::ShaderDataItem::boundingRect() const {
  return QRectF(-2, -2, 16, 16);
}

void koregui::ShaderDataItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
  QPen p;
  p.setColor(QColor(44,44,44));
  p.setWidth(2);

  QBrush b;

  switch(_data->type) {
  case GL_FLOAT_MAT4:
    b.setColor(QColor(35,203,173));
    break;
  case GL_FLOAT_VEC3:
    b.setColor(QColor(252,210,89));
    break;
  case GL_FLOAT_VEC2:
    b.setColor(QColor(244,179,138));
    break;
  case GL_SAMPLER_2D:
    b.setColor(QColor(250,123,28));
    break;
  default : b.setColor(Qt::GlobalColor::yellow);
  }
  b.setStyle(Qt::BrushStyle::SolidPattern);
  painter->setBrush(b);
  painter->setPen(p);
  if(_mouseover) {
    painter->drawRect(-2, -2, 16, 16);
  } else {
   painter->drawRect(0, 0, 12, 12);
  }
}

void koregui::ShaderDataItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event) {
  _mouseover = true;
  QGraphicsItem::hoverEnterEvent(event);
}
void koregui::ShaderDataItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event) {
  _mouseover = false;
  QGraphicsItem::hoverLeaveEvent(event);
}

void koregui::ShaderDataItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  // TODO (dospelt) change color?
}
