#remove any existing db and tokens to have consistent account ID and a clean database at the start.
rm -f client-token.dat client-token-alt.dat db.db
./server &
server_pid=$!
#sleep so the server can start up before client starts sending
sleep 0.1
cat ../test/end-end/client-commands.txt | ./client >client_out_actual.txt

#start a new client using a different token and check that it gets different results than the first client. This demonstrates the multiple client feature.
cat ../test/end-end/client-commands-2.txt | ./client client-token-alt.dat >client_out_actual-2.txt
#shut down the server
kill -9 $server_pid

#restart the server and expect to get the same output for the query commands when running client with same token. This demonstrates server persistence.
./server &
server_pid=$!
#sleep so the server can start up before client starts sending
sleep 0.1
cat ../test/end-end/client-commands-2-persistent.txt | ./client client-token-alt.dat >client_out_actual-2-persistent.txt
#shut down the server
kill -9 $server_pid

#check that output is same as expected
cmp client_out_actual.txt ../test/end-end/client_out_expected.txt
cmp client_out_actual-2.txt ../test/end-end/client_out_expected-2.txt
cmp client_out_actual-2-persistent.txt ../test/end-end/client_out_expected-2-persistent.txt
