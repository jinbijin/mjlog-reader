#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "parser.h"

void loop_body(const char *filename) {
  mj_elt *elt;
  parser(&elt, filename);
  elt_print_lines(elt);
  elt_delete(elt);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Wrong number of arguments; only the directory path is needed.\n");
    exit(EXIT_FAILURE);
  }

  // Open target directory and initialise stuff
  DIR *dir = opendir(argv[1]);
  struct dirent *files;
  char filename[MJ_PATH_LIMIT];
  int n = 0;

  if (dir == NULL) {
    printf("Error while opening directory \"%s\".\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // Loop over files in directory
  files = readdir(dir);
  while (files != NULL) {
    // Use `length == 37` as condition. This is the length of the filename with
    // `.mjlog` appended.
    if (strlen(files->d_name) == 37) {
      // Assemble complete path.
      n++;
      strcpy(filename, argv[1]);
      strncat(filename, "/", 2);
      strncat(filename, files->d_name, 38); // To avoid buffer overruns.

      loop_body(filename);
    }
    files = readdir(dir);
  }
  closedir(dir);

  // Success!
  printf("Done!\n");
  return EXIT_SUCCESS;
}
