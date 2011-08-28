/*
 * Copyright 2004-2010 Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Sandor Vroemisse
 *		Jérôme Duval
 *		Alexandre Deckner, alex@zappotek.com
 *		Axel Dörfler, axeld@pinc-software.de.
 */


#include "KeymapWindow.h"

#include <string.h>
#include <stdio.h>

#include <Alert.h>
#include <Button.h>
#include <Directory.h>
#include <File.h>
#include <FindDirectory.h>
#include <GroupLayoutBuilder.h>
#include <ListView.h>
#include <MenuBar.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Path.h>
#include <PopUpMenu.h>
#include <Screen.h>
#include <ScrollView.h>
#include <StringView.h>
#include <TextControl.h>

#include "KeyboardLayoutView.h"

#define B_TRANSLATE(x) x
#define B_TRANSLATE_SYSTEM_NAME(x) x

static const uint32 kChangeKeyboardLayout = 'cKyL';

static const uint32 kMsgMenuFontChanged = 'mMFC';

static const uint32 kMsgRevertKeymap = 'Rvrt';
static const uint32 kMsgKeymapUpdated = 'upkM';

static const char* kCurrentKeymapName = "(Current)";


KeymapWindow::KeymapWindow()
	:
	BWindow(BRect(80, 50, 880, 380), B_TRANSLATE_SYSTEM_NAME("Keymap"),
		B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS)
{
	SetLayout(new BGroupLayout(B_VERTICAL));

	fKeyboardLayoutView = new KeyboardLayoutView("layout");
	fKeyboardLayoutView->SetKeymap(&fCurrentMap);

	fRevertButton = new BButton("revertButton", B_TRANSLATE("Revert"),
		new BMessage(kMsgRevertKeymap));

	// controls pane
	AddChild(BGroupLayoutBuilder(B_VERTICAL)
		.Add(_CreateMenu())
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 10)
			.Add(BGroupLayoutBuilder(B_VERTICAL, 10)
				.Add(fKeyboardLayoutView)
				.AddGlue(0.0)
				.Add(BGroupLayoutBuilder(B_HORIZONTAL, 10)
					.AddGlue(0.0)
					.Add(fRevertButton)))
			.SetInsets(10, 10, 10, 10)));

	// Make sure the user keymap directory exists
	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.Append("Reloaded");

	entry_ref ref;
	get_ref_for_path(path.Path(), &ref);

	BDirectory userKeymapsDir(&ref);
	if (userKeymapsDir.InitCheck() != B_OK)
		create_directory(path.Path(), S_IRWXU | S_IRWXG | S_IRWXO);

	BRect windowFrame;
	BString keyboardLayout;
	_LoadSettings(windowFrame, keyboardLayout);
	_SetKeyboardLayout(keyboardLayout.String());

	ResizeTo(windowFrame.Width(), windowFrame.Height());
	MoveTo(windowFrame.LeftTop());

	// TODO: this might be a bug in the interface kit, but scrolling to
	// selection does not correctly work unless the window is shown.
	Show();
	Lock();

	path.Append("Keymap");
	get_ref_for_path(path.Path(), &ref);

	fCurrentMap.Load(ref);
	fPreviousMap = fCurrentMap;
	fAppliedMap = fCurrentMap;
	fCurrentMap.SetTarget(this, new BMessage(kMsgKeymapUpdated));

	_UpdateButtons();

	Unlock();
}


KeymapWindow::~KeymapWindow()
{
}


bool
KeymapWindow::QuitRequested()
{
	_SaveSettings();
	return true;
}


void
KeymapWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case B_SAVE_REQUESTED:
		{
			entry_ref ref;
			const char *name;
			if (message->FindRef("directory", &ref) == B_OK
				&& message->FindString("name", &name) == B_OK) {
				BDirectory directory(&ref);
				BEntry entry(&directory, name);
				entry.GetRef(&ref);
				fCurrentMap.SetName(name);
				fCurrentMap.Save(ref);
				fAppliedMap = fCurrentMap;
				fCurrentMapName = name;
			}
			break;
		}

		case kChangeKeyboardLayout:
		{
			entry_ref ref;
			BPath path;
			if (message->FindRef("ref", &ref) == B_OK)
				path.SetTo(&ref);

			_SetKeyboardLayout(path.Path());
			break;
		}

		case kMsgMenuFontChanged:
		{
			BMenuItem *item = fFontMenu->FindMarked();
			if (item != NULL) {
				BFont font;
				font.SetFamilyAndStyle(item->Label(), NULL);
				fKeyboardLayoutView->SetBaseFont(font);
			}
			break;
		}

		case kMsgRevertKeymap:
			_RevertKeymap();
			_UpdateButtons();
			break;

		case kMsgKeymapUpdated:
			_UpdateButtons();
			break;

		default:
			BWindow::MessageReceived(message);
			break;
	}
}


