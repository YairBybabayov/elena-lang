//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA IDE
//      Text class body
//                                             (C)2021-2022, by Aleksey Rakov
//---------------------------------------------------------------------------

#include "guicommon.h"
// --------------------------------------------------------------------------
#include "text.h"

using namespace elena_lang;

#ifdef __GNUG__

   constexpr auto SPACE_TEXT = " ";

#else

constexpr auto SPACE_TEXT = L" ";

#endif

// --- TextBookmarkReader ---

TextBookmarkReader :: TextBookmarkReader(Text* text)
{
   _text = text;
   _text->validateBookmark(_bookmark);
}

text_t TextBookmarkReader :: readLine(pos_t& length)
{
   return _text->getLine(_bookmark, length);
}

// --- TextBookmark ---

TextBookmark :: TextBookmark()
{
   _row = _column = 0;
   _virtual_column = 0;
   _status = BM_INVALID;
   _mode = EOLMode::None;
   _pagePosition = 0;
   _offset = 0;
   _length = NOTFOUND_POS;
}

TextBookmark :: ~TextBookmark()
{
}

void TextBookmark :: set(Pages* pages)
{
   // save the current position
   Point target = getCaret();

   // reset bookmark to the beginning
   _status = 0;
   _offset = 0;
   _length = NOTFOUND_POS;
   _page = pages->start();

   _row = _column = 0;
   _virtual_column = 0;

   // go to the current position
   moveTo(target.x, target.y);
}

void TextBookmark :: normalize()
{
   refreshStatus();

   if (_status == BM_EOP)
      goToNextPage();
}

bool TextBookmark :: goToPreviousPage(bool allowEmpty)
{
   do {
      if (!_page.first()) {
         --_page;
         _pagePosition -= (*_page).used;
         _offset = (*_page).used - 1;
      }
      else {
         _pagePosition = _offset = 0;
         refreshStatus();
         return false;
      }

   } while (!allowEmpty && (*_page).used == 0);

   if ((*_page).used == 0)
      _offset = 0;

   refreshStatus();
   return true;
}

bool TextBookmark :: goToNextPage(bool allowEmpty)
{
   do {
      if (!_page.last()) {
         _pagePosition += (*_page).used;
         _offset = 0;
         ++_page;
      }
      else {
         _status = BM_EOT;
         _offset = (*_page).used;

         return false;
      }

   } while (!allowEmpty && (*_page).used == 0);

   refreshStatus();
   return true;
}

bool TextBookmark :: go(disp_t disp, bool allowEmpty)
{
   if (disp < 0) {
      while (_offset < (size_t)-disp) {
         disp += _offset + 1;
         if (!goToPreviousPage(allowEmpty))
            return false;
      }
      _offset += disp;
   }
   else {
      while ((*_page).used < _offset + disp) {
         disp -= ((*_page).used - _offset);
         if (!goToNextPage(allowEmpty))
            return false;
      }
      _offset += disp;
   }
   refreshStatus();
   if (!allowEmpty && _status == BM_EOP) {
      goToNextPage();
   }
   return (_status != BM_EOT);
}

size_t TextBookmark :: length()
{
   if (_length == NOTFOUND_POS)
      _length = _column + seekEOL();

   return _length;
}

bool TextBookmark :: move(disp_t disp)
{
   if (disp < 0) {
      // go to the next page start if <end of the page>
      if (_status == BM_EOP)
         goToNextPage();

      while (disp < 0) {
         disp++;
         if (!prevChar(disp))
            return false;

         text_c ch = (*_page).text[_offset];
         if (isNewLineCh(ch)) {
            _length = NOTFOUND_POS;
            _row--;

            TextBookmark bm = *this;

            // if it is a CRLF - skip LF
            go(-1);
            if ((*_page).text[_offset] == 13) {
               disp++;
            }
            else {
               go(1);
            }

            bm.moveToPrevBOL();

            _length = bm.length();
            _column = (int)_length;
         }
         else if (ch == 0x9) {
            TextBookmark bm = *this;
            if (!bm.moveToPrevBOL())
               bm.moveToStart();

            bm.move(position() - bm.position());

            _column = bm._column;
         }
         else _column--;
      }
      return true;
   }
   else if (disp > 0) {
      // go to the next page start if <end of the page>
      if (_status == BM_EOT || (_status == BM_EOP && !goToNextPage()))
         return false;

      bool valid = true;
      while (valid && disp > 0) {
         disp--;

         text_c ch = (*_page).text[_offset];
         switch (ch) {
            case 13:
               _row++;
               _column = 0;
               _length = NOTFOUND_POS;

               if (!go(1))
                  return false;

               if ((*_page).text[_offset] == 10) {
                  valid = go(1);

                  disp--;
               }
               break;
            case 10:
               _row++;
               _column = 0;
               _length = NOTFOUND_POS;

               valid = go(1);
               break;
            case 9:
               _column += calcTabShift(_column, Text::TabSize);

               valid = go(1);
               break;
            default:
               valid = nextChar(disp);
               _column++;
               break;
         }
      }
      return valid;
   }
   else return true;
}

