# project-tiger



##Libraries
####SQLite
[SQLite](https://www.sqlite.org/) is available in the src/libraries/sqlite folder. In its current form, it is the amalgamated source (i.e. all source files merged, which is the preferred distribution). SQLite is in the public domain.

## Tools for linting, static analysis etc.
####Cpplint
We will be using cpplint, which enforces the [google style guide](https://google.github.io/styleguide/cppguide.html). To install, run `pip3 install cpplint`. For linting, run `cpplint <path to repo/src/*.cpp>`.  
TODO: Update this to include all source files and add it as a target within cmake. Google's style guide is also pretty restrictive, so we'll disable some of the warnings later on, in particular for GNU extensions and some C-style constructs.


####Cppcheck (static analysis)
Install [cppcheck](https://cppcheck.sourceforge.io/) and run `cppcheck --enable=warning,style,unusedFunction <path to repo>`.  
Note: Update this to exclude any library submodules and add it as a target within cmake.

####Codespell (spelling)
Install [codespell](https://github.com/codespell-project/codespell) using `pip3 install codespell` and run `codespell <path to repo>` to find spelling mistakes.  
TODO: Update this to exclude any library submodules and add it as a target within cmake.