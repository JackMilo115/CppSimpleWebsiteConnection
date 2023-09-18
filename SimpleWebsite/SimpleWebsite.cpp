#include <iostream>

#ifdef _WIN32
#define _WIN32_WINNIT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

int main()
{
	asio::error_code ec;
	// Create a context
	asio::io_context context;
	// Get the address of somewhere we with to connect to
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);
	// Create a socket
	asio::ip::tcp::socket socket(context);
	// Tell socket to try and connect
	socket.connect(endpoint, ec);

	if (!ec)
	{
		std::cout << "Connected!" << std::endl;
	}
	else
	{
		std::cout << "Failted to connect to address:\n" << ec.message() << std::endl;
	}

	return 0;
}