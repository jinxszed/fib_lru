// Alex Chavez
// CPSC 351 5:30PM - 6:45PM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int times_called = 0; // with this global variable i will attack and dethrone god

struct lru{
  int num_fibbed, //fib(num_fibbed) = fib_result, ex fib(4) = 2
      when_called; // total counter from 0 to size-1 -> i think it's currently pointless?
  long fib_result; 
};

struct lru fill(struct lru cache[], int index);
long rfibonacci(int n);
long fib_lru(struct lru cache[], int cache_size, int num_to_fib);
void insert(struct lru cache[], int cache_size, int num_fibbed);	
void evict(struct lru cache[], int cache_size);

int main(int argc, char* argv[]) {

  if(!strcmp(argv[argc-1], "-h")) {
    printf("Run the program with three positive integers greater than 1 in the command line: first is number of tests to run, second is the max fibonacci value allowed to calculate, third is cache size.\n");
    return 0;
  }
  
  int num_tests = atoi(argv[1]), 
      max_fib_val = atoi(argv[2]),
      CACHE_SIZE = atoi(argv[3]);
  struct lru cache[CACHE_SIZE];
  int new_times;

// testing pre-filled cache

	for(int i = 0; i < CACHE_SIZE; i++)
		fill(cache, i);
// works as expected
// end testing -> fixed overflow, LRU not working 


  printf("\ntimes called value: %d\n", times_called);
  
  for(int i = 0; i < num_tests; i++) {
    int fib_to_get = rand() % max_fib_val; // chooses a number to fib within the range provided (inclusive) 
		insert(cache, CACHE_SIZE, fib_to_get); // *******something going wrong here with evict
    printf("\nfib of %d = %ld\n", fib_to_get, fib_lru(cache, CACHE_SIZE, fib_to_get)); // ******* something causing recursion
  }

  printf("\ntimes called value: %d\n", times_called);

  new_times = times_called;
  float hitrate = (float)new_times / (float)num_tests;

  printf("\ncache size: %d, tests run: %d, cache hit ratio: %.2f\n", CACHE_SIZE, num_tests, hitrate);

  return 0;
}

// recursive fibonacci function
long rfibonacci(int n) {
  if (n == 0)
    return 0; // returns 0 to add to final result
  else if (n == 1)
    return 1; // returns 1 to add to final result
  else 
    return rfibonacci(n-1) + rfibonacci(n-2);
} 

// check cache and increment the heathen if num_to_fib is found in cache
long fib_lru(struct lru cache[], int cache_size, int num_to_fib) {
  
printf(">> fib_lru function successfully called\n");  
  
  for(int i = 0; i < cache_size; i++) {
  
  insert(cache, cache_size, num_to_fib);
  	
    if(num_to_fib == cache[i].num_fibbed) {
    	printf("<- times called before increment: %d\n", times_called);
      times_called++;
      printf("-> times called after increment: %d\n", times_called);
      
      return cache[i].fib_result;
    }
	}
  
  return rfibonacci(num_to_fib);
}

// insert into cache after removing
void insert(struct lru cache[], int cache_size, int num_fibbed) {

printf("> insert function successfully called\n");

	// go through the cache until the first empty spot then exit function
	for(int i = 0; i < cache_size; i++) {
	
		// if index is "empty", simply insert new values then exit function
		if(cache[cache_size-1].num_fibbed == -1) { 
			cache[cache_size-1].num_fibbed = num_fibbed;
			cache[cache_size-1].fib_result = rfibonacci(num_fibbed);
			cache[cache_size-1].when_called = cache_size - 1;
			return;
		}
		
		// "remove" last entry in cache, i.e. values of cache[cache_size-1] = -1
		else if (cache[cache_size-1].num_fibbed != -1){
			evict(&cache[cache_size-1], cache_size-1);
			return;
		}
	}
}; 

// remove from cache once full, cache[LAST-1] = cache[LAST], ..., cache[LAST] = NULL
void evict(struct lru cache[], int cache_size) {

printf("X evict function successfully called\n");
  
  // move values over 1 index to the left
  for(int i = 0; i < cache_size-1; i++) {
    //printf("old cache[%d] num_to_fib: %d\n", i, cache[i].num_fibbed);
    cache[i] = cache[i+1];
    //printf("new cache[%d] num_to_fib: %d\n", i, cache[i].num_fibbed);
  }

  // essentially deletes last value in cache
  fill(cache, cache_size-1);	
}; 


struct lru fill(struct lru cache[], int index){
  cache[index].num_fibbed = -1;
  cache[index].fib_result = -1;
  cache[index].when_called = -1;
  
  return cache[index];
};









