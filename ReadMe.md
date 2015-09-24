Remote Performance Assistance, OOD project, Spring 2014

NOTE:
Make sure to run the server before running any client
Do not terminate the client by closing the window or pressing ctrl+c, it might lead to unexpected behaviour of not connecting again to the server, as the sockets would not be closed.

How upload download and search string work
-Upload
	Uploads all the files in a specified directory to the server and stores it in serrver in a folder named "UploadFiles" 
-Download
	Downloads all the files from the server upload directory("UploadFiles") and stores it in local "DownloadFiles" folder
-Search
	Carries out search processing on the server upload directory("UploadFiles"). The default values for searching for numbe rof requests and threads is 1. So if the client supplies invalid values, then 1 search request is sent and processing is carried out on 1 string 

Interpretation of search results:


Different threads being spawned can be seen at server console

How to use console server, console client, console tester and WPF client:

Console client:
	Command:
	ClientExecutive.exe LISTENING_PORT SERVER_IP/SERVER_NETWORK_NAME TARGET_PORT OPTIONS[/u dir, /d, /s srch_str nThreads nRequests]

	Usage examples:
	e.g for upload: ClientExecutive 8181 127.0.0.1 8282 /u ../TestDir"
	e.g for download: ClientExecutive 8181 127.0.0.1 8282 /d
	e.g for string search: ClientExecutive 8181 127.0.0.1 8282 /s "hi" 3 5"

	Argument explanation:
	1. LISTENING_PORT - Receives incoming connections including responses from the server "
	2. SERVER_IP/SERVER_NETWORK_NAME - Server's IP address or network name"
	3. SERVER_PORT - Server's port number"
	4. Use one of the following options:"
	/u dir - Upload option, uploads the files in the local directory"
	/d - download option, Downloads all the files from the server upload directory"
	/s \"srch_str\" nThreads nRequests - Makes nRequests to the server for searching search string \"srch_str\" in the server upload directory with nThreads "

Tester:


              Console.Write("\n\n  Please enter on command line as shown:");
              Console.Write("\n Tester.exe 5 8282 127.0.0.1 8181 \"search string\" 5 3\n\n");
              Console.Write("\n The description of each of the command-line arguments is as below:\n");
              Console.Write("\n1. Number of instances to start(e.g. 5)");
              Console.Write("\n2. Starting Client port to receive responses(e.g. 8282). \n The first client instance will start at this port and each of the successive instances start receiving at one more than this port. So if client port is 8282, and instances to be executed is 5, then the instances will start at 8282, 8283, 8284, 8285, 8286 respectively.\n So make sure all of the ports are not occupied.");
              Console.Write("\n3. Server IP address (e.g. 127.0.0.1)");
              Console.Write("\n4. Server port (e.g. 8181)");
              Console.Write("\n5. String to search for in the server's uploaded files dir (e.g. hi)");
              Console.Write("\n6. Number of threads to carry out search on (e.g. 5)");
              Console.Write("\n7. Number of search requests to make (e.g. 3)\n");

              Console.Write("\nNote: The number of client instances spawned will start listening for ports");



