/**
* The Student Dormitory Information Management System
* 
* Written by Yu Feixia and their team in december 2023
*/

//------------------------
//~Welcome to use Mr.Yu Feixia and their team's student dormitory management system
//| This system uses instructions with parameters to achieve interactivity
//~~~~~~~~~~~~~~~~~~~~~~~~
//+ DISPLAY
//| -Display this database to this screen
//
//+ INSERT[StudentID][RoomNumber][Name]
//| -Insert a student information item
//| -Note: Student ID needs to be unique
//
//+ REMOVE[StudentId]
//| -Delete a student information item
//
//+ CHANGE[StudentID][RoomNumber][Name]
//| -Change other information for the current student ID
//
//+ FIND[DataItem]
//| -Find a list in the database with any item equal to[DataItem]
//
//+ SORT[-S | -R | -N]
//| -Sort the different keys in this database
//| -The above three parameter options correspond to : StudentID, RoomNumber, and Name
//
//+ SAVE[FileName]
//| -Save the current database to the specified file
//
//+ EXIT
//| -Exit The Student Dormitory Management System
//------------------------


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>

#pragma warning (disable : 4996)

// Object-oriented C language
typedef enum bool { false = 0, true = 1 } bool;

typedef char  byte;
typedef byte* iterator;

static const void* const nullptr = (void*)0;

#define class                                     struct
#define class_def                                 typedef struct
#define class_func(className, funcName)           class_##className##__##funcName
#define class_init(className, object, ...)        class_func(className, init)(&(object), ##__VA_ARGS__)
#define class_del(className, object)              class_func(className, del) (&(object))

#define class_funcdef(className, funcName, ...)   class_##className##__##funcName(className* this, ##__VA_ARGS__)
#define class_initdef(className, ...)             void class_funcdef(className, init, ##__VA_ARGS__)
#define class_deldef(className)                   void class_funcdef(className, del)

#define error_out(format, ...)                    do { fprintf(stderr, format, ##__VA_ARGS__); fputc('\n', stderr); } while (false)

#ifdef _WIN64
#	define SIZE_F "%llu"
#else
#	define SIZE_F "%u"
#endif

static void* new(size_t _Size)
{
	void* ptr = malloc(_Size);
	if (ptr == nullptr)
	{
		error_out("MemoryError: The memory allocation failed, the size is " SIZE_F ".", _Size);
		exit(EXIT_FAILURE);
	}
	else
	{
		memset(ptr, 0, _Size);
		return ptr;
	}
}
static void* renew(void* _Object, size_t _Size)
{
	void* ptr = realloc(_Object, _Size);
	if (ptr == nullptr)
	{
		error_out("MemoryError: The memory allocation failed, the size is " SIZE_F ".", _Size);
		free(_Object);
		exit(EXIT_FAILURE);
	}
	else return ptr;
}
static void delete(void* object)
{
	free(object);
}

void* ptrpos(const void* const pointer, size_t index, size_t elementSize)
{
	return ((iterator)(pointer) + ((index) * (elementSize)));
}

class_def vector
{
	void*  elements;
	size_t size;

	size_t elementSize;
	size_t capacity;
} vector;
class_initdef(vector, size_t _ElementSize)
{
	this->size = 0;
	this->elementSize = _ElementSize;
	this->capacity = 1;
	this->elements = new(this->capacity * this->elementSize);
}
class_deldef(vector)
{
	delete(this->elements);
	this->size = 0;
}

// vector::iterator::begin()
iterator class_funcdef(vector, begin)
{
	return this->elements;
}

// vector::iterator::end()
iterator class_funcdef(vector, end)
{
	return ptrpos(this->elements, this->size, this->elementSize);
}

void class_funcdef(vector, expand)
{
	this->capacity = (size_t)ceil((double)this->capacity * 1.5);
	this->elements = renew(this->elements, this->capacity * this->elementSize);
}

void class_funcdef(vector, push_back, const void* _Value, size_t _Size)
{
	while (this->size + _Size > this->capacity)
	{
		class_func(vector, expand)(this);
	}

	memcpy(class_func(vector, end)(this), _Value, _Size * this->elementSize);
	this->size += _Size;

	return;
}

