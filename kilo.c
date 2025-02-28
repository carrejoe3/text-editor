#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

// Original terminal attributes
struct termios orig_termios;

void disableRawMode() {
  // Restore original terminal attributes
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
  // Save original terminal attributes
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);

  // Set raw mode
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}
