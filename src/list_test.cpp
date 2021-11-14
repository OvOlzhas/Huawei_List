#include <list_test.h>

#define ListVerify(FUNC) {          \
  int error = FUNC;                 \
  if (error) printf("\x1b[31m-----------------ERROR----------------\x1b[0m\n");  \
};

void EasyTest() {
  printf("------------------------------------------EASY_TEST--------------------------------------------\n");
  List list = {};
  ListVerify(ListCtor(&list));
  ListVerify(ListPushBack(&list, 10));
  ListVerify(ListPushBack(&list, 20));
  ListVerify(ListPushBack(&list, 30));
  ListVerify(ListPushBack(&list, 40));
  ListVerify(ListPushFront(&list, 50));
  ListVerify(ListPushFront(&list, 60));
  ListVerify(ListPushFront(&list, 70));
  ListVerify(ListPushFront(&list, 80));
  ListDump(list);
  ListVerify(ListDtor(&list));
}

void HardTest() {
  printf("------------------------------------------HARD_TEST--------------------------------------------\n");
  List list = {};
  ListVerify(ListCtor(&list));
  size_t place10, place50, place30;
  ListVerify(ListPushBack(&list, 10, &place10));
  ListVerify(ListPushBack(&list, 20));
  ListVerify(ListPushBack(&list, 30, &place30));
  ListVerify(ListPushBack(&list, 40));
  ListVerify(ListPushFront(&list, 50, &place50));
  ListVerify(ListPushFront(&list, 60));
  ListVerify(ListPushFront(&list, 70));
  ListVerify(ListPushFront(&list, 80));
  ListVerify(ListPopFront(&list));
  ListVerify(ListPopBack(&list));
  ListVerify(ListPopFront(&list));
  ListVerify(ListPushAfter(&list, place50, 90));
  ListVerify(ListPushBefore(&list, place10, 100));
  ListVerify(ListPopPlace(&list, place30));
  ListImage(list);
  for (int i = 0; i < list.size; i++) {
    int value;
    ListValueByIndex(&list, i, &value);
    printf("%d: %d\n", i, value);
  }
  ListDump(list);
  ListVerify(ListDtor(&list));
}

void DecreaseTest() {
  printf("----------------------------------------DECREASE_TEST------------------------------------------\n");
  List list = {};
  ListVerify(ListCtor(&list));
  ListVerify(ListPushBack(&list, 10));
  ListVerify(ListPushBack(&list, 20));
  ListVerify(ListPushBack(&list, 30));
  ListVerify(ListPushBack(&list, 40));
  ListVerify(ListPushFront(&list, 50));
  ListVerify(ListPushFront(&list, 60));
  ListVerify(ListPushFront(&list, 70));
  ListVerify(ListPushFront(&list, 80));
  ListVerify(ListPopBack(&list));
  ListVerify(ListPopBack(&list));
  ListVerify(ListPopBack(&list));
  ListVerify(ListPopBack(&list));
  ListVerify(ListPopBack(&list));
  ListDump(list);
  ListImage(list);
  ListVerify(ListDtor(&list));
}

