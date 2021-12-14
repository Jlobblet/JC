#include <theft.h>
#include <check.h>
#include "jbits.h"

bool test_BitArray2d(theft_propfun1 prop1, const char* name);

bool test_BitArray2d_coords(theft_propfun1 prop1, const char* name);

enum theft_alloc_res theft_BitArray2d_alloc(struct theft* t, void* env, void** instance);

void theft_BitArray2d_free(void* instance, void* env);

void theft_BitArray2d_print(FILE *f, const void *instance, void *env);

static struct theft_type_info BitArray2d_info = {
        .alloc = theft_BitArray2d_alloc,
        .free = theft_BitArray2d_free,
        .print = theft_BitArray2d_print,
        .autoshrink_config = {
                .enable = true,
        },
};

struct BitArray2d_With_Coords {
    uptr row;
    uptr col;
    BitArray2d* arr;
};

enum theft_alloc_res theft_Coords_alloc(struct theft* t, void* env, void** instance);

void theft_Coords_free(void* instance, void* env);

void theft_Coords_print(FILE *f, const void *instance, void *env);

static struct theft_type_info Coords_info = {
        .alloc = theft_Coords_alloc,
        .free = theft_Coords_free,
        .print = theft_Coords_print,
        .autoshrink_config = {
                .enable = true,
        }
};

static enum theft_trial_res prop_init_should_be_all_zeroes(struct theft* t, void* arg1) {
    BitArray2d* arr = (BitArray2d*) arg1;
    uptr count = BitArray2d_count_on(arr);
    return (count == 0) ? THEFT_TRIAL_PASS : THEFT_TRIAL_FAIL;
}

static enum theft_trial_res prop_init_flip_should_be_all_ones(struct theft* t, void* arg1) {
    BitArray2d* arr = (BitArray2d*) arg1;
    BitArray2d_flip(arr);
    uptr count = BitArray2d_count_on(arr);
    uptr expected = arr->cols * arr->rows;
    return (count == expected) ? THEFT_TRIAL_PASS : THEFT_TRIAL_FAIL;
}

static enum theft_trial_res prop_get_set_should_be_same(struct theft* t, void* arg1) {
    struct BitArray2d_With_Coords* data = (struct BitArray2d_With_Coords*) arg1;
    BitArray2d_set(data->arr, data->row, data->col, 1);
    BitArray2dBacker bit = BitArray2d_get(data->arr, data->row, data->col);
    return (bit == 1) ? THEFT_TRIAL_PASS : THEFT_TRIAL_FAIL;
}

START_TEST(test_BitArray2d_init_all_zeroes)
    {
        ck_assert(test_BitArray2d(prop_init_should_be_all_zeroes, "Initialised BitArray2d should be all 0s"));

    }
END_TEST

START_TEST(test_BitArray2d_init_flip_all_ones)
    {
        ck_assert(test_BitArray2d(prop_init_flip_should_be_all_ones, "Initialised then flipped BitArray2d should be all 1s"));

    }
END_TEST

START_TEST(test_BitArray2d_get_set_same)
    {
        ck_assert(test_BitArray2d_coords(prop_get_set_should_be_same, "Getting a bit set to 1 should be 1"));
    }
END_TEST

Suite* BitArray2d_suite() {
    Suite* s;
    TCase* tc_core;

    s = suite_create("BitArray2d Properties");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_BitArray2d_init_all_zeroes);
    tcase_add_test(tc_core, test_BitArray2d_init_flip_all_ones);
    tcase_add_test(tc_core, test_BitArray2d_get_set_same);
    suite_add_tcase(s, tc_core);

    return s;
}

