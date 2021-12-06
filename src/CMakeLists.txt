add_library(JC
    jarray.c jarray.h
    jbits.c jbits.h
    jint.c jint.h
    jio.c jio.h
    jmmap.c jmmap.h
    jstring.c jstring.h
    jtime.c jtime.h
)
target_compile_options(JC PUBLIC -Wall -Wextra -Werror)
