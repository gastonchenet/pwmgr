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
  size_t pwc, uwc, i;
  uint64_t wv = 1;
  int gen = 0;

  // Check arguments
  if (argc < 2 || argc > 3)
  {
    printf("Usage: %s <website.url> [generation]\n", argv[0]);
    return 1;
  }

  url = hostname(argv[1]);

  if (argc == 3)
  {
    gen = atoi(argv[2]);
  }
  

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

  for (i = 0; i < strlen(url); i++)
  {
    wv *= (uint16_t) url[i];
  }

  // Generate attended password
  for (i = 0; i < ATTENDED_LENGTH; i++)
  {
    pwc = (size_t) password[i % strlen(password)];
    uwc = (size_t) url[i % strlen(url)];
    attended[i] = CHARSET[(pwc + uwc + wv + gen) * alias(i) % strlen(CHARSET)];
  }

  attended[ATTENDED_LENGTH] = '\0';

  // Print attended password
  printf("%s\n", attended);

  return 0;
}
