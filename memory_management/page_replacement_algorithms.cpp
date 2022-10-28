// Page replacement algorithms which includes OPT and LRU.
#include <iostream>

using namespace std;

const int MAX_PAGE_SIZE = 100;
const int ALLOCATED_PAGE_SIZE = 3;

typedef struct node {
  int page_number;
  int pageflag;
} page;
page pages[ALLOCATED_PAGE_SIZE];
int page_numbers[MAX_PAGE_SIZE];
int page_size;

void init() {
  int i;
  for (i = 0; i < ALLOCATED_PAGE_SIZE; i++) {
    pages[i].page_number = 0;
    pages[i].pageflag = 0;
  }
}

int readInput() {
  char fname[20] = "page_requests.txt";
  FILE *fp;
  int i = 0;

  if ((fp = fopen(fname, "r")) == NULL) {
    cout << "出现错误，不能访问文件。";
    return 0;
  }

  while (!feof(fp)) {
    fscanf(fp, "%d,", &page_numbers[i]);
    i++;
  }
  page_size = i;
  cout << endl;
  cout << "请求的页面序列:" << endl;
  for (i = 0; i < page_size; i++) {
    cout << page_numbers[i] << "  ";
  }
  cout << endl;
  fclose(fp);
  return 1;
}

void opt() {
  int i, k, fault_position, first_page_next_index = 100, second_page_next_index = 100, third_page_next = 100;
  cout << "OPT算法:" << endl;
  // First a few pages
  for (i = 0; i < ALLOCATED_PAGE_SIZE; i++) {
    pages[i].page_number = page_numbers[i];
    cout << "x  ";
  }
  for (i = ALLOCATED_PAGE_SIZE; i < page_size; i++) {
    first_page_next_index = 100;
    second_page_next_index = 100;
    third_page_next = 100;
    if (pages[0].page_number != page_numbers[i] &&
        pages[1].page_number != page_numbers[i] &&
        pages[2].page_number != page_numbers[i]) {
      for (k = i; k < page_size; k++) {
        if (pages[0].page_number == page_numbers[k])
          first_page_next_index = k;
      }
      for (k = i; k < page_size; k++) {
        if (pages[1].page_number == page_numbers[k])
          second_page_next_index = k;
      }
      for (k = i; k < page_size; k++) {
        if (pages[2].page_number == page_numbers[k])
          third_page_next = k;
      }

      if (first_page_next_index > second_page_next_index) {
        if (first_page_next_index > third_page_next)
          fault_position = 0;
      } else {
        if (second_page_next_index > third_page_next)
          fault_position = 1;
        else
          fault_position = 2;
      }
      cout << pages[fault_position].page_number << "  ";
      pages[fault_position].page_number = page_numbers[i];
    } else {
      cout << "*  ";
    }
  }
  cout << endl;
}

void lru() {
  int i, j, k, temp, flaga;
  cout << "LRU置页算法:" << endl;
  for (i = 0; i < 3; i++) {
    pages[i].page_number = page_numbers[i];
    cout << "x  ";
  }
  for (i = 3; i < page_size; i++) {
    if (pages[0].page_number != page_numbers[i] &&
        pages[1].page_number != page_numbers[i] &&
        pages[2].page_number != page_numbers[i]) {
      cout << pages[0].page_number << "  ";
      for (j = 0; j < 2; j++)
        pages[j].page_number = pages[j + 1].page_number;
      pages[2].page_number = page_numbers[i];
    } else {
      for (j = 0; j < 3; j++)
        if (pages[j].page_number == page_numbers[i])
          flaga = j;

      temp = pages[flaga].page_number;
      for (k = flaga; k < 2; k++)
        pages[k].page_number = pages[k + 1].page_number;

      pages[2].page_number = temp;
      cout << "*  ";
    }
  }
  cout << endl << endl;
}

int main() {
  int f;
  init();
  f = readInput();
  if (f) {
    opt();
    cout << endl;
    lru();
    cout << endl;
    cout << endl;
    cout << endl;
  }
  return 0;
}
