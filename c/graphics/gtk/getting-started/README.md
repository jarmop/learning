# Applications 1, 3, 9

The application* directories are copied from the examples directory in the GTK repo. The examples are also included in the GTK API reference documentation.

## Build and run

    $ cd application1
    $ make -f Makefile.example
    $ ./exampleapp

## Running with schema

The later demos are using GSettings,so they need a GSettings schema. The code for these demos contains a workaround to look for the schema in the current directory, so this works:

    $ cd application5
    & ./exampleapp window.ui
