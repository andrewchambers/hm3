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

hm3_value
hm3_number_add(struct hm3_vm *vm, hm3_value l, hm3_value r)
{
    int lsmall, rsmall;

    lsmall = l.number.small.is_small;
    rsmall = r.number.small.is_small;

    if (lsmall && rsmall) {
        int overflow;
        int64_t res;

        overflow = __builtin_add_overflow(
            l.number.small.value, l.number.small.value, &res);
        if (!overflow && fits_small_number(res)) {
            return wrap_small_number(res);
        } else if (!overflow) {
            abort();
        } else {
            abort();
        }
    } else if (lsmall) {
        abort();
    } else if (rsmall) {
        abort();
    }

    abort();
}