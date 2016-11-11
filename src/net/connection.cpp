#include "connection.h"
#include <unistd.h>
#include <string.h>
#include <list>
#include <poll.h>


namespace net
{
	struct data_chunk {
		static const int MAX_DATA = 4; // 1KB chunks
		uint8_t data[MAX_DATA + 1];// MAX_DATA plus one byte to hold a null character
		ssize_t count;
		data_chunk() : count(0) {
			// Clear the data array
			memset(data, 0, sizeof(uint8_t) * (MAX_DATA + 1));
		}
	};

#pragma region constructors
	connection::connection(int fd) : _fd(fd), _open(true) { }
#pragma endregion


#pragma region public methods
	bool connection::is_valid() const
	{
		pollfd pfd = { _fd, POLLHUP, 0 };
		if (::poll(&pfd, 1, 0) == 0) {
			return !(pfd.revents & POLLHUP);
		}
		return false;
	}
	int connection::has_data(const int &timeout) const
	{
		// TODO: FIGURE OUT WHY ::POLL() IS RETURNING 1 INSTEAD OF 0
		pollfd pfd = { _fd, POLLIN | POLLPRI, 0 };
		int res = ::poll(&pfd, 1, timeout);
		if (res >= 0) {
			return pfd.revents & POLLIN || pfd.revents & POLLPRI;
		}
		else if (res) {
			return -1;
		}
		return 0;
	}
	ssize_t connection::read(uint8_t * buf, const size_t &count) const
	{
		return _open ? ::read(_fd, buf, count) : -1;
	}
	ssize_t connection::read_all(uint8_t ** buf) const
	{
		std::list<data_chunk*> data;
		data_chunk *curr;
		ssize_t count = 0;
		while (true)
		{
			curr = new data_chunk();
			// Read in the data chunk
			curr->count = ::read(_fd, curr->data, data_chunk::MAX_DATA);
			count += curr->count;
			// If the previous chunk grabbed the last of the data
			if (curr->count <= 0) {
				delete curr;
				break;
			}
			// Data was read
			else {
				data.push_back(curr);
				// If there is no more data to be read
				if (curr->count < data_chunk::MAX_DATA) {
					break;
				}
			}
		}
		(*buf) = new uint8_t[count];
		ssize_t index = 0;
		for (data_chunk* gram : data) {
			memcpy((*buf) + index, gram->data, gram->count);
			index += gram->count;
			delete gram;
		}
		return count;
	}
	ssize_t connection::sread(std::string *str_out, const size_t &count) const
	{
		char *buf = new char[count + 1];
		buf[count] = '\0';
		ssize_t read = ::read(_fd, (uint8_t*)buf, count);
		*str_out = std::string(buf);
		delete[] buf;
		return read;
	}
	ssize_t connection::sread_all(std::string *str_out) const
	{
		char * buf;
		ssize_t count = read_all((uint8_t**)&buf);
		*str_out = std::string(buf, count);
		delete[] buf;
		return count;
	}

	ssize_t connection::write(const uint8_t * buf, const size_t &count) const
	{
		return _open ? ::write(_fd, buf, count) : -1;
	}
	ssize_t connection::swrite(const std::string &data) const
	{
		return _open ? ::write(_fd, data.c_str(), data.size()) : -1;
	}
#pragma endregion


#pragma region private methods
	int connection::close()
	{
		if (_open && _fd >= 0) {
			int res = ::close(_fd);
			if (!res) {
				_open = false;
				_fd = -1;
			}
			return res;
		}
		return 0;
	}
#pragma endregion


	ssize_t connection::sendmsg(const message *msg) const
	{
		ssize_t hresult = ::write(_fd, &msg->header, sizeof(message::message_header));
		if (hresult < 0) {
			return -1;
		}
		if (msg->header.data_size > 0) {
			ssize_t dresult = ::write(_fd, msg->data, msg->header.data_size);
			if (dresult < 0) {
				return -1;
			}
			hresult += dresult;
		}
		return hresult;
	}
	ssize_t connection::recvmsg(message *msg) const
	{
		size_t msg_hdr_size = sizeof(message::message_header);
		ssize_t hresult = ::read(_fd, &msg->header, msg_hdr_size);
		if (hresult < 0) {
			return -1;
		}
		if (msg->header.data_size > 0) {
			uint8_t *data = new uint8_t[msg->header.data_size];
			ssize_t dresult = ::read(_fd, data, msg->header.data_size);
			if (dresult < 0) {
				delete[] data;
				data = nullptr;
				return -1;
			}
			msg->data = data;
			hresult += dresult;
		}
		return hresult;
	}

}
