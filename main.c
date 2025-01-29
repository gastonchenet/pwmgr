#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define ATTENDED_LENGTH 24
#define CHARSET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~*$@#"

#define KEY_A 6364136223846793005
#define KEY_B 1442695040888963407
#define KEY_C 1844674407370955161

uint64_t alias(size_t i)
{
  // Alias method
  return (i * KEY_A + KEY_B) % KEY_C;
}

char* hostname(char* url)
{
  char* hostname = malloc(strlen(url) + 1);
  char* p = hostname;
  char* q = url;

  // Remove protocol if present
  if (strncmp(url, "http://", 7) == 0)
  {
    q += 7;
  }
  else if (strncmp(url, "https://", 8) == 0)
  {
    q += 8;
  }

  // Copy hostname
  while (*q && *q != '/')
  {
    *p++ = *q++;
  }

  *p = '\0';

  return hostname;
}

int main(int argc, char** argv)
{
  char attended[ATTENDED_LENGTH + 1];
  char* password;
  char* confirm_password;
  char* url;
  int pwc;
  size_t i;

  // Check arguments
  if (argc < 2 || argc > 2)
  {
    printf("Usage: %s <arg1>\n", argv[0]);
    return 1;
  }

  url = hostname(argv[1]);

  // Ask for password
  printf("Enter password: ");
  password = strdup(getpass(""));
  printf("Confirm password: ");
  confirm_password = strdup(getpass(""));

  // Check password
  if (strcmp(password, confirm_password) != 0)
  {
    printf("Passwords do not match\n");
    return 1;
  }

  // Generate attended password
  for (i = 0; i < ATTENDED_LENGTH; i++)
  {
    pwc = password[i % strlen(password)];
    attended[i] = CHARSET[(i + pwc + alias(i)) % strlen(CHARSET)];
  }

  attended[ATTENDED_LENGTH] = '\0';

  // Print attended password
  printf("%s\n", attended);

  return 0;
}