BMenuBar*
KeymapWindow::_CreateMenu()
{
	BMenuBar* menuBar = new BMenuBar(Bounds(), "menubar");
	BMenuItem* item;

	// Create keyboard layout menu
	fLayoutMenu = new BMenu(B_TRANSLATE("Layout"));
	fLayoutMenu->SetRadioMode(true);
	fLayoutMenu->AddItem(item = new BMenuItem(
		fKeyboardLayoutView->GetKeyboardLayout()->Name(),
		new BMessage(kChangeKeyboardLayout)));
	item->SetMarked(true);

	_AddKeyboardLayouts(fLayoutMenu);
	menuBar->AddItem(fLayoutMenu);

	// Create the Font menu
	fFontMenu = new BMenu(B_TRANSLATE("Font"));
	fFontMenu->SetRadioMode(true);
	int32 numFamilies = count_font_families();
	font_family family, currentFamily;
	font_style currentStyle;
	uint32 flags;

	be_plain_font->GetFamilyAndStyle(&currentFamily, &currentStyle);

	for (int32 i = 0; i < numFamilies; i++) {
		if (get_font_family(i, &family, &flags) == B_OK) {
			BMenuItem *item =
				new BMenuItem(family, new BMessage(kMsgMenuFontChanged));
			fFontMenu->AddItem(item);

			if (!strcmp(family, currentFamily))
				item->SetMarked(true);
		}
	}
	menuBar->AddItem(fFontMenu);

	return menuBar;
}


void
KeymapWindow::_AddKeyboardLayouts(BMenu* menu)
{
	directory_which dataDirectories[] = {
		B_USER_DATA_DIRECTORY,
		B_COMMON_DATA_DIRECTORY,
		B_BEOS_DATA_DIRECTORY
	};

	for (uint32 i = 0;
			i < sizeof(dataDirectories) / sizeof(dataDirectories[0]); i++) {
		BPath path;
		if (find_directory(dataDirectories[i], &path) != B_OK)
			continue;

		path.Append("Reloaded/KeyboardLayouts");

		BDirectory directory;
		if (directory.SetTo(path.Path()) == B_OK) {
			entry_ref ref;
			while (directory.GetNextRef(&ref) == B_OK) {
				if (menu->FindItem(ref.name) != NULL)
					continue;

				BMessage* message = new BMessage(kChangeKeyboardLayout);
				message->AddRef("ref", &ref);

				menu->AddItem(new BMenuItem(ref.name, message));
			}
		}
	}
}


status_t
KeymapWindow::_SetKeyboardLayout(const char* path)
{
	status_t status = B_OK;

	if (path != NULL && path[0] != '\0') {
		status = fKeyboardLayoutView->GetKeyboardLayout()->Load(path);
		if (status == B_OK) {
			// select item
			for (int32 i = fLayoutMenu->CountItems(); i-- > 0;) {
				BMenuItem* item = fLayoutMenu->ItemAt(i);
				BMessage* message = item->Message();
				entry_ref ref;
				if (message->FindRef("ref", &ref) == B_OK) {
					BPath layoutPath(&ref);
					if (layoutPath == path) {
						item->SetMarked(true);
						break;
					}
				}
			}
		}
	}

	if (path == NULL || status != B_OK) {
		fKeyboardLayoutView->GetKeyboardLayout()->SetDefault();
		fLayoutMenu->ItemAt(0)->SetMarked(true);
	}

	// Refresh currently set layout
	fKeyboardLayoutView->SetKeyboardLayout(
		fKeyboardLayoutView->GetKeyboardLayout());

	return status;
}


void
KeymapWindow::_UpdateButtons()
{
	if (fCurrentMap != fAppliedMap) {
		fCurrentMap.SetName(kCurrentKeymapName);
		_UseKeymap();
	}

	fRevertButton->SetEnabled(fCurrentMap != fPreviousMap);
}


