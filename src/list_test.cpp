#include <list_test.h>

#define ListVerify(FUNC) {          \
  int error = FUNC;                 \
  if (error) printf("ERROR!!!\n");  \
};

void EasyTest() {
  printf("------------------------------------------EASY_TEST--------------------------------------------\n");
  List list = {};
  ListVerify(ListCtor(&list));
  ListVerify(ListPushBack(&list, 10));
  ListVerify(ListPushBack(&list, 20));
  ListVerify(ListPushBack(&list, 30));
  ListVerify(ListPushBack(&list, 40));
  ListVerify(ListPushBack(&list, 50));
  ListVerify(ListPushBack(&list, 60));
  ListVerify(ListPushBack(&list, 70));
  ListVerify(ListPushBack(&list, 80));
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
  ListDump(list);
  ListVerify(ListPushBefore(&list, place10, 100));
  ListDump(list);
  ListVerify(ListPopPlace(&list, place30));

  ListDump(list);
  ListVerify(ListOrder(&list));
  ListDump(list);

  ListVerify(ListDtor(&list));
}

