# project-tiger

#### Build directions
```
cd src
mkdir build
cmake ..

#build server and client
make server client

#run tests
make test

#run linter, spell checker and static analysis
make code-tests
```

##Libraries
#### SQLite
[SQLite](https://www.sqlite.org/) is available in the src/libraries/sqlite folder. In its current form, it is the amalgamated source (i.e. all source files merged, which is the preferred distribution). SQLite is in the public domain.

#### Crow
[Crow](https://github.com/CrowCpp/Crow) is available in src/libraries/Crow. It is C++ framework for HTTP and Websocket web services with routing for servers and clients. It is covered by a BSD-style license.

## Tools for linting, static analysis etc.
####Cpplint
We will be using cpplint, which enforces the [google style guide](https://google.github.io/styleguide/cppguide.html). To install, run `pip3 install cpplint`.  
TODO: Google's style guide is pretty restrictive, so we'll disable some of the warnings later on, in particular for GNU extensions and some C-style constructs.


####Cppcheck (static analysis)
Install [cppcheck](https://cppcheck.sourceforge.io/) and run `cppcheck --enable=warning,style,unusedFunction <path to repo>`.  

####Codespell (spelling)
Install [codespell](https://github.com/codespell-project/codespell) using `pip3 install codespell` and run `codespell <path to repo>` to find spelling mistakes.
