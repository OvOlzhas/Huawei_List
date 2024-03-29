#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <listconfig.h>

struct List {
  list_data_t* data;
  size_t* next;
  size_t* prev;
  size_t head;
  size_t tail;
  size_t empty_place;
  size_t size;
  size_t capacity;
  bool sorted;
};

enum ListError {
  LIST_OK = 0,
  LIST_NO_MEMORY = 1,
  NULL_LIST = 2,
  LIST_EMPTY = 3,
  LIST_SEGMENTATION_ERROR = 4,
  LIST_SIZE_MORE_CAPACITY = 5,
  LIST_INCORRECT_NEXT_OR_PREV = 6,
  LIST_DOT_FILE_CANNOT_OPEN = 7
};

#define ListDump(list) {                                              \
  ListDump_(&list, #list, __PRETTY_FUNCTION__, __FILE__, __LINE__);   \
}

int ListCheck_(List* list);

int ListCtor(List* list);

int ListDtor(List* list);

int ListPushBack(List* list, int value, size_t* new_elem_place = nullptr);

int ListPopBack(List* list, list_data_t* pop_element = nullptr);

int ListPushFront(List* list, list_data_t value, size_t* new_elem_place = nullptr);

int ListPopFront(List* list, list_data_t* pop_element = nullptr);

int ListPushAfter(List* list, size_t place, list_data_t value, size_t* new_elem_place = nullptr);

int ListPushBefore(List* list, size_t place, list_data_t value, size_t* new_elem_place = nullptr);

int ListPopPlace(List* list, size_t place, list_data_t* pop_element = nullptr);

size_t ListFind(List* list, list_data_t value);

size_t ListBegin(List* list);

size_t ListEnd(List* list);

int ListNext(List* list, size_t place, size_t* next);

int ListPrev(List* list, size_t place, size_t* prev);

int ListOrder(List* list);

int ListValueByIndex(List* list, size_t index, list_data_t* value);

void ListDump_(List* list, const char* name, const char* function, const char* file, int line);

#define ListImage(list) {                         \
  char name[strlen(#list) + 20];                  \
  sprintf(name, "%s_line_%d", #list, __LINE__);   \
}

int ListImage_(List* list, const char* file_name);
