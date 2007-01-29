//=============================================================================
//  MuseScore
//  Linux Music Score Editor
//  $Id: undo.cpp,v 1.40 2006/04/12 14:58:10 wschweer Exp $
//
//  Copyright (C) 2002-2006 Werner Schweer (ws@seh.de)
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

/**
 \file
 Implementation of undo functions.

 The undo system requires calling startUndo() when starting a GUI command
 and calling endUndo() when ending the command. All changes to a score
 in response to a GUI command must be undoable/redoable by executing
 a sequence of low-level undo operations. This sequence is built by the code
 handling the command, by calling one or more undoOp()'s
 between startUndo() and endUndo().
*/

#include "undo.h"
#include "element.h"
#include "note.h"
#include "score.h"
#include "segment.h"
#include "measure.h"
#include "system.h"
#include "select.h"
#include "input.h"
#include "slur.h"
#include "clef.h"
#include "staff.h"
#include "layout.h"
#include "chord.h"
#include "sig.h"
#include "key.h"
#include "mscore.h"

extern Measure* tick2measure(int tick);

//
// for debugging:
//    keep in sync with enum UndoOp::UndoType
//
static const char* undoName[] = {
      "RemoveElement",      "AddElement",
      "InsertPart",        "RemovePart",
      "InsertStaff",       "RemoveStaff",
      "InsertSegStaff",    "RemoveSegStaff",
      "InsertMStaff",      "RemoveMStaff",
      "InsertMeasure",     "RemoveMeasure",
      "SortStaves",        "ToggleInvisible",
      "ChangeColor",       "ChangePitch",
      "ChangeSubtype",     "AddAccidental",
      "FlipStemDirection", "FlipSlurDirection",
      "ChangeTimeSig",
      };

static bool UNDO = false;

//---------------------------------------------------------
//   name
//---------------------------------------------------------

/**
 Return the UndoOp's name. For debugging only.
*/

const char* UndoOp::name() const
      {
      return undoName[type];
      }

//---------------------------------------------------------
//   Undo
//---------------------------------------------------------

Undo::Undo(const InputState& is, const Selection* s)
   : selection(*s)
      {
      inputState = is;
      }

//---------------------------------------------------------
//    startUndo
//---------------------------------------------------------

/**
 Start collecting low-level undo operations for a user-visible undo action.

 Called at the start of a GUI command.
*/

void Score::startUndo()
      {
      if (undoActive) {
            fprintf(stderr, "startUndo: already active\n");
            abort();
            }
      undoList.push_back(new Undo(*cis, sel));
      undoActive = true;
      }

//---------------------------------------------------------
//   endUndo
//---------------------------------------------------------

/**
 Stop collecting low-level undo operations for a user-visible undo action.

 Called at the end of a GUI command.
*/

void Score::endUndo()
      {
      if (!undoActive) {
            fprintf(stderr, "endUndo: not active\n");
            abort();
            }
//      printf("end undo: %d actions\n", undoList.back()->size());
      if (undoList.back()->empty()) {
            // nothing to undo
            delete undoList.back();
            undoList.pop_back();
            }
      else {
            setDirty(true);
            for (iUndo i = redoList.begin(); i != redoList.end(); ++i)
                  delete *i;
            redoList.clear();
            getAction("undo")->setEnabled(true);
            getAction("redo")->setEnabled(false);
            }
      undoActive = false;
      }

//---------------------------------------------------------
//   doUndo
//---------------------------------------------------------

/**
 Undo a user-visible undo action, containing one or more UndoOp's.
*/

void Score::doUndo()
      {
      Selection oSel(*sel);
      InputState oIs(*cis);
      sel->deselectAll(this);
      Undo* u = undoList.back();
      QMutableListIterator<UndoOp> i(*u);
      i.toBack();
      while (i.hasPrevious()) {
            UndoOp* op = &i.previous();
            processUndoOp(op, true);
            if (op->type == UndoOp::ChangeAccidental) {
                  // HACK:
                  // selection is not valid anymore because changeAccidental()
                  // changes the selected Accidental element
                  // u->selection.clear();
                  oSel.clear();
                  }
            }
      redoList.push_back(u); // put item on redo list
      undoList.pop_back();
      endUndoRedo(u);
      u->inputState = oIs;
      u->selection  = oSel;
      }

//---------------------------------------------------------
//   doRedo
//---------------------------------------------------------

/**
 Redo a user-visible undo action, containing one or more UndoOp's.
*/

