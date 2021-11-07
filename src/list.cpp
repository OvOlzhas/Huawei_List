#include <list.h>

#define YELLOW  "\x1b[33m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

static unsigned char poison = 0xF0;

#define ListCheck(list) {       \
  int error = ListCheck_(list); \
  if (error) return error;      \
}

#define ListPopCheck(list, pop_elem) {  \
  int error = ListCheck_(list);         \
  if (error) return error;              \
}

#define ListDefaultPopElement(pop_element) {            \
  list_data_t poison_elem;                              \
  memset(&poison_elem, poison, sizeof(list_data_t));    \
  if (pop_element) *pop_element = poison_elem;          \
}

int ListCheck_(List* list) {
  if (list == nullptr || list->capacity == 0 || list->data == nullptr
      || list->next == nullptr || list->prev == nullptr) {
    return NULL_LIST;
  }
  return LIST_OK;
}

static void StackPrint(size_t value) { printf("%zu", value); }

static size_t StackPoison() { return 0xF0F0F0F0F0F0F0F0; }

int ListCtor(List* list) {
  list->capacity = 8;
  list->size = 0;
  list->data = (list_data_t*)calloc(list->capacity, sizeof(list_data_t));
  list->next = (size_t*)calloc(list->capacity, sizeof(size_t));
  list->prev = (size_t*)calloc(list->capacity, sizeof(size_t));
  memset(list->data + 1, poison, (list->capacity - 1) * sizeof(list_data_t));
  memset(list->prev + 1, 0xFF, (list->capacity - 1) * sizeof(size_t));
  if (list->data == nullptr || list->next == nullptr || list->prev == nullptr) {
    return LIST_NO_MEMORY;
  }
  list->head = list->tail = 0;
  for (size_t i = 1; i < list->capacity - 1; i++) {
    list->next[i] = i + 1;
  }
  list->next[list->capacity - 1] = 0;
  list->empty_place = 1;
  return LIST_OK;
}

int ListDtor(List* list) {
  ListCheck(list);
  free(list->data);
  free(list->next);
  free(list->prev);
  list->capacity = 0;
  list->size = 0;
  return LIST_OK;
}

size_t ListTakeEmptyPlace(List* list) {
  if (ListCheck_(list)) return 0;
  size_t place = list->empty_place;
  list->empty_place = list->next[list->empty_place];
  return place;
}

int ListRealloc(List* list) {
  list->data = (list_data_t*)realloc(list->data, list->capacity * sizeof(list_data_t));
  list->next = (size_t*)realloc(list->next, list->capacity * sizeof(size_t));
  list->prev = (size_t*)realloc(list->prev, list->capacity * sizeof(size_t));
  if (list->data == nullptr || list->next == nullptr || list->prev == nullptr) {
    return LIST_NO_MEMORY;
  }
  return LIST_OK;
}

int ListIncreaseCheck(List* list) {
  if (list->size + 1 == list->capacity) {
    list->capacity <<= 1;
    if (ListRealloc(list)) {
      return LIST_NO_MEMORY;
    }
    memset(list->data + (list->capacity >> 1), poison, (list->capacity >> 1) * sizeof(list_data_t));
    memset(list->prev + (list->capacity >> 1), 0xFF, (list->capacity >> 1) * sizeof(size_t));
    for (size_t i = list->capacity >> 1; i < list->capacity; i++) {
      list->next[i] = i + 1;
    }
    list->next[list->capacity - 1] = 0;
    list->empty_place = list->capacity >> 1;
  }
  return LIST_OK;
}

int ListPushBack(List* list, list_data_t value, size_t* new_elem_place) {
  if (new_elem_place) *new_elem_place = -1;
  ListCheck(list);
  if (ListIncreaseCheck(list)) return LIST_NO_MEMORY;
  size_t empty_place = ListTakeEmptyPlace(list);
  if (new_elem_place) *new_elem_place = empty_place;
  list->data[empty_place] = value;
  list->prev[empty_place] = list->tail;
  list->next[empty_place] = 0;
  if (list->tail != 0) {
    list->next[list->tail] = empty_place;
  } else {
    list->head = empty_place;
  }
  list->tail = empty_place;
  list->size++;
  return LIST_OK;
}

int ListPopBack(List* list, list_data_t* pop_element) {
  ListDefaultPopElement(pop_element);
  ListCheck(list);
  if (list->size == 0) return LIST_EMPTY;
  if (pop_element) {
    *pop_element = list->data[list->tail];
  }
  memset(list->data + list->tail, poison, sizeof(list_data_t));
  list->next[list->tail] = list->empty_place;
  list->empty_place = list->tail;
  list->tail = list->prev[list->tail];
  if (list->tail != 0) {
    list->prev[list->next[list->tail]] = -1;
    list->next[list->tail] = 0;
  } else {
    list->head = 0;
  }
  list->size--;
  return LIST_OK;
}

int ListPushFront(List* list, list_data_t value, size_t* new_elem_place) {
  if (new_elem_place) *new_elem_place = -1;
  ListCheck(list);
  if (ListIncreaseCheck(list)) return LIST_NO_MEMORY;
  size_t empty_place = ListTakeEmptyPlace(list);
  if (new_elem_place) *new_elem_place = empty_place;
  list->data[empty_place] = value;
  list->next[empty_place] = list->head;
  list->prev[empty_place] = 0;
  if (list->head != 0) {
    list->prev[list->head] = empty_place;
  } else {
    list->tail = empty_place;
  }
  list->head = empty_place;
  list->size++;
  return LIST_OK;
}

