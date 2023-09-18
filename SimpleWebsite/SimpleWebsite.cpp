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
	// Get the address of somewhere we with to connect to - example.com
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);
	//asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 80);
	// Create a socket
	asio::ip::tcp::socket socket(context);
	// Tell socket to try and connect
	socket.connect(endpoint, ec);

	if (!ec) // Check if error code exists.
	{
		std::cout << "Connected!" << std::endl;
	}
	else
	{
		std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
	}

	if (socket.is_open())
	{
		// Compose request
		std::string sRequest =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";

		// Send request
		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

		// Wait for response
		socket.wait(socket.wait_read);

		// Check bytes returned
		size_t bytes = socket.available();
		std::cout << "Bytes Available " << bytes << std::endl;

		// Print bytes returned
		if (bytes > 0)
		{
			std::vector<char> vBuffer(bytes);
			socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

			for (auto c : vBuffer)
			{
				std::cout << c;
			}
		}
	}

	return 0;
}