void Score::doRedo()
      {
      Selection oSel(*sel);
      InputState oIs(*cis);
      sel->deselectAll(this);
      Undo* u = redoList.back();
      QMutableListIterator<UndoOp> i(*u);
      while (i.hasNext()) {
            UndoOp* op = &i.next();
            processUndoOp(op, false);
            if (op->type == UndoOp::ChangeAccidental) {
                  // HACK:
                  // selection is not valid anymore because changeAccidental()
                  // changes the selected Accidental element
                  u->selection.clear();
                  }
            }
      undoList.push_back(u); // put item on undo list
      redoList.pop_back();
printf("endUndoRedo\n");
      endUndoRedo(u);
      u->inputState = oIs;
      u->selection  = oSel;
      }

//---------------------------------------------------------
//   processUndoOp
//---------------------------------------------------------

/**
 Process a single low-level undo/redo operation.
*/

void Score::processUndoOp(UndoOp* i, bool undo)
      {
      UNDO = true;
// printf("Score::processUndoOp(i->type=%s, undo=%d)\n", i->name(), undo);
      switch(i->type) {
            case UndoOp::RemoveElement:
                  if (undo)
                        addElement(i->obj);
                  else
                        removeElement(i->obj);
                  break;
            case UndoOp::AddElement:
                  if (undo)
                        removeElement(i->obj);
                  else
                        addElement(i->obj);
                  break;
            case UndoOp::InsertPart:
                  if (undo)
                        removePart(i->part);
                  else
                        insertPart(i->part, i->val1);
                  break;
            case UndoOp::RemovePart:
                  if (undo)
                        insertPart(i->part, i->val1);
                  else
                        removePart(i->part);
                  break;
            case UndoOp::InsertStaff:
                  if (undo)
                        removeStaff(i->staff);
                  else
                        insertStaff(i->staff, i->val1);
                  break;
            case UndoOp::RemoveStaff:
                  if (undo)
                        insertStaff(i->staff, i->val1);
                  else
                        removeStaff(i->staff);
                  break;
            case UndoOp::InsertSegStaff:
                  if (undo)
                        i->segment->removeStaff(i->val1);
                  else
                        i->segment->insertStaff(i->val1);
                  break;
            case UndoOp::RemoveSegStaff:
                  if (undo)
                        i->segment->insertStaff(i->val1);
                  else
                        i->segment->removeStaff(i->val1);
                  break;
            case UndoOp::InsertMStaff:
                  if (undo)
                        i->measure->removeMStaff(i->mstaff, i->val1);
                  else
                        i->measure->insertMStaff(i->mstaff, i->val1);
                  break;
            case UndoOp::RemoveMStaff:
                  if (undo)
                        i->measure->insertMStaff(i->mstaff, i->val1);
                  else
                        i->measure->removeMStaff(i->mstaff, i->val1);
                  break;
            case UndoOp::InsertMeasure:
                  if (undo)
                        removeMeasure(i->measure->tick());
                  else
                        addMeasure(i->measure);
                  break;
            case UndoOp::RemoveMeasure:
                  if (undo)
                        addMeasure(i->measure);
                  else
                        removeMeasure(i->measure->tick());
                  break;
            case UndoOp::SortStaves:
                  if (undo)
                        sortStaves(i->di, i->si);
                  else
                        sortStaves(i->si, i->di);
                  break;
            case UndoOp::ToggleInvisible:
                  i->obj->setVisible(!i->obj->visible());
                  break;
            case UndoOp::ChangeColor:
                  {
                  QColor color = i->obj->color();
                  i->obj->setColor(i->color);
                  i->color = color;
                  }
                  break;
            case UndoOp::ChangePitch:
                  {
                  Note* note = (Note*)(i->obj);
                  int pitch  = note->pitch();
                  note->changePitch(i->val1);
                  i->val1 = pitch;
                  }
                  break;
            case UndoOp::ChangeAccidental:
                  {
                  Note* note = (Note*)(i->obj);
                  int accidental  = note->userAccidental();
                  note->changeAccidental(i->val1);
                  i->val1 = accidental;
                  }
                  break;
            case UndoOp::FlipStemDirection:
                  {
                  Chord* chord = (Chord*)(i->obj);
                  chord->setStemDirection(chord->isUp() ? DOWN : UP);
                  }
                  break;
            case UndoOp::FlipSlurDirection:
                  {
                  SlurTie* slur = (SlurTie*)(i->obj);
                  slur->setSlurDirection(slur->isUp() ? DOWN : UP);
                  }
                  break;
            case UndoOp::ChangeSubtype:
                  {
                  int st = i->obj->subtype();
                  int t = i->obj->type();
//                  printf("obj=%p t=%d curst=%d newst=%d\n",
//                         i->obj, t, st, i->val1);
                  i->obj->setSubtype(i->val1);
                  if (t == CLEF)
                        changeClef(i->obj->tick(), i->obj->staffIdx(), i->val1);
                  else if (t == KEYSIG)
                        changeKeySig(i->obj->tick(), i->val1);
                  i->val1 = st;
                  }
                  break;
            case UndoOp::ChangeTimeSig:
                  printf("UndoOp::ChangeTimeSig: todo\n");
                  break;
            }
      UNDO = FALSE;
      }

