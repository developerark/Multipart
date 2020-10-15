#pragma once
#include <string>
#include <sstream>
#include <memory>

namespace HTTP {
	class Multipart {
	private:
		std::string _boundary;
		std::string _body;
	public:
		Multipart(std::string const& boundary);
		void addTextParameter(std::string const& name, std::string const& value);
		void addFileParameter(std::string const& name, std::string const& filepath, std::string const& filename, std::string const& contentType);
		void addRawImage(std::string const& name, unsigned char* data, size_t size, std::string const& filename, std::string const& contentType);
		std::string getBody();
		std::string getContentType();
	};
}
