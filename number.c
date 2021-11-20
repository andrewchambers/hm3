/*
 * This file implements operations on the hm3 arbitrary precision number type.
 *
 * Numbers are represented by either a 63 bit signed integer, or on overflow
 * they become heap allocated arbitrary precision integers.
 */
#include "hm3.h"

static int
fits_small_number(int64_t v)
{
    return v >= HM3_SMALL_NUMBER_MIN && v <= HM3_SMALL_NUMBER_MAX;
}

static hm3_value
wrap_small_number(int64_t v)
{
    return (hm3_value){
            .number = (union hm3_number){
                .small = (struct hm3_small_number){
                    .value = v,
                    .is_small = 1
            },
        },
    };
}

static hm3_value
wrap_big_number(struct hm3_big_number *v)
{
    return (hm3_value){
        .number = (union hm3_number){
            .big = v,
        },
    };
}

static struct hm3_big_number *
promote_number(struct hm3_vm *vm, int64_t v)
{
    struct hm3_big_number *big_v;

    big_v = hm3_alloc_gcobject(vm, sizeof(struct hm3_big_number));
    if (mp_init(&big_v->value) != MP_OKAY)
        hm3_out_of_memory(vm);

    mp_set_i64(&big_v->value, v);

    return big_v;
}

hm3_value
hm3_number_add(struct hm3_vm *vm, hm3_value l, hm3_value r)
{
    int overflow, decref_l, decref_r;
    int l_is_small, r_is_small;
    struct hm3_big_number *big_result, *big_l, *big_r;
    int64_t small_result;

    l_is_small = l.number.small.is_small;
    r_is_small = r.number.small.is_small;

    if (l_is_small && r_is_small) {
        overflow = __builtin_add_overflow(
            l.number.small.value, l.number.small.value, &small_result);
        if (!overflow && fits_small_number(small_result)) {
            return wrap_small_number(small_result);
        } else if (!overflow) {
            return wrap_big_number(promote_number(vm, small_result));
        } else {
            big_l = promote_number(vm, l.number.small.value);
            big_r = promote_number(vm, r.number.small.value);
            decref_l = 1;
            decref_r = 1;
        }
    } else if (l_is_small) {
        big_l = promote_number(vm, l.number.small.value);
        big_r = r.number.big;
        decref_l = 1;
        decref_r = 0;
    } else if (r_is_small) {
        big_l = l.number.big;
        big_r = promote_number(vm, r.number.small.value);
        decref_l = 0;
        decref_r = 1;
    } else {
        big_l = l.number.big;
        big_r = r.number.big;
        decref_l = 0;
        decref_r = 0;
    }

    big_result = hm3_alloc_gcobject(vm, sizeof(struct hm3_big_number));

    if (mp_init(&big_result->value) != MP_OKAY)
        hm3_out_of_memory(vm);

    if (mp_add(&big_result->value, &l.number.big->value, &r.number.big->value)
        != MP_OKAY)
        hm3_out_of_memory(vm);

    if (decref_l)
        hm3_decref(wrap_big_number(big_l));
    if (decref_r)
        hm3_decref(wrap_big_number(big_r));

    return wrap_big_number(big_result);
}