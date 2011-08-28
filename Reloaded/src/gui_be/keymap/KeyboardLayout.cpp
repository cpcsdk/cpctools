/*
 * Copyright 2009, Axel Dörfler, axeld@pinc-software.de.
 * Distributed under the terms of the MIT License.
 */


#include "KeyboardLayout.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <File.h>
#include <InterfaceDefs.h>


#undef TRACE

//#define TRACE_LAYOUT
#ifdef TRACE_LAYOUT
#	define TRACE(x...) printf(x)
#else
#	define TRACE(x...)
#endif


KeyboardLayout::KeyboardLayout()
	:
	fKeys(NULL),
	fKeyCount(0),
	fKeyCapacity(0),
	fIndicators(5, true),
	fIsDefault(true)
{
	SetDefault();
}


KeyboardLayout::~KeyboardLayout()
{
	free(fKeys);
}


const char*
KeyboardLayout::Name()
{
	return fName.String();
}


int32
KeyboardLayout::CountKeys()
{
	return fKeyCount;
}


Key*
KeyboardLayout::KeyAt(int32 index)
{
	if (index < 0 || index >= fKeyCount)
		return NULL;

	return &fKeys[index];
}


int32
KeyboardLayout::CountIndicators()
{
	return fIndicators.CountItems();
}


Indicator*
KeyboardLayout::IndicatorAt(int32 index)
{
	return fIndicators.ItemAt(index);
}


BRect
KeyboardLayout::Bounds()
{
	return fBounds;
}


BSize
KeyboardLayout::DefaultKeySize()
{
	return fDefaultKeySize;
}


int32
KeyboardLayout::IndexForModifier(int32 modifier)
{
	return 0;
}


status_t
KeyboardLayout::Load(const char* path)
{
	entry_ref ref;
	get_ref_for_path(path, &ref);

	return Load(ref);
}


status_t
KeyboardLayout::Load(entry_ref& ref)
{
	BFile file;
	status_t status = file.SetTo(&ref, B_READ_ONLY);
	if (status != B_OK)
		return status;

	off_t size;
	status = file.GetSize(&size);
	if (status != B_OK)
		return status;

	if (size > 65536) {
		// We don't accept files larger than this
		return B_BAD_VALUE;
	}

	char* data = (char*)malloc(size + 1);
	if (data == NULL)
		return B_NO_MEMORY;

	ssize_t bytesRead = file.Read(data, size);
	if (bytesRead != size) {
		free(data);

		if (bytesRead < 0)
			return bytesRead;

		return B_IO_ERROR;
	}

	data[size] = '\0';

	status = _InitFrom(data);
	if (status == B_OK)
		fIsDefault = false;

	free(data);

	return status;
}


void
KeyboardLayout::SetDefault()
{
	// The keyboard layout description language defines the position,
	// size, shape, and scancodes of the keys on the keyboard, row by
	// row.
	// You can define variables that are substituted in the row
	// descriptions. Variables are always prefixed with a '$' symbol.

	// On top level, only two different terms are accepted: value pairs,
	// and row descriptions.
	// Value pairs are in the form "<name> = <value>". There are only two
	// predefined names at this moment "name" that specifies the name of
	// a layout, and "default-size" that specifies the size of keys when
	// no specific size is given. Also, variables can be specified this
	// way.

	// Row descriptions are embedded in the '[' and ']' brackets.
	// The first term within a row is the position of the row, written as
	// "<x>,<y>;" - the delimiter between terms/keys is usually the
	// semi-colon. After the initial position, key descriptions are expected
	// until the row is closed with a ']'.

	// A key description is of the form "<shape>:<scancodes>", where <shape>
	// is combined of the shape of the character, and its size, written as
	// "<kind><width>,<height>[,<second-row-width>]".
	// The kind can either be 'r' (the default, can also be omitted) for
	// rectangular keys, or 'l' for the enter key. Additionally, you can use
	// the 'd' character to mark dark keys. The size can be omitted completely,
	// in which case the defined "default-size" is used. The default size is
	// also used to determine the height of the first row of the enter key;
	// the "<second-row-width>" specifier is only valid for enter keys, too.

	// The scancodes can be written in different ways:
	// 1) "+<count>": adds <count> keys, the scancodes will be used relative
	//    to the previous keys.
	// 2) "<first>-<last>": keys are added for scancode <first> to scancode
	//    <last>.
	// 3) "<first>+<count>": one key with the <first> scancode is added, plus
	//    <count> ones with relative scancode from that one.

	// Finally, you can also define LED indicator. Those can be made instead
	// of a key, it accepts a size, but no shape modifiers. Also, instead of
	// a scancode, the name of the modifer is given, currently only the
	// following are allowed: "led-num", "led-caps", and "led-scroll".

	// See for examples in the default layout below.
	
	static const char* k6128Layout =
		"name = CPC 6128 / Amstrad Plus\n"
		"default-size = 10,10\n"
		" [ 0,0; d:0; :+12; d:+2; :+3; ]\n"
		" [ 0,10; d15,10:+1; :+12; dl15,20,12:+1; :+3; ]\n"
		" [ 0,20; d17,10:+1; :+12; 13,10:-; :+3; ]\n"
		" [ 0,30; d23,10:+1; :+11; d17,10:+1; :+1; d:+1; :+1; ]\n"
		" [ 0,40; d23,10:+1; d17,10:+1; 80,10:+1; d30,10:+1; d:+3; ]\n";
		
	_InitFrom(k6128Layout);
	fIsDefault = true;
}