void class_funcdef(vector, erase, size_t _Pos, size_t _Size)
{
	memcpy(
		ptrpos(this->elements, _Pos, this->elementSize),
		ptrpos(this->elements, _Pos + _Size, this->elementSize),
		class_func(vector, end)(this) - (iterator)ptrpos(this->elements, _Pos + _Size, this->elementSize));

	this->size -= _Size;
}

void class_funcdef(vector, clear)
{
	this->size = 0;
	return;
}

size_t class_funcdef(vector, find, size_t start, const void* _Value)
{
	iterator beginptr = class_func(vector, begin)(this);
	iterator endptr = class_func(vector, end)(this);
	for (iterator i = ptrpos(this->elements, start, this->elementSize); i != endptr; i += this->elementSize)
	{
		if (memcmp(i, _Value, this->elementSize) == 0)
		{
			return (i - beginptr) / this->elementSize;
		}
	}
	return this->size;
}

void class_funcdef(vector, sort, _CoreCrtNonSecureSearchSortCompareFunction _Func)
{
	qsort(this->elements, this->size, this->elementSize, _Func);
}

size_t move_to_next_not_blank(vector _Str, size_t _CurrentIndex)
{
	for (; _CurrentIndex != _Str.size; ++_CurrentIndex)
	{
		if (isblank(((char*)_Str.elements)[_CurrentIndex]) == false)
		{
			break;
		}
	}
	return _CurrentIndex;
}

// Return a vector<vector<char>>
vector split(vector _Str)
{
	vector result;
	class_init(vector, result, sizeof(vector));

	// vector<char>, aka string
	vector ct;
	class_init(vector, ct, sizeof(char));
	
	for (size_t i = 0; i < _Str.size; ++i)
	{
		if (isblank(((char*)_Str.elements)[i]))
		{
			class_func(vector, push_back)(&result, &ct, 1);
			class_init(vector, ct, sizeof(char));

			i = move_to_next_not_blank(_Str, i) - 1;
		}
		else
		{
			class_func(vector, push_back)(&ct, &((char*)_Str.elements)[i], 1);
		}
	}
	if (ct.size)
	{
		class_func(vector, push_back)(&result, &ct, 1);
	}

	return result;

	// Undone
	class_del(vector, result);
}

typedef vector string;

// The beginning
// of the student dormitory information management system

#define Instruct_Display "display"
#define Instruct_Insert  "insert"
#define Instruct_Remove  "remove"
#define Instruct_Change  "change"
#define Instruct_Find    "find"
#define Instruct_Sort    "sort"
#define Instruct_Save    "save"
#define Instruct_Exit    "exit"

// This is template of functions for the student dormitory information management system
// _ParamList: vector<char>
// _Data:      vector<DataItem>
#define ins_func(funcName)         instruct_##funcName##_func
#define ins_funcdef(funcName)      bool ins_func(funcName)(const vector* _ParamList, vector* const _DataBase)

ins_funcdef(display);
ins_funcdef(insert);
ins_funcdef(remove);
ins_funcdef(change);
ins_funcdef(find);
ins_funcdef(sort);
ins_funcdef(save);
ins_funcdef(exit);

typedef bool(*trie_func_ptr)(void*, void*);

// Use this trie to store instruction lists and function pointers
class_def trie
{
	class trie* son[26];
	trie_func_ptr fp;
} *trie;
class_initdef(trie)
{
	*this = new(sizeof(**this));
}
void class_funcdef(trie, _del_dfs, class trie* _Ptr)
{
	for (int i = 0; i < 26; ++i)
	{
		if (_Ptr->son[i] != nullptr)
		{
			class_func(trie, _del_dfs)(this, _Ptr->son[i]);
			delete(_Ptr->son[i]);
		}
	}
	delete(_Ptr);
}
class_deldef(trie)
{
	class_func(trie, _del_dfs)(this, *this);
}
void class_funcdef(trie, insert, const char* const _Instruct, trie_func_ptr _Func)
{
	trie ct = *this;
	for (size_t i = 0; _Instruct[i] != '\0'; ++i)
	{
		char c = _Instruct[i];
		c = tolower(c) - 'a';

		if (ct->son[c])
			ct = ct->son[c];
		else
			ct->son[c] = new(sizeof(class trie)), ct = ct->son[c];
	}
	ct->fp = _Func;
	return;
}
trie_func_ptr class_funcdef(trie, find, const char* const _Str)
{
	trie ct = *this;
	for (size_t i = 0; _Str[i] != '\0'; ++i)
	{
		char c = _Str[i];
		c = tolower(c) - 'a';

		if (ct->son[c] == nullptr)
		{
			return (trie_func_ptr)nullptr;
		}
		ct = ct->son[c];
	}
	return ct->fp;
}

