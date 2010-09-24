//=============================================================================
//  MuseScore
//  Linux Music Score Editor
//  $Id:$
//
//  Copyright (C) 2010 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================

#ifndef __DRUMTOOLS_H__
#define __DRUMTOOLS_H__

class Score;
class Drumset;
class Palette;

//---------------------------------------------------------
//   DrumTools
//---------------------------------------------------------

class DrumTools : public QDockWidget {
      Q_OBJECT

      Score* _score;
      Palette* drumPalette;
      Drumset* drumset;

   private slots:
      void drumNoteSelected(int val);
      void editDrumset();

   public:
      DrumTools(QWidget* parent = 0);
      void setDrumset(Score*, Drumset*);
      };


#endif