bool TextBookmark :: moveToClosestPosition(size_t destPosition)
{
   moveToStart();
   if (destPosition != 0) {
      while (position() + (*_page).used < destPosition) {
         _row += (*_page).rows;
         if (!goToNextPage())
            return false;
      }
      moveToNextBOL();
      return move(destPosition - position());
   }
   return true;
}

void TextBookmark :: moveToStart()
{
   while (goToPreviousPage());

   _column = 0;
   _row = 0;
   _length = NOTFOUND_POS;
}

void TextBookmark :: moveToClosestRow(int row)
{
   moveToStart();
   if (_row != row) {
      int closestRow = 0;
      // looking for the closest page
      while (closestRow + (*_page).rows < row) {
         closestRow += (*_page).rows;
         if (!goToNextPage()) {
            _row = closestRow;
            return;
         }
      }
      _offset = 0;

      // looking for the closest row
      _row = closestRow;
      moveToNextBOL();
   }
}

void TextBookmark :: moveToClosestColumn(int column)
{
   if (column > _column) {
      while (column > _column && move(1));
   }
   else if (column < _column) {
      while (column < _column && move(-1));
   }
}

bool TextBookmark :: moveToPrevBOL()
{
   _column = 0;
   _length = NOTFOUND_POS;

   if (_row == 0) {
      moveToStart();
      return false;
   }
   // find EOL
   if (!go(-1))
      return false;

   if ((*_page).text[_offset] == 10) {
      if (!go(-1))
         return false;

      _row--;
   }
   // move until BOL
   while ((*_page).text[_offset] != 10) {
      if (!go(-1))
         return false;
   }
   go(1);

   return true;
}

bool TextBookmark :: moveToNextBOL()
{
   if (_status == BM_EOT)
      return false;

   // go to the next page start if <end of the page>
   if (_status == BM_EOT || (_status == BM_EOP && !goToNextPage()))
      return false;

   // move until EOL
   while ((*_page).text[_offset] != 10) {
      if (!go(1))
         return false;
   }

   // go to the next position
   go(1);
   _column = 0;
   _length = NOTFOUND_POS;
   _row++;

   return true;
}

bool TextBookmark :: moveTo(int column, int row)
{
   if (_status == BM_INVALID)
      return false;

   refreshStatus();

   if (row == 0)
      moveToStart();

   // backward navigation
   if (row < _row) {
      // estimate if backward navigation is feasible (two times less than number of rows on the page)
      if ((_row - row) < ((*_page).rows << 1)) {
         while (_row > row && moveToPrevBOL());
      }
      else moveToClosestRow(row);
   }
   // estimate if forward navigation is feasible (two times less than number of rows on the page)
   if (((row - _row) > 2) && (row - _row) > ((*_page).rows << 1)) {
      moveToClosestRow(row);
   }
   while (row > _row && moveToNextBOL());

   if (_row != row)
      return false;

   _virtual_column = column;
   if (column > (int)length_pos())
      column = (int)length_pos();

   moveToClosestColumn(column);

   return true;
}

bool TextBookmark :: moveOn(disp_t disp)
{
   if (_status == BM_INVALID)
      return false;

   refreshStatus();

   bool valid = false;
   if (disp < 0) {
      disp = abs(disp);
      if ((pos_t)disp < (PAGE_SIZE << 2)) {
         valid = move(-disp);
      }
      else valid = moveToClosestPosition(position() - disp);
   }
   else {
      if (disp < (PAGE_SIZE << 2)) {
         valid = move(disp);
      }
      else valid = moveToClosestPosition(position() + disp);
   }
   _virtual_column = _column;

   return valid;
}

size_t TextBookmark :: seekEOL()
{
   TextBookmark bm = *this;

   // go to the next page start if <end of the page>
   if (bm._status == BM_EOT || (bm._status == BM_EOP && !bm.goToNextPage()))
      return 0;

   text_c ch = (*bm._page).text[bm._offset];
   while (!isNewLineCh(ch)) {
      if (!bm.move(1))
         break;

      ch = (*bm._page).text[bm._offset];
   }

   return bm._column - _column;
}

// --- Text static variables ---
int Text::TabSize = 4;

// --- Text ---

Text :: Text(EOLMode mode)
   : _watchers(nullptr)
{
   _mode = mode;
   _encoding = FileEncoding::UTF8;
}

Text :: ~Text()
{
   _pages.clear();
}

