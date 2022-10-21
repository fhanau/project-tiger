# project-tiger
## About


### Build directions
```
#install required libraries
apt-get install --no-install-recommends -y libasio-dev curl libcurl4-openssl-devlibcurlpp-dev

cd src
mkdir build
cd build
cmake ..

#build server and client
make server client

#run tests
make test

```

### Run program, analysis tools and tests
```
#Launch server
./server/server

#Run example client, provides interface to run commands and talk with server
./client/client

#Run all unit tests
make run-tiger-test

#run linter, spell checker and static analysis
#Make sure the tools described below are available
make code-tests
```

## Libraries

#### curl, curlpp, asio
The program uses [libcurl](https://curl.se/libcurl/), [curlpp](https://github.com/jpbarrette/curlpp) and [asio](https://think-async.com/Asio/) for the network code and assumes that these are installed, they are not included in the project itself. The libraries are used in the client and server programs and are available under the Curl License, Boost Software License and MIT license, respectively.  
At present, curl and curlpp are also included in src/libraries, we may remove them in the future.

#### SQLite
[SQLite](https://www.sqlite.org/) is available in the src/libraries/sqlite folder. In its current form, it is the amalgamated source (i.e. all source files merged, which is the preferred distribution). SQLite is in the public domain. We use SQLite for the database class used by the server.

#### Crow
[Crow](https://github.com/CrowCpp/Crow) is available in src/libraries/Crow. It is C++ framework for HTTP and Websocket web services with routing for the server. It is covered by a BSD-style license.

#### Openssl
[Openssl](https://github.com/janbar/openssl-cmake) is required by curl and included in the src/libraries/openssl-cmake folder. It is available under the OpenSSL License.

## Testing/Mocking
We use [GoogleTest and GMock](https://github.com/google/googletest), available in test/googletest. The libraries use a 3-Clause BSD license. Use `make run-tiger-test` to run the tests.  

## Tools for linting, static analysis etc.
#### Cpplint
We will be using cpplint, which enforces the [google style guide](https://google.github.io/styleguide/cppguide.html). To install, run `pip3 install cpplint`.  
TODO: Google's style guide is pretty restrictive, so we'll disable some of the warnings later on, in particular for GNU extensions and some C-style constructs.


#### Cppcheck (static analysis)
Install [cppcheck](https://cppcheck.sourceforge.io/) and run `cppcheck --enable=warning,style,unusedFunction <path to repo>`.  

#### Codespell (spelling)
Install [codespell](https://github.com/codespell-project/codespell) using `pip3 install codespell` and run `codespell <path to repo>` to find spelling mistakes.