typedef unsigned long long int StudentId_t;
typedef unsigned int RoomId_t;

typedef struct DataItem
{
	StudentId_t studentId;
	RoomId_t    roomId;
	vector      name;
} DataItem;

// Comparison function for sorting three primary keys
int dataItem_sortCompare_studentId(const DataItem* lv, const DataItem* rv)
{
	return (lv->studentId) > (rv->studentId);
}
int dataItem_sortCompare_roomId(const DataItem* lv, const DataItem* rv)
{
	return (lv->roomId) > (rv->roomId);
}
int dataItem_sortCompare_name(const DataItem* lv, const DataItem* rv)
{
	return stricmp((char*)lv->name.elements, (char*)rv->name.elements);
}

class_def command_line
{
	// vector<char>
	vector      instruct;

	// vector<char>
	vector      parameters;

} command_line;

class_initdef(command_line)
{
	class_init(vector, this->instruct, sizeof(char));
	class_init(vector, this->parameters, sizeof(char));
}
class_deldef(command_line)
{
	class_del(vector, this->instruct);
	class_del(vector, this->parameters);
}

void welcome(void)
{
	puts("\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x0A"
		"\x7E\x20\x57\x65\x6C\x63\x6F\x6D\x65\x20\x74\x6F\x20\x75\x73\x65\x20\x4D\x72\x2E\x20\x59\x75\x20\x46\x65\x69\x78\x69\x61\x20\x61\x6E\x64\x20\x74\x68\x65\x69\x72\x20\x74\x65\x61\x6D\x27\x73\x20\x73\x74\x75\x64\x65\x6E\x74\x20\x64\x6F\x72\x6D\x69\x74\x6F\x72\x79\x20\x6D\x61\x6E\x61\x67\x65\x6D\x65\x6E\x74\x20\x73\x79\x73\x74\x65\x6D\x0A"
		"\x7C\x20\x54\x68\x69\x73\x20\x73\x79\x73\x74\x65\x6D\x20\x75\x73\x65\x73\x20\x69\x6E\x73\x74\x72\x75\x63\x74\x69\x6F\x6E\x73\x20\x77\x69\x74\x68\x20\x70\x61\x72\x61\x6D\x65\x74\x65\x72\x73\x20\x74\x6F\x20\x61\x63\x68\x69\x65\x76\x65\x20\x69\x6E\x74\x65\x72\x61\x63\x74\x69\x76\x69\x74\x79\x0A"
		"\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x0A"
		"\x2B\x20\x44\x49\x53\x50\x4C\x41\x59\x0A"
		"\x20\x20\x7C\x2D\x20\x44\x69\x73\x70\x6C\x61\x79\x20\x74\x68\x69\x73\x20\x64\x61\x74\x61\x62\x61\x73\x65\x20\x74\x6F\x20\x74\x68\x69\x73\x20\x73\x63\x72\x65\x65\x6E\x0A"
		"\x0A"
		"\x2B\x20\x49\x4E\x53\x45\x52\x54\x20\x20\x5B\x53\x74\x75\x64\x65\x6E\x74\x49\x44\x5D\x20\x20\x5B\x52\x6F\x6F\x6D\x4E\x75\x6D\x62\x65\x72\x5D\x20\x20\x5B\x4E\x61\x6D\x65\x5D\x0A"
		"\x20\x20\x7C\x2D\x20\x49\x6E\x73\x65\x72\x74\x20\x61\x20\x73\x74\x75\x64\x65\x6E\x74\x20\x69\x6E\x66\x6F\x72\x6D\x61\x74\x69\x6F\x6E\x20\x69\x74\x65\x6D\x0A"
		"\x20\x20\x7C\x2D\x20\x4E\x6F\x74\x65\x3A\x20\x53\x74\x75\x64\x65\x6E\x74\x20\x49\x44\x20\x6E\x65\x65\x64\x73\x20\x74\x6F\x20\x62\x65\x20\x75\x6E\x69\x71\x75\x65\x0A"
		"\x0A"
		"\x2B\x20\x52\x45\x4D\x4F\x56\x45\x20\x20\x5B\x53\x74\x75\x64\x65\x6E\x74\x49\x64\x5D\x0A"
		"\x20\x20\x7C\x2D\x20\x44\x65\x6C\x65\x74\x65\x20\x61\x20\x73\x74\x75\x64\x65\x6E\x74\x20\x69\x6E\x66\x6F\x72\x6D\x61\x74\x69\x6F\x6E\x20\x69\x74\x65\x6D\x0A"
		"\x0A"
		"\x2B\x20\x43\x48\x41\x4E\x47\x45\x20\x20\x5B\x53\x74\x75\x64\x65\x6E\x74\x49\x44\x5D\x20\x20\x5B\x52\x6F\x6F\x6D\x4E\x75\x6D\x62\x65\x72\x5D\x20\x20\x5B\x4E\x61\x6D\x65\x5D\x0A"
		"\x20\x20\x7C\x2D\x20\x43\x68\x61\x6E\x67\x65\x20\x6F\x74\x68\x65\x72\x20\x69\x6E\x66\x6F\x72\x6D\x61\x74\x69\x6F\x6E\x20\x66\x6F\x72\x20\x74\x68\x65\x20\x63\x75\x72\x72\x65\x6E\x74\x20\x73\x74\x75\x64\x65\x6E\x74\x20\x49\x44\x0A"
		"\x0A"
		"\x2B\x20\x46\x49\x4E\x44\x20\x20\x5B\x44\x61\x74\x61\x49\x74\x65\x6D\x5D\x0A"
		"\x20\x20\x7C\x2D\x20\x46\x69\x6E\x64\x20\x61\x20\x6C\x69\x73\x74\x20\x69\x6E\x20\x74\x68\x65\x20\x64\x61\x74\x61\x62\x61\x73\x65\x20\x77\x69\x74\x68\x20\x61\x6E\x79\x20\x69\x74\x65\x6D\x20\x65\x71\x75\x61\x6C\x20\x74\x6F\x20\x5B\x44\x61\x74\x61\x49\x74\x65\x6D\x5D\x0A"
		"\x0A"
		"\x2B\x20\x53\x4F\x52\x54\x20\x20\x5B\x2D\x53\x20\x7C\x20\x2D\x52\x20\x7C\x20\x2D\x4E\x5D\x0A"
		"\x20\x20\x7C\x2D\x20\x53\x6F\x72\x74\x20\x74\x68\x65\x20\x64\x69\x66\x66\x65\x72\x65\x6E\x74\x20\x6B\x65\x79\x73\x20\x69\x6E\x20\x74\x68\x69\x73\x20\x64\x61\x74\x61\x62\x61\x73\x65\x0A"
		"\x20\x20\x7C\x2D\x20\x54\x68\x65\x20\x61\x62\x6F\x76\x65\x20\x74\x68\x72\x65\x65\x20\x70\x61\x72\x61\x6D\x65\x74\x65\x72\x20\x6F\x70\x74\x69\x6F\x6E\x73\x20\x63\x6F\x72\x72\x65\x73\x70\x6F\x6E\x64\x20\x74\x6F\x3A\x20\x53\x74\x75\x64\x65\x6E\x74\x49\x44\x2C\x20\x52\x6F\x6F\x6D\x4E\x75\x6D\x62\x65\x72\x2C\x20\x61\x6E\x64\x20\x4E\x61\x6D\x65\x0A"
		"\x0A"
		"\x2B\x20\x53\x41\x56\x45\x20\x5B\x46\x69\x6C\x65\x4E\x61\x6D\x65\x5D\x0A"
		"\x20\x20\x7C\x2D\x20\x53\x61\x76\x65\x20\x74\x68\x65\x20\x63\x75\x72\x72\x65\x6E\x74\x20\x64\x61\x74\x61\x62\x61\x73\x65\x20\x74\x6F\x20\x74\x68\x65\x20\x73\x70\x65\x63\x69\x66\x69\x65\x64\x20\x66\x69\x6C\x65\x0A"
		"\x0A"
		"\x2B\x20\x45\x58\x49\x54\x0A"
		"\x20\x20\x7C\x2D\x20\x45\x78\x69\x74\x20\x54\x68\x65\x20\x53\x74\x75\x64\x65\x6E\x74\x20\x44\x6F\x72\x6D\x69\x74\x6F\x72\x79\x20\x4D\x61\x6E\x61\x67\x65\x6D\x65\x6E\x74\x20\x53\x79\x73\x74\x65\x6D\x0A"
		"\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x2D\x0A");
}

