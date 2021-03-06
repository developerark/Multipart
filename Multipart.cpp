#include "stdafx.h"
#include "Multipart.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../dataModel/Globals.h"

HTTP::Multipart::Multipart(std::string const& boundary)
{
	this->_body.reserve(4 * 1024 * 1024);
	// Sets the boundary
	this->_boundary = boundary;
}

void HTTP::Multipart::addTextParameter(std::string const& name, std::string const& value)
{
	// Add a text parameter to the body of the multipart request
	this->_body.append("--").append(this->_boundary).append("\r\n");
	this->_body.append("Content-Disposition: form-data; name=\"").append(name).append("\"\r\n\r\n");
	this->_body.append(value);
	this->_body.append("\r\n");
}

void HTTP::Multipart::addFileParameter(std::string const& name, std::string const& filepath, std::string const& filename, std::string const& contentType)
{
	// Stream the file from the disk
	std::ifstream ifile(filepath, std::ios::in | std::ios::binary | std::ios::ate);
	// Check to see if the file was opened properly
	if (!ifile.is_open()) {
		throw std::exception("Cannot open the file");
	}

	// Add a file parameter to the body of the multipart request
	this->_body.append("--").append(this->_boundary).append("\r\n");
	this->_body.append("Content-Disposition: form-data; name=\"").append(name).append("\"; filename=\"").append(filename).append("\"; Content-Type: image/").append(contentType).append("\r\n\r\n");

	// Get the file content into a buffer
	std::streamsize size = ifile.tellg();
	ifile.seekg(0, std::ios::beg);
	char *buff = new char[size];
	ifile.read(buff, size);
	ifile.close();
	std::string stream(buff, size);
	std::cout << stream.size() << std::endl;
	delete[] buff;

	// Append the buffer to the body
	this->_body.append(stream);
	this->_body.append("\r\n");
}

void HTTP::Multipart::addRawImage(std::string const& name, unsigned char* data, size_t size, std::string const& filename, std::string const& contentType)
{
	this->_body.append("--").append(this->_boundary).append("\r\n");
	this->_body.append("Content-Disposition: form-data; name=\"").append(name).append("\"; filename=\"").append(filename).append("\"\r\nContent-Type: image/").append(contentType).append("\r\n\r\n");
	
	// Append the stream to the body
	this->_body.append(reinterpret_cast<char const*>(data), size);
	this->_body.append("\r\n");
}

std::string HTTP::Multipart::getBody()
{
	// Append the closing section and return the body
	this->_body.append("--").append(this->_boundary).append("--");
	return this->_body;
}

std::string HTTP::Multipart::getContentType()
{
	// Returns the Content-Type value for the header
	std::string contentType = "multipart/form-data; boundary=\"" + this->_boundary + "\"";
	return contentType;
}
