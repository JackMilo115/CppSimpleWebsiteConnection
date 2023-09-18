#include <iostream>
#include <chrono>

#ifdef _WIN32
#define _WIN32_WINNIT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

// Create reusable buffer to store returned data
std::vector<char> vBuffer(20 * 1024);

// Asynchronously read returned data
void GrabSomeData(asio::ip::tcp::socket& socket)
{
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
		[&](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::cout << "\n\nRead " << length << " bytes\n\n";

				for (int i = 0; i < length; i++)
				{
					std::cout << vBuffer[i];
				}

				// Continues grabbing data until there is none left.
				GrabSomeData(socket);
			}
		}
	);
}
int main()
{
	// Create error code storage
	asio::error_code ec;

	// Create a context
	asio::io_context context;

	// Give some fake tasks to asio so the context doesn't finish
	asio::io_context::work idleWork(context);

	// Run the context in its own thread
	std::thread thrContext = std::thread([&]() { context.run(); });

	// Get the address of somewhere we with to connect to - example.com
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80);

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
		GrabSomeData(socket);

		// Compose request
		std::string sRequest =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";

		// Send request
		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

		// Manually wait
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(20000ms);

		context.stop();
		if (thrContext.joinable())
		{
			thrContext.join();
		}
	}

	return 0;
}