int main() {
//    BitArray2d arr = {
//            .rows = 1,
//            .cols = 10,
//    };
//    BitArray2d_init(&arr);
//    BitArray2d_flip(&arr);
//    theft_BitArray2d_print(stdout, &arr, NULL);
//    printf("Count   : %lu\n", BitArray2d_count_on(&arr));
//    printf("Expected: %lu\n", arr.cols * arr.rows);
//    BitArray2d_dest(&arr);
//    return 0;
    Suite* s = BitArray2d_suite();
    SRunner* runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    int number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

enum theft_alloc_res theft_BitArray2d_alloc(struct theft* t, void* env, void** instance) {
    uptr cols = (uptr) theft_random_bits(t, sizeof(uptr));
    uptr rows = (uptr) theft_random_bits(t, sizeof(uptr));
    if (cols == 0) { cols++; }
    if (rows == 0) { rows++; }

    *instance = calloc(1, sizeof(BitArray2d));
    if (*instance == NULL) {
        return THEFT_ALLOC_ERROR;
    }
    BitArray2d* arr = *(BitArray2d**) instance;
    arr->cols = cols;
    arr->rows = rows;
    if (BitArray2d_init(arr) == -1) {
        return THEFT_ALLOC_ERROR;
    }

    return THEFT_ALLOC_OK;
}

void theft_BitArray2d_free(void* instance, void* env) {
    BitArray2d_dest(instance);
    free(instance);
}

void theft_BitArray2d_print(FILE *f, const void *instance, void *env) {
    BitArray2d* arr = (BitArray2d*) instance;
    fprintf(f, "BitArray2d [%lu, %lu]:\n", arr->rows, arr->cols);
    for (uptr i = 0; i < arr->rows; i++) {
        for (uptr j = 0; j < arr->cols; j++) {
            fputc(BitArray2d_get(arr, i, j) ? '1' : '0', f);
        }
        fputc('\n', f);
    }
}

enum theft_alloc_res theft_Coords_alloc(struct theft* t, void* env, void** instance) {
    *instance = calloc(1, sizeof(struct BitArray2d_With_Coords));
    if (*instance == NULL) {
        return THEFT_ALLOC_ERROR;
    }
    struct BitArray2d_With_Coords* c = *(struct BitArray2d_With_Coords**)instance;
    enum theft_alloc_res res = theft_BitArray2d_alloc(t, env, (void**) &c->arr);
    if (res != THEFT_ALLOC_OK) { return res; }

    uptr col = (uptr) theft_random_choice(t, c->arr->cols - 1);
    uptr row = (uptr) theft_random_choice(t, c->arr->rows - 1);

    c->col = col;
    c->row = row;
    return THEFT_ALLOC_OK;
}

void theft_Coords_free(void* instance, void* env) {
    struct BitArray2d_With_Coords* coords = (struct BitArray2d_With_Coords*) instance;
    theft_BitArray2d_free(coords->arr, env);
    free(instance);
}

void theft_Coords_print(FILE *f, const void *instance, void *env) {
    struct BitArray2d_With_Coords* data = (struct BitArray2d_With_Coords*) instance;
    fprintf(f, "BitArray2d with coords [%lu, %lu]:\n", data->arr->rows, data->arr->cols);
    for (uptr j = 0; j < data->arr->cols; j++) {
        fputc(j == data->col ? '|' : ' ', f);
    }
    for (uptr i = 0; i < data->arr->rows; i++) {
        fputc(i == data->row ? '-' : ' ', f);
        for (uptr j = 0; j < data->arr->cols; j++) {
            fputc(BitArray2d_get(data->arr, i, j) ? '1' : '0', f);
        }
        fputc('\n', f);
    }
}

bool test_BitArray2d(theft_propfun1 prop1, const char* name) {
    theft_seed seed = theft_seed_of_time();

    struct theft_run_config config = {
            .name = name,
            .prop1 = prop1,
            .type_info = {&BitArray2d_info},
            .seed = seed,
    };

    enum theft_run_res res = theft_run(&config);
    return res == THEFT_RUN_PASS;
}

bool test_BitArray2d_coords(theft_propfun1 prop1, const char* name) {
    theft_seed seed = theft_seed_of_time();

    struct theft_run_config config = {
            .name = name,
            .prop1 = prop1,
            .type_info = {&Coords_info},
            .seed = seed,
    };

    enum theft_run_res res = theft_run(&config);
    return res == THEFT_RUN_PASS;
}