int ListPopFront(List* list, list_data_t* pop_element) {
  ListDefaultPopElement(pop_element);
  ListCheck(list);
  if (list->size == 0) return LIST_EMPTY;
  if (pop_element) *pop_element = list->data[list->tail];
  memset(list->data + list->head, poison, sizeof(list_data_t));
  list->prev[list->head] = -1;
  size_t empty_place = list->head;
  list->head = list->next[list->head];
  if (list->head != 0) {
    list->next[list->prev[list->head]] = list->empty_place;
    list->prev[list->head] = 0;
  } else {
    list->tail = 0;
  }
  list->empty_place = empty_place;
  list->size--;
  return LIST_OK;
}

int ListPushAfter(List* list, size_t place, list_data_t value, size_t* new_elem_place) {
  if (new_elem_place) *new_elem_place = -1;
  ListCheck(list);
  list_data_t poison_elem;
  memset(&poison_elem, poison, sizeof(list_data_t));
  if (list->capacity <= place || list->data[place] == poison_elem) {
    return LIST_SEGMENTATION_ERROR;
  }
  if (ListIncreaseCheck(list)) return LIST_NO_MEMORY;
  size_t empty_place = ListTakeEmptyPlace(list);
  if (new_elem_place) *new_elem_place = empty_place;
  list->data[empty_place] = value;
  list->next[empty_place] = list->next[place];
  list->prev[empty_place] = place;
  list->next[place] = empty_place;
  list->prev[list->next[empty_place]] = empty_place;
  if (place == list->tail) {
    list->tail = empty_place;
  }
  return LIST_OK;
}

int ListPushBefore(List* list, size_t place, list_data_t value, size_t* new_elem_place) {
  if (new_elem_place) *new_elem_place = -1;
  ListCheck(list);
  list_data_t poison_elem;
  memset(&poison_elem, poison, sizeof(list_data_t));
  if (list->capacity <= place || list->data[place] == poison_elem) {
    return LIST_SEGMENTATION_ERROR;
  }
  if (ListIncreaseCheck(list)) return LIST_NO_MEMORY;
  size_t empty_place = ListTakeEmptyPlace(list);
  if (new_elem_place) *new_elem_place = empty_place;
  list->data[empty_place] = value;
  list->prev[empty_place] = list->prev[place];
  list->next[empty_place] = place;
  list->prev[place] = empty_place;
  list->next[list->prev[empty_place]] = empty_place;
  if (place == list->head) {
    list->head = empty_place;
  }
  return LIST_OK;
}

int ListPopPlace(List* list, size_t place, list_data_t* pop_element) {
  ListDefaultPopElement(pop_element);
  ListCheck(list);
  list_data_t poison_elem;
  memset(&poison_elem, poison, sizeof(list_data_t));
  if (list->size == 0 || list->capacity <= place || list->data[place] == poison_elem) {
    return LIST_OK;
  }
  if (list->prev[place] != 0) {
    list->next[list->prev[place]] = list->next[place];
  }
  if (list->prev[place] != 0) {
    list->prev[list->next[place]] = list->prev[place];
  }
  if (list->tail == place) {
    list->tail = list->prev[place];
  }
  if (list->head == place) {
    list->head = list->next[place];
  }
  list->prev[place] = -1;
  list->next[place] = list->empty_place;
  list->empty_place = place;
  if(pop_element) *pop_element = list->data[place];
  list->data[place] = poison_elem;
  return LIST_OK;
}

size_t ListFind(List* list, list_data_t value) {
  if (ListCheck_(list)) return -1;
  size_t index = list->head;
  while (index != 0) {
    if (list->data[index] == value) {
      return index;
    }
    index = list->next[index];
  }
  return -1;
}

void ListDump_(List* list, const char* name, const char* function, const char* file, int line) {
  printf("\n");
  if (ListCheck_(list)) {
    printf(RED "Null list!\n" RESET);
    return;
  }
  printf("List '" YELLOW "%s" RESET "' at " YELLOW "%s" RESET " at " BLUE "%s(%d)" RESET ":\n{\n" RESET, name, function, file, line);
  printf("\t" YELLOW "size" RESET " = %zu\t" YELLOW "capacity" RESET " = %zu\n\t{\n", list->size, list->capacity);
  printf(GREEN "\t\t\tDATA\t\tNEXT\t\tPREV\n" RESET);
  list_data_t poison_elem;
  memset(&poison_elem, poison, sizeof(list_data_t));
  for (int i = 0; i < list->capacity; i++) {
    printf(BLUE"\t%d" RESET ":\t\t", i);
    list_data_t data = list->data[i];
    size_t next = list->next[i];
    size_t prev = list->prev[i];
    if (data == poison_elem) {
      printf(RED "POISON\t\t" RESET);
    } else {
      printf("%d\t\t\t", data);
    }

    if (next == -1) {
      printf(RED "-1\t\t\t" RESET);
    } else {
      printf("%zu\t\t\t", next);
    }

    if (prev == -1) {
      printf(RED "-1" RESET);
    } else {
      printf("%zu", prev);
    }

    if (list->head == i) {
      printf("\t<-head");
    }
    if (list->tail == i) {
      printf("\t<-tail");
    }
    if (list->empty_place == i) {
      printf("\t<-empty");
    }
    printf("\n");
  }
  printf("\t}\n}\n");
  printf("\n");
}
