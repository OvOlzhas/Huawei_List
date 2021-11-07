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
};

enum ListError {
  LIST_OK = 0,
  LIST_NO_MEMORY = 1,
  NULL_LIST = 2,
  LIST_EMPTY = 3,
  LIST_SEGMENTATION_ERROR = 4,
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

void ListDump_(List* list, const char* name, const char* function, const char* file, int line);
