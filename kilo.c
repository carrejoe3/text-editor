#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>

// Original terminal attributes
struct termios orig_termios;

// Print error message and exit
void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  // Restore original terminal attributes
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
    die("tcsetattr");
  }
}

void enableRawMode() {
  // Save original terminal attributes
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
    die("tcgetattr");
  }

  // Call disableRawMode() when program exits
  atexit(disableRawMode);

  // Set raw mode
  struct termios raw = orig_termios;

  // Disable IXON (software flow control)
  // Disable ICRNL (carriage return -> newline)
  // Disable BRKINT (break condition)
  // Disable INPCK (parity check)
  // Disable ISTRIP (strip 8th bit)
  raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
  // Disable OPOST (output processing)
  raw.c_oflag &= ~(OPOST);
  // Disable ECHO (print input characters)
  // Disable ICANON (canonical mode)
  // Disable ISIG (signals)
  // Disable IEXTEN (extended input processing)
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  // Set character size to 8 bits
  raw.c_cflag |= (CS8);
  // Set timeout for read() to return immediately
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  // Apply changes
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    die("tcsetattr");
  }
}

int main() {
  enableRawMode();

  while (1) {
    char c = '\0';
    // Read 1 byte from standard input
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
      die("read");
    }

    if (c == 'q') {
      break;
    }

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
