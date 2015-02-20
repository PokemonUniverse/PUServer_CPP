#ifndef __pusocket_h_
#define __pusocket_h_

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class PUSocket
{
public:
	PUSocket(boost::asio::io_service &io_service) : socket(io_service) {}
	~PUSocket() {}

	boost::asio::ip::tcp::socket socket;
};

#endif