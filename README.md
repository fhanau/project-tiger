# project-tiger
## About
Project Tiger will offer data analytics for games and competitions. These analytics will be between records/milestones/statistics for each user that clients will host to general analytics regarding total user counts per day, week, month, etc. , most common winning strategies, and general overall user results. Basically, our server provides analytics for our clients to have access to.

The clients will be organizations that host games and/or competitions. Basically, any client that desires analytics regarding competitive play and milestone tracking will be able to utilize our API. Our clients will be allowed to display this data for its users and they can decide which data to showcase publicly, privately per each of their users, or retain internally for research purposes. With regards to research purposes, our clients could be able to use information we provide to make decisions as to how to further develop their applications.

The data we will be accumulating will be records/milestones per user (such as best result achieved, biggest bet won, biggest bet lost, total number of games, win-loss ratio). In order to keep track of these records, our server will store the username of all the users that our clients have. With regards to created data, our server can calculate the win-loss ratio and predict best possible move suggestions depending on which game is being played. The other forms of data are game logs, which contain metadata such as number of players, player ids, moves made, and money wagered. These game logs will be made accessible to all of our clients’ users to search for with game ids.

## API
The server can be accessed on port 18080 using the following API. At present, all API methods are implemented using HTTP GET requests using CROW on the server side and curl for the example clients.

```/``` URL used to test that the interface works, returns "Welcome to Project Tiger" if the server is working.

```/create/<username:string>/<password:string>``` Create a user with the given name and password. This and the following interfaces will be updated to support improved authentication in the second iteration. Returns an error if the user already exists.

```/login/<username:string>/<password:string>```  Log in the user with the given name and password. Returns an error if the user does not exist or the password is incorrect.

```/gametype/<type:string>/<sessionId:string>``` Upload game data to the database. Uses the sessionId to authenticate the user, returns an error message for inauthenticated users/users with wrong session ID.

```/public/<type:string>``` Request public game data of a given type. Returns an error if the type is invalid or there is an error when querying the data.

```private/total-earnings-all/<sessionId:string>/<host:string>``` Request total earnings for all players for a client. Must be logged in otherwise server returns -1.

```private/total-earnings-game/<sessionId:string>/<host:string>/<gameName:string>``` Request total earnings for a specific game for a client. Must be logged in otherwise server returns -1.

```private/total-earnings-all/<sessionId:string>/<host:string>/<player:string>``` Request total earnings for a specific player for a client. Must be logged in otherwise server returns -1.

```private/total-wins-all/<sessionId:string>/<host:string>``` Request total wins for all players for a client. Must be logged in otherwise server returns -1.

```private/total-wins-game/<sessionId:string>/<host:string>/<gameName:string>``` Request total wins for a specific game for a client. Must be logged in otherwise server returns -1.

```private/total-wins-all/<sessionId:string>/<host:string>/<playerId:string>``` Request total wins for a specific player for a client. Must be logged in otherwise server returns -1.

```private/total-losses-all/<sessionId:string>/<host:string>``` Request total losses for all players for a client. Must be logged in otherwise server returns -1.

```private/total-losses-game/<sessionId:string>/<host:string>/<gameName:string>``` Request total losses for a specific game for a client. Must be logged in otherwise server returns -1.

```private/total-losses-all/<sessionId:string>/<host:string>/<playerId:string>``` Request total losses for a specific player for a client. Must be logged in otherwise server returns -1.

### Build directions
```
#install required libraries
sudo apt install --no-install-recommends -y libasio-dev libcurl4-openssl-dev
pip install gcovr

#set up build folder
mkdir build
cd build
cmake ..

#build server and client
make server client

#build tests
make tiger-test

```

### Run program, analysis tools and tests
```
#Launch server
./server

#Run example client, provides interface to run commands and talk with server
./client

#Run all unit tests
make run-tiger-test

#run linter, spell checker and static analysis
#Make sure the tools described below are available
make code-lint
make code-analyze
make code-spell
```

## Libraries

#### curl, curlpp, asio
The program uses [libcurl](https://curl.se/libcurl/), [curlpp](https://github.com/jpbarrette/curlpp) and [asio](https://think-async.com/Asio/) for the network code. It assumes that asio and curl are installed while curlpp is included in src/libraries/curlpp. The libraries are used in the client and server programs and are available under the Curl License, MIT licence and Boost Software License, respectively.

#### SQLite
[SQLite](https://www.sqlite.org/) is available in the src/libraries/sqlite folder. In its current form, it is the amalgamated source (i.e. all source files merged, which is the preferred distribution). SQLite is in the public domain. We use SQLite for the database class used by the server.

#### Crow
[Crow](https://github.com/CrowCpp/Crow) is available in src/libraries/Crow. It is C++ framework for HTTP and Websocket web services with routing for the server. It is covered by a BSD-style license.

#### Openssl
[Openssl](https://github.com/janbar/openssl-cmake) is required by curl as well as being used as our authentication library for generating random tokens as well as computing SHA-256 hashes. It should come installed on Linux and is not included in this repository. It is available under the OpenSSL License.

## Testing/Mocking
We use [GoogleTest and GMock](https://github.com/google/googletest), available in test/googletest. The libraries use a 3-Clause BSD license. Use `make run-tiger-test` to run the tests.

## Tools for linting, static analysis etc.
#### Cpplint
We will be using cpplint, which enforces the [google style guide](https://google.github.io/styleguide/cppguide.html). To install, run `pip3 install cpplint`.

Note that Google's style guide is pretty restrictive, so we decided to disable some of the warnings, in particular for GNU extensions and some C-style constructs.

Use `make code-lint` to run the linter on our code base (excluding third-party libraries)

#### Cppcheck (static analysis)
Install [cppcheck](https://cppcheck.sourceforge.io/) and run `cppcheck --enable=warning,style,unusedFunction <path to repo>`. For this project, just run `make code-analyze`.

#### Codespell (spelling)
Install [codespell](https://github.com/codespell-project/codespell) using `pip3 install codespell` and run `codespell <path to repo>` to find spelling mistakes (or `make code-spell`).
