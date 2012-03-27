/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#include "Stream.hpp"
#include <iostream>
#include <fstream>
#ifdef WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

Stream::Stream(const std::string& file) : error_(false) {
    // Create stream that writes output to 'file.' 
    buffer_.reserve(1024);
    if (file == "-") {
#ifdef WINDOWS
        fd_ = GetStdHandle(STD_OUTPUT_HANDLE);
#else
        fd_ = 1;
#endif
    } else {
#ifdef WINDOWS
        fd_ = ::CreateFile(
            file.c_str(), // file name
            GENERIC_WRITE, // desired access 
            FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
            0, // security attributes
            CREATE_ALWAYS, // creation disposition
            0, // attributes and flags
            0); // template file
        if (fd_ == INVALID_HANDLE_VALUE) {
            char message[1024];
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, 
                message, 1024, 0);
            message_ = message;
            error_ = true;
        }
#else
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        fd_ = ::open(file.c_str(), O_WRONLY|O_CREAT|O_TRUNC, mode);
        if (fd_ < 0) {
            error_ = true;
        }
#endif
    }
}

#ifdef WINDOWS
Stream::Stream(HANDLE fd) : error_(false) {
    // Creates an output stream that writes to a fixed file descriptor.
    buffer_.reserve(1024);
    fd_ = fd;
}
#else
Stream::Stream(int fd) : error_(false) {
    // Creates an output stream that writes to a fixed file descriptor.
    buffer_.reserve(1024);
    fd_ = fd;	
}
#endif

Stream::~Stream() {
    // Flush the remaining buffer, and then close the stream if it isn't a
    // standard input/output/error stream.
    flush();
#ifdef WINDOWS
	if(fd_ != GetStdHandle(STD_OUTPUT_HANDLE)) {
		CloseHandle(fd_);
	}
#else
    if (fd_ != 1 && fd_ != 0 && fd_ != 2) {
        ::close(fd_);
    }
#endif
}

Stream::Ptr Stream::sterr() {
#ifdef WINDOWS
    static Stream::Ptr sterr = new Stream(GetStdHandle(STD_ERROR_HANDLE));
#else
    static Stream::Ptr sterr = new Stream(2);
#endif
    return sterr;
}

Stream::Ptr Stream::stout() {
#ifdef WINDOWS
    static Stream::Ptr stout = new Stream(GetStdHandle(STD_OUTPUT_HANDLE));
#else
    static Stream::Ptr stout = new Stream(1);
#endif
    return stout;
}

void Stream::write(const char* data, int len) {
    // Writes 'data' to the stream.  This function buffers the output for
    // better low-level write performance.
	if (len == 0) { return; }
    if (buffer_.size() + len > buffer_.capacity()) {
        flush();
    }
    if (len > buffer_.capacity()) {
        write_direct(data, len);
    } else {
        int offset = buffer_.size();
        buffer_.resize(buffer_.size()+len);
        memcpy(&buffer_.front()+offset, data, len);
    }
}

void Stream::write_direct(const char* data, int len) {
    // Write all bytes in buffer 'data' immediately, without buffering.
    int offset = 0;
    do {
#ifdef WINDOWS
        DWORD written = 0;
        if (!WriteFile(fd_, data, len, &written, 0)) {
            error_ = true;
			std::cerr << GetLastError() << std::endl;
            return;
        }
#else
        int written = ::write(fd_, data+offset, len-offset);
        if (written < 0) {
            error_ = true;
            return;
        }
#endif
        offset += written;
    } while(offset < len);
}

void Stream::flush() {
    // Write all bytes in the buffer immediately.
    if (!buffer_.empty()) {
        write_direct(&buffer_.front(), buffer_.size());
        buffer_.resize(0);
    }
}
