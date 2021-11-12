#include <list.h>

#define YELLOW  "\x1b[33m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

static unsigned char poison = 0xF0;

#define ListCheck(list) {       \
  int error = ListCheck_(list); \
  if (error) return error;          \
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
  if (list->size > list->capacity) return LIST_SIZE_MORE_CAPACITY;
#ifdef ListSlowlyVerify
  size_t prev = list->head;
  size_t position = list->next[list->head];
  while (position != 0) {
    if (list->prev[position] != prev) {
      return LIST_INCORRECT_NEXT_OR_PREV;
    }
    prev = position;
    position = list->next[position];
  }
#endif
  return LIST_OK;
}

int ListCtor(List* list) {
  list->capacity = 8;
  list->size = 0;
  list->ordered = true;
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
  list->ordered = false;
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
  list->ordered = false;
  list->size--;
  return LIST_OK;
}

#define CheckPlace(place) {                                           \
  if (list->capacity <= place || list->prev[place] == -1) {           \
    return LIST_SEGMENTATION_ERROR;                                   \
  }                                                                   \
}

int ListPushAfter(List* list, size_t place, list_data_t value, size_t* new_elem_place) {
  if (new_elem_place) *new_elem_place = -1;
  ListCheck(list);
  CheckPlace(place);
  if (ListIncreaseCheck(list)) return LIST_NO_MEMORY;
  size_t empty_place = ListTakeEmptyPlace(list);
  if (new_elem_place) *new_elem_place = empty_place;
  list->data[empty_place] = value;
  list->next[empty_place] = list->next[place];
  list->prev[empty_place] = place;
  list->next[place] = empty_place;
  list->prev[list->next[empty_place]] = empty_place;
  list->size++;
  if (place == list->tail) {
    list->tail = empty_place;
  } else {
    list->ordered = false;
  }
  return LIST_OK;
}

int ListPushBefore(List* list, size_t place, list_data_t value, size_t* new_elem_place) {
  if (new_elem_place) *new_elem_place = -1;
  ListCheck(list);
  CheckPlace(place);
  if (ListIncreaseCheck(list)) return LIST_NO_MEMORY;
  size_t empty_place = ListTakeEmptyPlace(list);
  if (new_elem_place) *new_elem_place = empty_place;
  list->data[empty_place] = value;
  list->prev[empty_place] = list->prev[place];
  list->next[empty_place] = place;
  list->prev[place] = empty_place;
  list->next[list->prev[empty_place]] = empty_place;
  list->size++;
  if (place == list->head) {
    list->head = empty_place;
  }
  list->ordered = false;
  return LIST_OK;
}

int ListPopPlace(List* list, size_t place, list_data_t* pop_element) {
  ListDefaultPopElement(pop_element);
  ListCheck(list);
  if (list->size == 0 || list->capacity <= place || list->prev[place] == -1) {
    return LIST_OK;
  }
  if (list->prev[place] != 0) {
    list->next[list->prev[place]] = list->next[place];
  }
  if (list->next[place] != 0) {
    list->prev[list->next[place]] = list->prev[place];
  }
  if (list->tail == place) {
    list->tail = list->prev[place];
  } else {
    list->ordered = false;
  }
  if (list->head == place) {
    list->head = list->next[place];
  }
  list->prev[place] = -1;
  list->next[place] = list->empty_place;
  list->empty_place = place;
  list->size--;
  if(pop_element) *pop_element = list->data[place];
  memset(list->data + place, poison, sizeof(list_data_t));
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

size_t ListBegin(List* list) {
  if (list->head == 0) return -1;
  return list->head;
}

size_t ListEnd(List* list) {
  if (list->tail == 0) return -1;
  return list->tail;
}

int ListNext(List* list, size_t place, size_t* next) {
  ListCheck(list);
  CheckPlace(place);
  if (list->next[place] == 0) *next = -1;
  else *next = list->next[place];
  return LIST_OK;
}

int ListPrev(List* list, size_t place, size_t* prev) {
  ListCheck(list);
  CheckPlace(place);
  if (list->prev[place] == 0) *prev = -1;
  else *prev = list->prev[place];
  return LIST_OK;
}

int ListOrder(List* list) {
  ListCheck(list);
  list_data_t* ordered_array = (list_data_t*)calloc(list->size, sizeof(list_data_t));
  size_t pointer = list->head;
  size_t index = 0;
  while (pointer != 0) {
    ordered_array[index++] = list->data[pointer];
    pointer = list->next[pointer];
  }
  for (int i = 1; i <= list->size; i++) {
    list->data[i] = ordered_array[i - 1];
    list->prev[i] = i - 1;
    list->next[i] = i + 1;
  }
  list->next[list->size] = 0;
  list->head = 1;
  list->tail = list->size;
  list->empty_place = list->capacity == list->size + 1 ? 0 : list->size + 1;
  memset(list->data + list->size + 1, poison, sizeof(list_data_t) * (list->capacity - list->size - 1));
  memset(list->prev + list->size + 1, 0xFF, sizeof(list_data_t) * (list->capacity - list->size - 1));
  for (int i = list->size + 1; i < list->capacity; i++) {
    list->next[i] = i + 1;
  }
  list->next[list->capacity - 1] = 0;
  list->ordered = true;
  free(ordered_array);
  return LIST_OK;
}

int ListValueByIndex(List* list, size_t index, list_data_t* value) {
  ListCheck(list);
  if (!list->ordered) ListOrder(list);
  *value = list->data[index + 1];
  return LIST_OK;
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

int ListImage(List* list) {
  ListCheck(list);
  FILE* fout = fopen("dot.txt", "w");
  if (fout == nullptr) return LIST_DOT_FILE_CANNOT_OPEN;
  fprintf(fout, "digraph {\n");
  size_t pointer = list->head;
  fprintf(fout, "\"head\"->node%zu", list->head);
  fprintf(fout, "\"tail\"->node%zu", list->tail);
  fprintf(fout, "\"empty_place\"->node%zu", list->empty_place);
  for (size_t i = 0; i <= list->size; i++) {
    fprintf(fout, "\tnode%zu[color=\"blue\", shape=record, label = \"{<prev>prev\\n%zu|data\\n%d|address\\n%zu|<next>next\\n%zu}\"]\n",
            pointer, list->prev[pointer], list->data[pointer], pointer, list->next[pointer]);
    fprintf(fout, "\tnode%zu:<prev>->node%zu[color=\"blue\"]\n", pointer, list->prev[pointer]);
    fprintf(fout, "\tnode%zu:<next>->node%zu[color=\"blue\"]\n", pointer, list->next[pointer]);
    pointer = list->next[pointer];
  }
  pointer = list->empty_place;
  while (pointer != 0) {
    fprintf(fout, "\tnode%zu[color=\"red\", shape=record, label = \"{<prev>prev\\n%zu|data\\n%d|address\\n%zu|<next>next\\n%zu}\"]\n",
            pointer, list->prev[pointer], list->data[pointer], pointer, list->next[pointer]);
    fprintf(fout, "\tnode%zu:<next>->node%zu[color=\"red\"]\n", pointer, list->next[pointer]);
    pointer = list->next[pointer];
  }
  fprintf(fout, "}");
  fclose(fout);
  system("dot dot.txt -Tpng -o image.png");
  system("xdg-open image.png");
  return LIST_OK;
}