void init_instructTrie(trie _InstructTrie)
{
	class_func(trie, insert)(&_InstructTrie, Instruct_Display, ins_func(display));
	class_func(trie, insert)(&_InstructTrie, Instruct_Insert,  ins_func(insert));
	class_func(trie, insert)(&_InstructTrie, Instruct_Remove,  ins_func(remove));
	class_func(trie, insert)(&_InstructTrie, Instruct_Change,  ins_func(change));
	class_func(trie, insert)(&_InstructTrie, Instruct_Find,    ins_func(find));
	class_func(trie, insert)(&_InstructTrie, Instruct_Sort,    ins_func(sort));
	class_func(trie, insert)(&_InstructTrie, Instruct_Save,    ins_func(save));
	class_func(trie, insert)(&_InstructTrie, Instruct_Exit,    ins_func(exit));
}

bool read_commandline_from_screen(command_line* _CommandLine)
{
	if (_CommandLine == nullptr)
	{
		return false;
	}

	char buffer[1024];
	fgets(buffer, sizeof(buffer), stdin);

	size_t i = 0;
	char* begin = buffer, *end;
	while (*begin != '\0' && *begin != '\n' && isblank(*begin)) ++begin;
	if (*begin == '\0' || *begin == '\n') return false;

	for (; begin[i] != '\0' && begin[i] != '\n' && isblank(begin[i]) == false; ++i) continue;

	class_func(vector, push_back)(&_CommandLine->instruct, begin, i);

	if (begin[i] == '\0' || begin[i] == '\n') return true;

	for (; begin[i] != '\0' && begin[i] != '\n' && isblank(begin[i]); ++i) continue;

	if (begin[i] == '\0' || begin[i] == '\n') return true;

	end = &begin[i];
	while (*end != '\0' && *end != '\n') ++end;

	class_func(vector, push_back)(&_CommandLine->parameters, &begin[i], end - &begin[i]);
	return true;
}

