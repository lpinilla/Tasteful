#include "testing_suite.h"

void create_suite(char *suite_name) {
  suite = (t_test_suite *)malloc(sizeof(t_test_suite));
  suite->suite_name = suite_name;
  suite->tests = NULL;
  suite->n_of_tests = 0;
  suite->suite_state = FAILURE;
  suite->suite_name = suite_name;
}

void add_test(void (*fun)()) {
    add_named_test(fun, "");
}

void add_named_test(void (*fun)(), char const * name){
  if (suite == NULL || fun == NULL || name == NULL) return;
  if (suite->n_of_tests % ARRAY_STEP == 0){
    suite->tests = (test) realloc(
        (test) suite->tests,
            sizeof(t_test) *
            ARRAY_STEP *
            (1 + suite->n_of_tests/ ARRAY_STEP));
  }
  suite->tests[suite->n_of_tests].fun_ptr = fun;
  strncpy(suite->tests[suite->n_of_tests].name, name, MAX_FUNC_NAME);
  suite->n_of_tests++;
}

void run_suite() {
  printf("----------------------------------------------\n");
  printf("Testing Suite \" %s \" \n", suite->suite_name);
  printf("----------------------------------------------\n");
  int cpid[suite->n_of_tests], child_status[suite->n_of_tests];
  suite->suite_state = SUCCESS;
  // correr los procesos
  for (int i = 0; i < suite->n_of_tests; i++) {
    cpid[i]         = fork();
    child_status[i] = 0;
    if (cpid[i] == -1) {
      perror("Error creating child process");
      exit(EXIT_FAILURE);
    } else if (cpid[i] == 0) {  // proceso hijo, el test
      suite->tests[i].fun_ptr();
      if (errno != 0) {
        fprintf(stderr, "Error %d %s \n", errno, strerror(errno));
      }
      exit(EXIT_SUCCESS);
    }
    waitpid(cpid[i], &child_status[i], 0);
  }

  for (int i = 0; i < suite->n_of_tests; i++) {
    if (WIFEXITED(child_status[i])) {  // si terminó
      if (!WEXITSTATUS(child_status[i])) {
        printf("\033[0;32m");
        printf("%d: %s %s \n", i, "PASS", suite->tests[i].name);
        printf("\033[0m");
      } else {
        suite->suite_state = FAILURE;
        printf("\033[0;31m");
        printf("%d: %s %s \n", i, "FAIL", suite->tests[i].name);
        printf("-- return code: %d \n", WEXITSTATUS(child_status[i]));
        // print_trace();
        printf("\033[0m");
      }
    } else if (WIFSIGNALED(child_status[i])) {  // terminó por una señal
      suite->suite_state = FAILURE;
      printf("\033[0;31m");
      printf("%d: %s %s \n", i, "FAIL", suite->tests[i].name);
      printf("Killed by signal %d \n", WTERMSIG(child_status[i]));
      printf("-- return code: %d \n", WEXITSTATUS(child_status[i]));
      printf("\033[0m");
      //print_trace();
    } else if (WIFSTOPPED(child_status[i])) {
      suite->suite_state = FAILURE;
      printf("\033[0;31m");
      printf("%d: %s %s \n", i, "FAIL", suite->tests[i].name);
      printf("-- return code: %d \n", WEXITSTATUS(child_status[i]));
      printf("\033[0m");
    } else {  // el proceso no terminó
      suite->suite_state = FAILURE;
      printf("\033[0;31m");
      printf("%d: %s %s \n", i, "FAIL", suite->tests[i].name);
      printf("-- return code: %d \n", WEXITSTATUS(child_status[i]));
      printf("\033[0m");
    }
  }
  // TODO: hay que ver qué pasa si un proceso se cuelga
  int aux = suite->suite_state;
  if (suite->suite_state) {
    printf("\033[0;31m");
    printf("\t \t \t The Suite failed \n");
    printf("\033[0m");
  } else {
    printf("\033[0;32m");
    printf("\t \t \t The Suite passed \n");
    printf("\033[0m");
  }
  printf("----------------------------------------------\n");
  clear_suite();
  (aux) ? exit(EXIT_FAILURE) : exit(EXIT_SUCCESS);
}

void clear_suite() {
  free(suite->tests);
  free(suite);
}

void assert_equals(const void *a, const void *b, size_t size_of_data) {
  if ((a == NULL) && (b == NULL)) {
    exit(EXIT_SUCCESS);
  }
  if (((a == NULL) && (b != NULL)) || ((a != NULL) && (b == NULL))) {
    exit(EXIT_FAILURE);
  }
  memcmp(a, b, size_of_data) ? exit(EXIT_FAILURE) : exit(EXIT_SUCCESS);
}

void assert_not_equals(const void *a, const void *b, size_t size_of_data) {
  if ((a == NULL) && (b == NULL)) {
    exit(EXIT_FAILURE);
  }
  if (((a == NULL) && (b != NULL)) || ((a != NULL) && (b == NULL))) {
    exit(EXIT_SUCCESS);
  }
  memcmp(a, b, size_of_data) ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
}

void assert_uint_eq(uintmax_t a, uintmax_t b) {
  assert_equals(&a, &b, sizeof(uintmax_t));
}

inline void assert_true(int i) {  // podría ser una macro
  i ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
}

inline void assert_false(int i) {  // podría ser una macro
  i ? exit(EXIT_FAILURE) : exit(EXIT_SUCCESS);
}

// https://stackoverflow.com/questions/9555837/how-to-know-caller-function-when-tracing-assertion-failure
void print_trace(void) {
  void * array[20];
  size_t size, i;
  char **strings;
  size    = backtrace(array, 20);
  strings = backtrace_symbols(array, size);
  printf("Obtained %zd stack frames.\n", size);
  for (i = 0; i < size; i++) {
    printf("\t %s \n", strings[i]);
  }
  free(strings);
}

void check_child_status(int status) {
  if (WIFEXITED(status)) {
    printf("exited, status=%d\n", WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    printf("killed by signal %d", WTERMSIG(status));
    switch (WTERMSIG(status)) {
      case SIGSEGV:
        printf("\t ++Segmentation fault \n");
        break;
      case SIGABRT:
        printf("\t ++Abort \n");
        break;
    }
  } else if (WIFSTOPPED(status)) {
    printf("stopped by signal %d\n", WSTOPSIG(status));
  } /* else if (WIFCONTINUED(status)) {
       printf("continued\n");
   }*/
}
