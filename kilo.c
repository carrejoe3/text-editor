#include <stdio.h>
#include <ctype.h>
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

  // Disable IXON (software flow control)
  // Disable ICRNL (carriage return -> newline)
  raw.c_iflag &= ~(IXON | ICRNL);
  // Disable OPOST (output processing)
  raw.c_oflag &= ~(OPOST);
  // Disable ECHO (print input characters)
  // Disable ICANON (canonical mode)
  // Disable ISIG (signals)
  // Disable IEXTEN (extended input processing)
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

  // Apply changes
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();

  char c;
  // Read one byte at a time
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    if (iscntrl(c)) {
      // Print control character
      printf("%d\r\n", c);
    } else {
      // Print ASCII character
      printf("%d ('%c')\r\n", c, c);
    }
  }

  return 0;
}