int main(void)
{
	welcome();

	trie tree;
	class_init(trie, tree);
	init_instructTrie(tree);

	vector /*DataItem*/ dataBase;
	class_init(vector, dataBase, sizeof(DataItem));

	command_line commandLine;
	class_init(command_line, commandLine);

	while (true)
	{
		printf(">>> ");
		if (read_commandline_from_screen(&commandLine) == false) continue;
		class_func(vector, push_back)(&commandLine.instruct, "\0", 1);

		trie_func_ptr ptr = class_func(trie, find)(&tree, (char*)commandLine.instruct.elements);
		if (ptr == nullptr)
		{
			puts("Invaild instruct");
		}
		else
		{
			class_func(vector, push_back)(&commandLine.parameters, "\0", 1);
			if (ptr(&commandLine.parameters, &dataBase) == true)
			{
				printf("~%s: Done\n", (char*)commandLine.instruct.elements);
			}
			else
			{
				printf("~%s: Failed\n", (char*)commandLine.instruct.elements);
			}
		}
		putchar('\n');
		class_func(vector, clear)(&commandLine.instruct);
		class_func(vector, clear)(&commandLine.parameters);
	}

	class_del(command_line, commandLine);
	class_del(vector, dataBase);
	class_del(trie, tree);
	return 0;
}

ins_funcdef(display)
{
	puts("      StudentId | RoomId | Name");
	puts("------------------------------------");

	for (size_t i = 0; i < _DataBase->size; ++i)
	{
		DataItem* item = &(((DataItem*)_DataBase->elements)[i]);
		printf(" | %12llu | %6u | %s\n", item->studentId, item->roomId, (char*)(item->name.elements));
	}
	return true;
}

// Using the 'sscanf' function is unsafe
ins_funcdef(insert)
{
	DataItem item;
	class_init(vector, item.name, sizeof(char));

	char name[100];

	if (sscanf((char*)_ParamList->elements, "%llu%u%s", &item.studentId, &item.roomId, name) != 3)
	{
		error_out("Invalid or missing parameters");
		return false;
	}

	// push_back contains '\0' in end
	class_func(vector, push_back)(&item.name, name, strlen(name) + 1);

	class_func(vector, push_back)(_DataBase, &item, 1);

	return true;
}

