# mjlog-reader

## A tool for analysing Tenhou's log files

### Usage

Fill out `mjquery.c` and the struct definition in `mjquery.h`, and compile the files using the included `Makefile`. Included is a simple example; in the future, more examples may show up in a different repository.

The resulting executable `mjlog.o` takes one argument, which is the directory containing the logfiles to be analysed.
Their filenames need have their standard names (and an `.mjlog` extension); this is currently checked purely by filename length, which is 37 characters.

Note that access to the "parsed" content is currently quite limited; this is work in progress.
