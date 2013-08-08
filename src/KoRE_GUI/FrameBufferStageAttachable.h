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

#ifndef FRAMEBUFFERSTAGEATTACHABLE_H_
#define FRAMEBUFFERSTAGEATTACHABLE_H_

#include <QGraphicsItem>

namespace koregui {
  class FrameBufferStageAttachable : public QGraphicsItem {
  public:
    FrameBufferStageAttachable(QGraphicsItem* parent = 0);
    ~FrameBufferStageAttachable(void);

    virtual void refresh(void) = 0;

    inline int getHeight(void) {return _attachableheight;};
    inline int getWidth(void) {return _attachablewidth;};

  private:
    
    std::string _name;
    unsigned int _attachableheight;
    unsigned int _attachablewidth;
  };
}
#endif  // FRAMEBUFFERSTAGEATTACHABLE_H_
