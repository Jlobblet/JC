add_library(JC jint.c jint.h jio.c jio.h jtime.c jtime.h)
target_compile_options(JC PUBLIC -Wall -Wextra -Werror)