void Text :: attachWatcher(TextWatcherBase* watcher)
{
   _watchers.add(watcher);
}

void Text :: dettachWatcher(TextWatcherBase* watcher)
{
   _watchers.cut(watcher);
}

void Text :: refreshPage(Pages::Iterator& page)
{
   size_t used = (*page).used;
   text_c* text = (*page).text;

   (*page).rows = 0;
   for (size_t i = 0; i < used; i++) {
      if (text[i] == 10)
         (*page).rows++;
   }
}

void Text :: validateBookmark(TextBookmark& bookmark)
{
   if (!bookmark.isValid()) {
      bookmark._mode = _mode;
      bookmark.set(&_pages);
   }
}

void Text :: copyLineTo(TextBookmark& bookmark, TextWriter<text_c>& writer, pos_t length, bool stopOnEOL)
{
   validateBookmark(bookmark);

   bookmark.normalize();
   if (bookmark._status == BM_EOT)
      return;

   pos_t diff = (pos_t)bookmark.getVirtualDiff();
   if (diff > 0) {
      writer.fillText(SPACE_TEXT, 1, diff);

      diff = 0;
   }

   int col = bookmark._column;
   while (length > 0) {
      size_t offset = bookmark._offset;
      size_t count = (*bookmark._page).used - offset;
      if (count > length)
         count = length;

      const text_c* line = (*bookmark._page).text + offset;
      size_t i = 0;
      while (i < count) {
         if (stopOnEOL && TextBookmark::isNewLineCh(line[i])) {
            bookmark.moveOn(i);
            return;
         }
         else if (line[i] == '\t') {
            int disp = calcTabShift(col, Text::TabSize);
            writer.fillText(SPACE_TEXT, 1, disp + diff);
            diff = 0;
            col += disp;
            i++;
         }
         else {
            size_t chLen = TextBookmark::charLength(line, i);
            if (chLen > 1) {
               if (i + chLen < count) {
                  writer.write(&line[i], chLen);
                  i += (chLen - 1);
               }
               else break;
            }
            else writer.write(&line[i], 1u);

            col++;
            i++;
         }
      }

      if (!bookmark.moveOn(i))
         break;

      length -= i;
   }
}

void Text :: copyLineToX(TextBookmark& bookmark, TextWriter<text_c>& writer, pos_t length, int x)
{
   validateBookmark(bookmark);

   bookmark.normalize();
   if (bookmark._status == BM_EOT)
      return;

   pos_t diff = (pos_t)bookmark.getVirtualDiff();
   if (diff > 0) {
      writer.fillText(SPACE_TEXT, 1, diff);

      diff = 0;
   }

   int col = bookmark._column;
   while (length > 0 && col < x) {
      size_t offset = bookmark._offset;
      size_t count = (*bookmark._page).used - offset;
      if (count > length)
         count = length;

      const text_c* line = (*bookmark._page).text + offset;
      disp_t i = 0;
      while (i < count && col < x) {
         if (TextBookmark::isNewLineCh(line[i])) {
            bookmark.moveOn(i);
            return;
         }
         else if (line[i] == '\t') {
            int disp = calcTabShift(col, Text::TabSize);
            writer.fillText(SPACE_TEXT, 1, disp + diff);
            diff = 0;
            col += disp;
            i++;
         }
         else {
            size_t chLen = TextBookmark::charLength(line, i);
            if (chLen > 1) {
               if (i + chLen < count) {
                  writer.write(&line[i], chLen);
                  i += (chLen - 1);
               }
               else break;
            }
            else writer.write(&line[i], 1u);

            col++;
            i++;
         }
      }

      if (!bookmark.moveOn(i))
         break;

      length -= i;
   }
}

text_t Text :: getLine(TextBookmark& bookmark, pos_t& length)
{
   validateBookmark(bookmark);

   bookmark.normalize();
   if (bookmark._status == BM_EOT) {
      length = 0;
      return nullptr;
   }
   else {
      length = static_cast<pos_t>((*bookmark._page).used - bookmark._offset);

      return (*bookmark._page).text + bookmark._offset;
   }
}

void Text :: create()
{
   _pages.clear();

   _pages.add(Page());

   _rowCount = 0;
}

bool Text :: load(path_t path, FileEncoding encoding, bool autoDetecting)
{
   FileReader file(path, FileRBMode, encoding, autoDetecting);
   _encoding = file.encoding();

   if (!file.isOpen())
      return false;

   while (!file.eof()) {
      Page page;
      file.readText(page.text, PAGE_SIZE, page.used);

      _pages.add(page);
   }
   if (_pages.count() == 0) {
      create();
   }
   else _rowCount = 1;

   for(auto it = _pages.start(); !it.eof(); ++it) {
      refreshPage(it);

      _rowCount += (*it).rows;
   }

   return true;
}