//---------------------------------------------------------
//   endUndoRedo
//---------------------------------------------------------

/**
 Common handling for ending undo or redo
*/

void Score::endUndoRedo(Undo* undo)
      {
      getAction("undo")->setEnabled(!undoList.empty());
      getAction("redo")->setEnabled(!redoList.empty());
      setDirty(true);

      *cis = undo->inputState;
      moveCursor();
      *sel = undo->selection;
      sel->update();
      layout();
      endCmd(false);
      }

//---------------------------------------------------------
//   checkUndoOp
//---------------------------------------------------------

/**
 Abort with error message if not in undo handling
*/

void Score::checkUndoOp()
      {
      if (!undoActive) {
            fprintf(stderr, "undoOp: undo not started\n");
            abort();
            }
      if (UNDO) {
            fprintf(stderr, "create undo op in undo/redo operation\n");
            abort();
            }
      }

//---------------------------------------------------------
//   undoOp
//---------------------------------------------------------

void Score::undoOp(UndoOp::UndoType type, Element* object, int idx)
      {
//      printf("Score::undoOp(type=%d, el=%p, idx=%d)\n", type, object, idx);
      checkUndoOp();
      UndoOp i;
      i.type = type;
      i.obj  = object;
      i.val1 = idx;
      undoList.back()->push_back(i);
      }

//---------------------------------------------------------
//   undoOp
//---------------------------------------------------------

void Score::undoOp(UndoOp::UndoType type, Element* object)
      {
      checkUndoOp();
      UndoOp i;
      i.type = type;
      i.obj  = object;
      undoList.back()->push_back(i);
      //
      // TEST: process REDO
      // TODO: all undoOp's should be extended to do the
      //    appropriate "undo" action
      //
      if (type == UndoOp::AddElement)
            processUndoOp(&i,false);
      }

//---------------------------------------------------------
//   undoOp
//---------------------------------------------------------

void Score::undoOp(UndoOp::UndoType type, Element* object, const QColor& color)
      {
      checkUndoOp();
      UndoOp i;
      i.type  = type;
      i.obj   = object;
      i.color = color;
      undoList.back()->push_back(i);
      }

//---------------------------------------------------------
//   undoOp
//---------------------------------------------------------

void Score::undoOp(UndoOp::UndoType type, Segment* seg, int staff)
      {
      checkUndoOp();
      UndoOp i;
      i.type    = type;
      i.segment = seg;
      i.val1     = staff;
      undoList.back()->push_back(i);
      }

void Score::undoOp(UndoOp::UndoType type, Part* part, int idx)
      {
      checkUndoOp();
      UndoOp i;
      i.type = type;
      i.part = part;
      i.val1  = idx;
      undoList.back()->push_back(i);
      }

void Score::undoOp(UndoOp::UndoType type, Staff* staff, int idx)
      {
      checkUndoOp();
      UndoOp i;
      i.type  = type;
      i.staff = staff;
      i.val1   = idx;
      undoList.back()->push_back(i);
      }

void Score::undoOp(UndoOp::UndoType type, Measure* m, MStaff s, int staff)
      {
      checkUndoOp();
      UndoOp i;
      i.type    = type;
      i.measure = m;
      i.mstaff  = s;
      i.val1     = staff;
      undoList.back()->push_back(i);
      }

void Score::undoOp(UndoOp::UndoType type, Measure* m)
      {
      checkUndoOp();
      UndoOp i;
      i.type    = type;
      i.measure = m;
      undoList.back()->push_back(i);
      }

void Score::undoOp(QList<int> si, QList<int> di)
      {
      checkUndoOp();
      UndoOp i;
      i.type = UndoOp::SortStaves;
      i.si   = si;
      i.di   = di;
      undoList.back()->push_back(i);
      }

