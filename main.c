#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "BENSCHILLIBOWL.h"

// Feel free to play with these numbers! This is a great way to
// test your implementation.
#define BENSCHILLIBOWL_SIZE 100
#define NUM_CUSTOMERS 10
#define NUM_COOKS 5
#define ORDERS_PER_CUSTOMER 10
#define EXPECTED_NUM_ORDERS NUM_CUSTOMERS * ORDERS_PER_CUSTOMER

// Global variable for the restaurant.
BENSCHILLIBOWL *bcb;

/**
 * Thread funtion that represents a customer. A customer should:
 *  - allocate space (memory) for an order.
 *  - select a menu item.
 *  - populate the order with their menu item and their customer ID.
 *  - add their order to the restaurant.
 */
void* BENSCHILLIBOWLCustomer(void* tid) {
  int customer_id = (int)(long) tid;
  for (int i = 0; i < ORDERS_PER_CUSTOMER; i++){
    Order *ord = malloc(sizeof(Order));
    ord->menu_item = PickRandomMenuItem();
    ord->customer_id = customer_id;
    AddOrder(bcb, ord);
  }
  
	return NULL;
}

/**
 * Thread function that represents a cook in the restaurant. A cook should:
 *  - get an order from the restaurant.
 *  - if the order is valid, it should fulfill the order, and then
 *    free the space taken by the order.
 * The cook should take orders from the restaurants until it does not
 * receive an order.
 */
void* BENSCHILLIBOWLCook(void* tid) {
  int cook_id = (int)(long) tid;
	int orders_fulfilled = 0;
  Order *order;
  
  for (int i = 0; i < EXPECTED_NUM_ORDERS / NUM_COOKS; i++){
    order = GetOrder(bcb);
    if(order != NULL){
      orders_fulfilled++;
      free(order);
    }
  }
	printf("Cook #%d fulfilled %d orders\n", cook_id, orders_fulfilled);
	return NULL;
}

/**
 * Runs when the program begins executing. This program should:
 *  - open the restaurant
 *  - create customers and cooks
 *  - wait for all customers and cooks to be done
 *  - close the restaurant.
 */
int main() {
  srand(time(0));
  bcb = OpenRestaurant(BENSCHILLIBOWL_SIZE, EXPECTED_NUM_ORDERS);
  pthread_t cooks[NUM_COOKS];
  pthread_t customers[NUM_CUSTOMERS];
  
  for (int i = 0; i < NUM_CUSTOMERS; i++){
    pthread_create(&(customers[i]), NULL, BENSCHILLIBOWLCustomer, &i);
  }
  for (int i = 0; i < NUM_COOKS; i++){
    pthread_create(&(cooks[i]), NULL, BENSCHILLIBOWLCook, &i);
  }
  for (int i = 0; i < NUM_CUSTOMERS; i++){
    pthread_join(customers[i], NULL);
  }
   for (int i = 0; i < NUM_COOKS; i++){
    pthread_join(cooks[i], NULL);
  }
  CloseRestaurant(bcb);
    return 0;
}