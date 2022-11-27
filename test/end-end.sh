#remove existing db and tokens to have consistent account ID and a clean database at the start.
rm client-token.dat db.db
./server &
server_pid=$!
#sleep so the server can start up before client starts sending
sleep 0.1
cat ../test/client-commands.txt | ./client >client_out_actual.txt
#shut down the server
kill -9 $server_pid
#check that output is same as expected
cmp client_out_actual.txt ../test/client_out_expected.txt
