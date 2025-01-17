#include "socket.hh"
#include "util.hh"

#include <cstdlib>
#include <iostream>

using namespace std;

void get_URL(const string &host, const string &path) {
    // Your code here.

    // You will need to connect to the "http" service on
    // the computer whose name is in the "host" string,
    // then request the URL path given in the "path" string.

    // Then you'll need to print out everything the server sends back,
    // (not just one call to read() -- everything) until you reach
    // the "eof" (end of file).
	
	TCPSocket Tcp_socket;//默认无参构造
	//address.hh,Address(const std::string &hostname, const std::string &service);
	Address serveraddr(host,"http");
	////void connect(const Address &address);
	Tcp_socket.connect(serveraddr);
	//file_des---.hh,size_t write(const char *str, const bool write_all = true)
	Tcp_socket.write("GET "+path+" HTTP/1.1\r\n"+"Host: "+host+"\r\n");
	Tcp_socket.write("\r\n");
    //file_des--.hh,bool eof() const { return _internal_fd->_eof; }	
	Tcp_socket.shutdown(SHUT_WR); 
	while(!Tcp_socket.eof()){
	  //file_des--.hh,std::string read(const size_t limit = std::numeric_limits<size_t>::max());
			std::cout<<Tcp_socket.read();
	}
	Tcp_socket.close();
	exit(0);
    //cerr << "Function called: get_URL(" << host << ", " << path << ").\n";
    //cerr << "Warning: get_URL() has not been implemented yet.\n";
}

int main(int argc, char *argv[]) {
    try {
        if (argc <= 0) {
            abort();  // For sticklers: don't try to access argv[0] if argc <= 0.
        }

        // The program takes two command-line arguments: the hostname and "path" part of the URL.
        // Print the usage message unless there are these two arguments (plus the program name
        // itself, so arg count = 3 in total).
        if (argc != 3) {
            cerr << "Usage: " << argv[0] << " HOST PATH\n";
            cerr << "\tExample: " << argv[0] << " stanford.edu /class/cs144\n";
            return EXIT_FAILURE;
        }

        // Get the command-line arguments.
        const string host = argv[1];
        const string path = argv[2];

        // Call the student-written function.
        get_URL(host, path);
    } catch (const exception &e) {
        cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
