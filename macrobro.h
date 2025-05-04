#ifdef _MB_DYNAMIC_ARRAY_
// struct dynamic_array {
//     TYPE *items;
//     size_t count;
//     size_t capacity;
//     ...
// };
#define mb_da_append(da, elem)                                          \
    do {                                                                \
        if ((da).capacity <= (da).count) {                              \
            (da).capacity += 10;                                        \
            (da).items = realloc((da).items,                            \
                    sizeof(elem)*(da).capacity);                        \
            assert((da).items != NULL);                                 \
        }                                                               \
        (da).items[(da).count++] = (elem);                              \
    } while (0)

#define mb_da_remove(da, idx, clear_func)                               \
    do {                                                                \
        if ((da).count > 0) {                                           \
            if (clear_func != NULL) clear_func((da).items+idx);         \
            memcpy((da).items+(idx), (da).items+(idx)+1,                \
                    sizeof(*(da).items)*((da).capacity-(idx)-1));       \
            (da).count--;                                               \
        }                                                               \
    } while (0)
#endif // _MB_DYNAMIC_ARRAY_

#ifdef _MB_TEST_
#define RESET   "\x1b[0m"
#define BLACK   "\x1b[30m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define PURPLE  "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"
#define mb_pass(case_id)                                            \
    do {                                                            \
        printf("case %d: [" GREEN "pass" RESET "]\n", (case_id));   \
        fflush(stdout);                                             \
    } while (0)

#define mb_fail(case_id)                                            \
    do {                                                            \
        printf("case %d: [" RED "fail" RESET "]\n", (case_id));     \
        fflush(stdout);                                             \
    } while (0)

#define mb_test(exp, case_id)                                       \
    do {                                                            \
        if ((exp)) mb_pass((case_id));                              \
        else       mb_fail((case_id));                              \
    } while (0)
#endif // _MB_TEST_