void Score::undoOp(UndoOp::UndoType type, int a, int b)
      {
      checkUndoOp();
      UndoOp i;
      i.type = type;
      i.val1 = a;
      i.val2 = b;
      undoList.back()->push_back(i);
      }

//---------------------------------------------------------
//   addElement
//---------------------------------------------------------

/**
 Add \a element to its parent.

 Several elements (clef, keysig, timesig) need special handling, as they may cause
 changes throughout the score.
*/

void Score::addElement(Element* element)
      {
// printf("Score::addObject %p %s parent %s\n", element, element->name(), element->parent()->name());
      element->parent()->add(element);

      if (element->type() == CLEF) {
            int staffIdx = element->staffIdx();
            Clef* clef   = (Clef*) element;
            ClefList* ct = staff(staffIdx)->clef();
            int tick     = clef->tick();
            ct->setClef(clef->tick(), clef->subtype());

            //-----------------------------------------------
            //   move notes
            //-----------------------------------------------

            bool endFound = false;
            for (Measure* measure = _layout->first(); measure; measure = measure->next()) {
                  for (Segment* segment = measure->first(); segment; segment = segment->next()) {
                        int startTrack = staffIdx * VOICES;
                        int endTrack   = startTrack + VOICES;
                        for (int track = startTrack; track < endTrack; ++track) {
                              Element* ie = segment->element(track);
                              if (ie && ie->type() == CLEF && ie->tick() > tick) {
                                    endFound = true;
                                    break;
                                    }
                              }
                        measure->layoutNoteHeads(staffIdx);
                        if (endFound)
                              break;
                        }
                  if (endFound)
                        break;
                  }
            }
      else if (element->type() == KEYSIG) {
            // FIXME: update keymap here (and remove that from Score::changeKeySig)
            // but only after fixing redo for elements contained in segments

            // fixup all accidentals
            for (Measure* m = _layout->first(); m; m = m->next()) {
                  for (int staffIdx = 0; staffIdx < nstaves(); ++staffIdx) {
                              m->layoutNoteHeads(staffIdx);
                        }
                  }
            layout();
            }
/*      else if (element->type() == SLUR_SEGMENT) {
            SlurSegment* ss = (SlurSegment*)element;
            SlurTie* slur = ss->slurTie();
            slur->add(element);
            }
      */
      }

//---------------------------------------------------------
//   removeElement
//---------------------------------------------------------

/**
 Remove \a element from its parent.

 Several elements (clef, keysig, timesig) need special handling, as they may cause
 changes throughout the score.
*/

void Score::removeElement(Element* element)
      {
      Element* parent = element->parent();

// printf("Score::removeElement %p %s parent %p %s\n",
//   element, element->name(), parent, parent->name());

      parent->remove(element);
      if (element->type() == CLEF) {
            Clef* clef = (Clef*)element;
            int tick  = clef->tick();
            int staffIdx = clef->staffIdx();

            Staff* instr = staff(staffIdx);
            ClefList* ct = instr->clef();
            ct->erase(tick);

            //-----------------------------------------------
            //   move notes
            //-----------------------------------------------

            bool endFound = false;
            for (Measure* measure = _layout->first(); measure; measure = measure->next()) {
                  for (Segment* segment = measure->first(); segment; segment = segment->next()) {
                        int startTrack = staffIdx * VOICES;
                        int endTrack   = startTrack + VOICES;
                        for (int track = startTrack; track < endTrack; ++track) {
                              Element* ie = segment->element(track);
                              if (ie && ie->type() == CLEF && ie->tick() > tick) {
                                    endFound = true;
                                    break;
                                    }
                              }
                        measure->layoutNoteHeads(staffIdx);
                        if (endFound)
                              break;
                        }
                  if (endFound)
                        break;
                  }
            }
      else if (element->type() == TIMESIG) {
            // remove entry from siglist
            sigmap->del(element->tick());
            }
      else if (element->type() == KEYSIG) {
            // remove entry from keymap
            keymap->erase(element->tick());
            // fixup all accidentals
            for (Measure* m = _layout->first(); m; m = m->next()) {
                  for (int staffIdx = 0; staffIdx < nstaves(); ++staffIdx) {
                              m->layoutNoteHeads(staffIdx);
                        }
                  }
            layout();
            }
/*      else if (element->type() == SLUR_SEGMENT) {
            SlurSegment* ss = (SlurSegment*)element;
            SlurTie* slur = ss->slurTie();
            slur->remove(element);
            }
      */
      }

