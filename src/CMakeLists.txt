add_library(JC
    jarray.c jarray.h
    jbits.c jbits.h
    jint.c jint.h
    jio.c jio.h
    jiterator.c jiterator.h
    jiterator_short.c jiterator_short.h
    jlog.c jlog.h
    jmmap.c jmmap.h
    jprint.c jprint.h
    jstring.c jstring.h
    jtime.c jtime.h
    jvector.c jvector.h
)
target_compile_options(JC PUBLIC -Wall -Wextra -Werror)
