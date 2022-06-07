#ifndef JC_JITERATOR_SHORT_H
#define JC_JITERATOR_SHORT_H

#include "jiterator.h"

#define collect jiterator_collect

#define vector_reset jiterator_vector_reset
#define vector jiterator_vector_init

#define map jiterator_map_init
#define action jiterator_action_init
#define filter jiterator_filter_init

#define reduce jiterator_reduce
#define fold jiterator_fold
#define scan jiterator_scan_init

#define min_max jiterator_min_max
#define min jiterator_min
#define max jiterator_max
#define all jiterator_all
#define any jiterator_any

#define iota jiterator_iota_init
#define take jiterator_take_init
#define drop jiterator_drop_init
#define take_while jiterator_take_while_init
#define drop_while jiterator_drop_while_init
#define zip jiterator_zip_init
#define enumerate jiterator_enumerate_init
#define pairwise jiterator_pairwise_init

#define once jiterator_once_init
#define once_with jiterator_once_with_init
#define repeat jiterator_repeat_init
#define repeat_with jiterator_repeat_with_init

#define concat jiterator_concat_init

#endif //JC_JITERATOR_SHORT_H
