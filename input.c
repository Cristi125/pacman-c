#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "input.h"

static struct termios original_termios;

void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &original_termios);

    struct termios raw = original_termios;
    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void disable_raw_mode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    flags &= ~O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, flags);
}

char read_input_nonblocking(void) {
    char c;

    if (read(STDIN_FILENO, &c, 1) == 1) {
        return c;
    }

    return '\0';
}