void
KeyboardLayout::_FreeKeys()
{
	free(fKeys);
	fKeys = NULL;
	fKeyCount = 0;
	fKeyCapacity = 0;
	fBounds = BRect();

	fIndicators.MakeEmpty();
}


void
KeyboardLayout::_Error(const parse_state& state, const char* reason, ...)
{
	va_list args;
	va_start(args, reason);

	fprintf(stderr, "Syntax error in line %ld: ", state.line);
	vfprintf(stderr, reason, args);
	fprintf(stderr, "\n");

	va_end(args);
}


void
KeyboardLayout::_AddAlternateKeyCode(Key* key, int32 modifier, int32 code)
{
	if (key == NULL)
		return;

	// TODO: search for free spot
}


bool
KeyboardLayout::_AddKey(const Key& key)
{
	TRACE("    add %ld (%g,%g)\n", key.code, key.frame.left, key.frame.top);
	if (fKeyCount + 1 > fKeyCapacity) {
		// enlarge array
		int32 newCapacity = fKeyCapacity + 32;
		Key* newKeys = (Key*)realloc(fKeys, newCapacity * sizeof(Key));
		if (newKeys == NULL)
			return false;

		fKeys = newKeys;
		fKeyCapacity = newCapacity;
	}

	fKeys[fKeyCount] = key;
	fKeyCount++;

	fBounds = key.frame | fBounds;

	return true;
}


void
KeyboardLayout::_SkipCommentsAndSpace(parse_state& state, const char*& data)
{
	while (data[0] != '\0') {
		while (isspace(data[0])) {
			if (data[0] == '\n')
				state.line++;
			data++;
		}

		if (data[0] == '#') {
			// skip comment
			while (data[0] != '\0' && data[0] != '\n') {
				data++;
			}
		} else
			break;
	}
}


void
KeyboardLayout::_Trim(BString& string, bool stripComments)
{
	// Strip leading spaces
	int32 i = 0;
	while (isspace(string[i])) {
		i++;
	}
	if (i > 0)
		string.Remove(0, i);

	// Remove comments
	if (stripComments) {
		i = string.FindFirst('#');
		if (i >= 0)
			string.Truncate(i);
	}

	// Strip trailing spaces
	i = string.Length() - 1;
	while (i > 0 && isspace(string[i])) {
		i--;
	}
	string.Truncate(i + 1);
}


bool
KeyboardLayout::_GetPair(const parse_state& state, const char*& data,
	BString& name, BString& value)
{
	// Get name
	name = "";
	while (data[0] != '\0' && data[0] != '=') {
		name += data[0];
		data++;
	}

	if (data[0] != '=') {
		_Error(state, "no valid pair");
		return false;
	}

	// Skip sign
	data++;

	// Get value
	value = "";
	while (data[0] != '\0' && data[0] != '\n') {
		value += data[0];
		data++;
	}

	_Trim(name, false);
	_Trim(value, true);

	return true;
}