ins_funcdef(remove)
{
	StudentId_t sid = 0;
	if (sscanf((char*)_ParamList->elements, "%llu", &sid) != 1)
	{
		error_out("Invalid or missing parameters");
		return false;
	}

	bool found = false;
	for (size_t i = 0; i < _DataBase->size; ++i)
	{
		if (((DataItem*)_DataBase->elements)[i].studentId == sid)
		{
			found = true;
			class_del(vector, ((DataItem*)_DataBase->elements)[i].name);
			class_func(vector, erase)(_DataBase, i, 1);
			break;
		}
	}

	if (found == false)
		printf("No student with student ID %llu was found in the database\n", sid);

	return true;
}

ins_funcdef(change)
{
	DataItem item;
	class_init(vector, item.name, sizeof(char));

	char name[100];

	if (sscanf((char*)_ParamList->elements, "%llu%u%s", &item.studentId, &item.roomId, name) != 3)
	{
		error_out("Invalid or missing parameters");
		return false;
	}

	bool found = false;
	for (size_t i = 0; i < _DataBase->size; ++i)
	{
		if (((DataItem*)_DataBase->elements)[i].studentId == item.studentId)
		{
			found = true;

			((DataItem*)_DataBase->elements)[i].roomId = item.roomId;

			class_func(vector, clear)(&((DataItem*)_DataBase->elements)[i].name);
			class_func(vector, push_back)(&((DataItem*)_DataBase->elements)[i].name, name, strlen(name) + 1);

			break;
		}
	}

	if (found == false)
		printf("No student with student ID %llu was found in the database\n", item.studentId);

	return true;
}

ins_funcdef(find)
{
	char buffer[1024];

	puts("      StudentId | RoomId | Name");
	puts("------------------------------------");

	for (size_t i = 0; i < _DataBase->size; ++i)
	{
		DataItem* item = &(((DataItem*)_DataBase->elements)[i]);

		sprintf(buffer, "%llu", item->studentId);

		if (stricmp(buffer, (char*)_ParamList->elements) == 0)
		{
			printf(" | %12llu | %6u | %s\n", item->studentId, item->roomId, (char*)(item->name.elements));
			continue;
		}

		sprintf(buffer, "%u", item->roomId);

		if (stricmp(buffer, (char*)_ParamList->elements) == 0)
		{
			printf(" | %12llu | %6u | %s\n", item->studentId, item->roomId, (char*)(item->name.elements));
			continue;
		}

		if (stricmp((char*)item->name.elements, (char*)_ParamList->elements) == 0)
		{
			printf(" | %12llu | %6u | %s\n", item->studentId, item->roomId, (char*)(item->name.elements));
			continue;
		}
	}

	return true;
}

ins_funcdef(sort)
{
	if (_ParamList->size != 3 || ((char*)_ParamList->elements)[0] != '-')
	{
		error_out("Invalid or missing parameters");
		return false;
	}

	switch (tolower(((char*)_ParamList->elements)[1]))
	{
		case 's':
		{
			class_func(vector, sort)(_DataBase, dataItem_sortCompare_studentId);
			break;
		}

		case 'r':
		{
			class_func(vector, sort)(_DataBase, dataItem_sortCompare_roomId);
			break;
		}

		case 'n':
		{
			class_func(vector, sort)(_DataBase, dataItem_sortCompare_name);
			break;
		}

		default:
		{
			error_out("Invalid or missing parameters");
			return false;
		}
	}

	return true;
}

ins_funcdef(save)
{
	FILE* fptr = fopen((char*)_ParamList->elements, "wb");
	if (fptr == nullptr)
	{
		error_out("File open failed");
		return false;
	}

	fputs("      StudentId | RoomId | Name\n", fptr);
	fputs("------------------------------------\n", fptr);
	for (size_t i = 0; i < _DataBase->size; ++i)
	{
		DataItem* item = &((DataItem*)_DataBase->elements)[i];
		fprintf(fptr, " | %12llu | %6u | %s\n", item->studentId, item->roomId, (char*)item->name.elements);
	}
	fclose(fptr);
	return true;
}

ins_funcdef(exit)
{
	puts("Exited");
	exit(EXIT_SUCCESS);
	return true;
}