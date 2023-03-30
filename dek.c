#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sched.h>

#define false 0
#define true 1
typedef int bool; // or #define bool int


pthread_t tid[2];
volatile int x = 0;
bool wantp = false;
bool wantq = false;
int turn = 1;

void * criticalSection() {
  x++;
}

void* p() {
  wantp = true;
  while(wantq) {
    if(turn == 2) {
      wantp = false;
      pthread_yield();
      while(turn != 1) {

      }
      wantp = true;
    }
  }
  criticalSection();
  turn = 2;
  wantp = false;
}

void* q() {
  wantq = true;
  while(wantp) {
    if(turn == 1) {
      wantq = false;
      pthread_yield();
      while(turn != 2) {

      }
      wantq = true;
    }
  }
  criticalSection();
  turn = 1;
  wantq = false;
}


void* addNumbers() {
  int i;
  for(i = 0; i < 50000; i++) {
    pthread_t id = pthread_self();
    if(pthread_equal(id,tid[0])){
      p();
    } else if(pthread_equal(id, tid[1])) {
      q();
    }
  }
}


int main() {
  pthread_attr_t myattr;
  cpu_set_t cpuset;

  pthread_attr_init(&myattr);
  CPU_ZERO(&cpuset);
  CPU_SET(0, &cpuset);
  pthread_attr_setaffinity_np(&myattr, sizeof(cpu_set_t), &cpuset);

  pthread_create(&(tid[0]), &myattr, &addNumbers, NULL);
  pthread_create(&(tid[1]), &myattr, &addNumbers, NULL);

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);

  printf("Final output of x should be 100,000\n");
  printf("x: %i\n", x);
}
