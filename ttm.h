/*
 *  This file is part of 'Johnny Reborn'
 *
 *  An open-source engine for the classic
 *  'Johnny Castaway' screensaver by Sierra.
 *
 *  Copyright (C) 2019 Jeremie GUILLAUME
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

extern int ttmDx;
extern int ttmDy;

uint32 ttmFindTag(struct TTtmSlot *ttmSlot, uint16 reqdTag);
void ttmLoadTtm(struct TTtmSlot *ttmSlot, char *ttmName);
void ttmInitSlot(struct TTtmSlot *ttmSlot);
void ttmResetSlot(struct TTtmSlot *ttmSlot);
void ttmPlay(struct TTtmThread *ttmThread);