//!	Saves previous map to the "Key_map" file.
void
KeymapWindow::_RevertKeymap()
{
	entry_ref ref;
	_GetCurrentKeymap(ref);

	status_t status = fPreviousMap.Save(ref);
	if (status != B_OK) {
		printf("error when saving keymap: %s", strerror(status));
		return;
	}

	fPreviousMap.Use();
	fCurrentMap.Load(ref);
	fAppliedMap = fCurrentMap;

	fKeyboardLayoutView->SetKeymap(&fCurrentMap);

	fCurrentMapName = _GetActiveKeymapName();
}


//!	Saves current map to the "Key_map" file.
void
KeymapWindow::_UseKeymap()
{
	entry_ref ref;
	_GetCurrentKeymap(ref);

	status_t status = fCurrentMap.Save(ref);
	if (status != B_OK) {
		printf("error when saving : %s", strerror(status));
		return;
	}

	fCurrentMap.Use();
	fAppliedMap.Load(ref);

	fCurrentMapName = _GetActiveKeymapName();
}

status_t
KeymapWindow::_GetCurrentKeymap(entry_ref& ref)
{
	BPath path;
	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) != B_OK)
		return B_ERROR;

	path.Append("Key_map");

	return get_ref_for_path(path.Path(), &ref);
}


BString
KeymapWindow::_GetActiveKeymapName()
{
	BString mapName = kCurrentKeymapName;	// safe default

	entry_ref ref;
	_GetCurrentKeymap(ref);

	BNode node(&ref);

	if (node.InitCheck() == B_OK)
		node.ReadAttrString("keymap:name", &mapName);

	return mapName;
}


status_t
KeymapWindow::_GetSettings(BFile& file, int mode) const
{
	BPath path;
	status_t status = find_directory(B_USER_SETTINGS_DIRECTORY, &path,
		(mode & O_ACCMODE) != O_RDONLY);
	if (status != B_OK)
		return status;

	path.Append("Keymap settings");

	return file.SetTo(path.Path(), mode);
}


status_t
KeymapWindow::_LoadSettings(BRect& windowFrame, BString& keyboardLayout)
{
	BScreen screen(this);

	windowFrame.Set(-1, -1, 799, 329);
	// See if we can use a larger default size
	if (screen.Frame().Width() > 1200) {
		windowFrame.right = 899;
		windowFrame.bottom = 349;
	}

	keyboardLayout = "";

	BFile file;
	status_t status = _GetSettings(file, B_READ_ONLY);
	if (status == B_OK) {
		BMessage settings;
		status = settings.Unflatten(&file);
		if (status == B_OK) {
			BRect frame;
			if (settings.FindRect("window frame", &frame) == B_OK)
				windowFrame = frame;

			settings.FindString("keyboard layout", &keyboardLayout);
		}
	}

	if (!screen.Frame().Contains(windowFrame)) {
		// Make sure the window is not larger than the screen
		if (windowFrame.Width() > screen.Frame().Width())
			windowFrame.right = windowFrame.left + screen.Frame().Width();
		if (windowFrame.Height() > screen.Frame().Height())
			windowFrame.bottom = windowFrame.top + screen.Frame().Height();

		// Make sure the window is on screen (and center if it isn't)
		if (windowFrame.left < screen.Frame().left
			|| windowFrame.right > screen.Frame().right
			|| windowFrame.top < screen.Frame().top
			|| windowFrame.bottom > screen.Frame().bottom) {
			windowFrame.OffsetTo(BAlert::AlertPosition(windowFrame.Width(),
				windowFrame.Height()));
		}
	}

	return status;
}


status_t
KeymapWindow::_SaveSettings() const
{
	BFile file;
	status_t status
		= _GetSettings(file, B_WRITE_ONLY | B_ERASE_FILE | B_CREATE_FILE);
	if (status != B_OK)
		return status;

	BMessage settings('keym');
	settings.AddRect("window frame", Frame());

	BMenuItem* item = fLayoutMenu->FindMarked();
	entry_ref ref;
	if (item != NULL && item->Message()->FindRef("ref", &ref) == B_OK) {
		BPath path(&ref);
		if (path.InitCheck() == B_OK)
			settings.AddString("keyboard layout", path.Path());
	}

	return settings.Flatten(&file);
}