bool
KeyboardLayout::_AddKeyCodes(const parse_state& state, BPoint& rowLeftTop,
	Key& key, const char* data, int32& lastCount)
{
	if (data[0] == '-') {
		// no key, just free space
		int32 num = strtoul(data + 1, NULL, 0);
		if (num < 1)
			num = 1;
		else if (num > 32) {
			_Error(state, "empty key count too large");
			return false;
		}

		key.frame.OffsetTo(rowLeftTop);
		rowLeftTop.x = key.frame.left + key.frame.Width() * num;
		return true;
	}

	int32 modifier = 0;

	if (isalpha(data[0])) {
		bool led = false;
		if (!strcmp("led-caps", data)) {
			modifier = B_CAPS_LOCK;
			led = true;
		} else if (!strcmp("led-num", data)) {
			modifier = B_NUM_LOCK;
			led = true;
		} else if (!strcmp("led-scroll", data)) {
			modifier = B_SCROLL_LOCK;
			led = true;
		} else {
			// TODO: get modifier (ie. "num")
		}

		if (led) {
			key.frame.OffsetTo(rowLeftTop);
			rowLeftTop.x = key.frame.right;
			fBounds = key.frame | fBounds;

			Indicator* indicator = new(std::nothrow) Indicator;
			if (indicator != NULL) {
				indicator->modifier = modifier;
				indicator->frame = key.frame;

				fIndicators.AddItem(indicator);
			}
			return true;
		}
	}

	int32 first;
	int32 last;
	int32 num = 1;

	if (data[0] == '+') {
		num = strtoul(data + 1, NULL, 0);
		if (num < 1)
			num = 1;
		else if (num > 32) {
			_Error(state, "key count too large");
			return false;
		}

		if (fKeyCount > 0)
			first = fKeys[fKeyCount - 1].code + 1;
		else
			first = 1;

		last = first + num - 1;
	} else {
		char* end;
		first = strtoul(data, &end, 0);
		last = first;

		if (end[0] == '-') {
			last = strtoul(end + 1, NULL, 0);
			if (first > last) {
				_Error(state, "invalid key code specifier");
				return false;
			}

			num = last - first;
		} else if (end[0] == '+') {
			num = strtoul(end + 1, NULL, 0) + 1;
			last = first + num - 1;
		} else if (end[0] != '\0') {
			_Error(state, "invalid key range");
			return false;
		}
	}

	if (lastCount != 0) {
		// update existing keys
		if (lastCount != num) {
			_Error(state, "modifier key mismatch");
			return false;
		}

		for (int32 i = fKeyCount - num; i < fKeyCount; i++, first++) {
			Key* key = KeyAt(i);

			_AddAlternateKeyCode(key, modifier, first);
		}
	} else {
		// add new keys
		for (int32 i = first; i <= last; i++) {
			key.code = i;

			// "layout"
			key.frame.OffsetTo(rowLeftTop);
			rowLeftTop.x = key.frame.right;

			_AddKey(key);
		}

		lastCount = num;
	}

	return true;
}


bool
KeyboardLayout::_GetSize(const parse_state& state, const char* data,
	float& x, float& y, float* _secondRow)
{
	if (data[0] == '\0') {
		// default size
		x = fDefaultKeySize.width;
		y = fDefaultKeySize.height;
		return true;
	}

	float secondRow = 0;
	int num = sscanf(data, "%g,%g,%g", &x, &y, &secondRow);
	if (num < 2) {
		_Error(state, "invalid size");
		return false;
	}

	if (_secondRow != NULL)
		*_secondRow = secondRow;
	return true;
}


bool
KeyboardLayout::_GetShape(const parse_state& state, const char* data, Key& key)
{
	// the default
	key.shape = kRectangleKeyShape;
	key.dark = false;

	while (isalpha(data[0])) {
		switch (tolower(data[0])) {
			case 'r':
				key.shape = kRectangleKeyShape;
				break;
			case 'c':
				key.shape = kCircleKeyShape;
				break;
			case 'l':
				key.shape = kEnterKeyShape;
				break;
			case 'd':
				key.dark = true;
				break;

			default:
				_Error(state, "unknown shape specifier '%c'", data[0]);
				return false;
		}

		data++;
	}

	float width, height;
	if (!_GetSize(state, data, width, height, &key.second_row))
		return false;

	// don't accept second row with anything but kEnterKeyShape
	if ((key.shape != kEnterKeyShape && key.second_row != 0)
		|| (key.shape == kEnterKeyShape && key.second_row == 0)) {
		_Error(state, "shape size mismatch");
		return false;
	}

	key.frame.left = 0;
	key.frame.top = 0;
	key.frame.right = width;
	key.frame.bottom = height;

	return true;
}


/*!	Returns the term delimiter expected in a certain parse mode. */
const char*
KeyboardLayout::_Delimiter(parse_mode mode)
{
	switch (mode) {
		default:
		case kSize:
			return "";
		case kRowStart:
			return ";";

		case kKeyShape:
			return ":";
		case kKeyCodes:
			return ";:";
	}
}


bool
KeyboardLayout::_GetTerm(const char*& data, const char* delimiter,
	BString& term, bool closingBracketAllowed)
{
	// Get term
	term = "";
	while (data[0] != '\0' && strchr(delimiter, data[0]) == NULL
		&& data[0] != '\n' && data[0] != '#'
		&& (!closingBracketAllowed || data[0] != ']')) {
		term += data[0];
		data++;
	}

	if (data[0] == '\0' && delimiter[0])
		return false;

	_Trim(term, true);
	return true;
}


