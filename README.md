# Tasteful
Custom C Testing Framework made from Scratch.

A Testing Suite is composed of many Tests (functions). Each Testing Suite should be created in its own file.

See example.c to see how to initialize and load the tests into a suite.

After having the suites created, just by running ./tasteful will find every binry file that has a "test" in its name (or any variation with CAPS) and run the funciton run_suite of every suite.

The use of a makefile in order to automate the process of compiling and running is suggested.
