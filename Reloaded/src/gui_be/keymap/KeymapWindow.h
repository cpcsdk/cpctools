/*
 * Copyright 2004-2009 Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Sandor Vroemisse
 *		Jérôme Duval
 *		Alexandre Deckner, alex@zappotek.com
 *		Axel Dörfler, axeld@pinc-software.de.
 */
#ifndef KEYMAP_WINDOW_H
#define KEYMAP_WINDOW_H


#include <FilePanel.h>
#include <ListView.h>
#include <String.h>
#include <Window.h>

#include "Keymap.h"


class BMenu;
class BMenuBar;
class BMenuField;
class BTextControl;
class KeyboardLayoutView;
class KeymapListItem;


class KeymapWindow : public BWindow {
public:
								KeymapWindow();
								~KeymapWindow();

	virtual	bool				QuitRequested();
	virtual void				MessageReceived(BMessage* message);

protected:
			BMenuBar*			_CreateMenu();
			void				_AddKeyboardLayouts(BMenu* menu);
			status_t			_SetKeyboardLayout(const char* path);
 
			void				_UpdateButtons();

			void				_UseKeymap();
			void				_RevertKeymap();

			status_t			_GetCurrentKeymap(entry_ref& ref);
			BString				_GetActiveKeymapName();

			status_t			_GetSettings(BFile& file, int mode) const;
			status_t			_LoadSettings(BRect& frame,
									BString& keyboardLayout);
			status_t			_SaveSettings() const;

private:
			BButton*			fRevertButton;
			BMenu*				fLayoutMenu;
			BMenu*				fFontMenu;
			KeyboardLayoutView*	fKeyboardLayoutView;
 			
			Keymap				fCurrentMap;
			Keymap				fPreviousMap;
			Keymap				fAppliedMap;
			BString				fCurrentMapName;
 };

#endif	// KEYMAP_WINDOW_H