bool
KeyboardLayout::_SubstituteVariables(BString& term, VariableMap& variables,
	BString& unknown)
{
	while (true) {
		int32 index = term.FindFirst('$');
		if (index < 0)
			break;

		// find variable name

		VariableMap::iterator iterator = variables.begin();
		VariableMap::iterator best = variables.end();
		int32 bestLength = 0;

		for (; iterator != variables.end(); iterator++) {
			const BString& name = iterator->first;
			if (!name.Compare(&term[index], name.Length())
				&& name.Length() > bestLength) {
				best = iterator;
				bestLength = name.Length();
			}
		}

		if (best != variables.end()) {
			// got one, replace it
			term.Remove(index, bestLength);
			term.Insert(best->second.String(), index);
		} else {
			// variable has not been found
			unknown = &term[index];
			int32 length = 1;
			while (isalpha(unknown[length])) {
				length++;
			}
			unknown.Truncate(length);
			return false;
		}
	}

	return true;
}


bool
KeyboardLayout::_ParseTerm(const parse_state& state, const char*& data,
	BString& term, VariableMap& variables)
{
	if (!_GetTerm(data, _Delimiter(state.mode), term,
			state.mode == kKeyCodes)) {
		_Error(state, state.mode == kRowStart
			? "no valid row start" : "invalid term");
		return false;
	}

	BString unknown;
	if (!_SubstituteVariables(term, variables, unknown)) {
		_Error(state, "Unknown variable \"%s\"", unknown.String());
		return false;
	}

	return true;
}


/*!	Initializes the keyboard layout from the data given.
	The string has to be a valid keyboard layout description, otherwise
	an error is returned.
*/
status_t
KeyboardLayout::_InitFrom(const char* data)
{
	_FreeKeys();

	VariableMap variables;
	BPoint rowLeftTop;
	int32 lastKeyCount = 0;
	Key key;

	parse_state state = {kPairs, 1};

	while (data[0] != '\0') {
		_SkipCommentsAndSpace(state, data);

		if (data[0] == '[') {
			state.mode = kRowStart;

			rowLeftTop = BPoint(0, 0);
			data++;
			continue;
		} else if (data[0] == '\0')
			break;

		switch (state.mode) {
			case kPairs:
			{
				BString name;
				BString value;
				if (!_GetPair(state, data, name, value))
					return B_BAD_VALUE;

				TRACE("<%s> = <%s>\n", name.String(), value.String());
				if (name == "name")
					fName = value;
				else if (name == "default-size") {
					const char* valueString = value.String();
					parse_state tempState = {kSize, state.line};
					BString term;
					if (!_ParseTerm(tempState, valueString, term, variables))
						return B_BAD_VALUE;

					TRACE("  size = %s\n", term.String());
					if (!_GetSize(state, term.String(), fDefaultKeySize.width,
							fDefaultKeySize.height))
						return B_BAD_VALUE;
				} else if (name[0] == '$')
					variables[name] = value;
				break;
			}

			case kRowStart:
			case kKeyShape:
			case kKeyCodes:
			{
				if (data[0] == ']') {
					if (state.mode == kKeyShape) {
						state.mode = kPairs;
						data++;
						continue;
					}
					_Error(state, "unexpected row closing bracket");
					return B_BAD_VALUE;
				}

				BString term;
				if (!_ParseTerm(state, data, term, variables))
					return B_BAD_VALUE;

				switch (state.mode) {
					case kRowStart:
						if (!_GetSize(state, term.String(), rowLeftTop.x,
								rowLeftTop.y))
							return B_BAD_VALUE;

						TRACE("row: %s (%g:%g)\n", term.String(), rowLeftTop.x,
							rowLeftTop.y);

						state.mode = kKeyShape;
						break;
					case kKeyShape:
						memset(&key, 0, sizeof(Key));
						if (!_GetShape(state, term.String(), key))
							return B_BAD_VALUE;

						TRACE("  shape: %s (%g:%g:%g)\n", term.String(),
							key.frame.Width(), key.frame.Height(),
							key.second_row);

						lastKeyCount = 0;
						state.mode = kKeyCodes;
						break;
					case kKeyCodes:
						TRACE("   raw key: %s\n", term.String());

						if (!_AddKeyCodes(state, rowLeftTop, key, term.String(),
								lastKeyCount))
							return B_BAD_VALUE;

						if (data[0] != ':')
							state.mode = kKeyShape;
						break;

					default:
						break;
				}
				if (data[0] != ']' && data[0] != '\0')
					data++;
				break;
			}

			default:
				return B_BAD_VALUE;
		}
	}

	return B_OK;
}

