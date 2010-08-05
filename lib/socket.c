typedef struct Net_Socket {
    IO_Handle* handle;
    mushort refcount;
} *Net_Socket;

void Net_Socket__init(Net_Socket** self, muint type) {
    mint fd; 
    if (Net_Socket_TCP == type) {
        fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    } else if (Net_Socket_UDP == type) {
        fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    } else {
        *self = 0;
        return;
    }

    MALLOC(self, sizeof Net_Socket);
    MINCREF(*self);
    IO_Handle__init(&(*self)->handle, fd); 
}

void Net_Socket__destroy(Net_Socket** self) {
    MDECREF((*self)->handle);
    MFREE(self);
}

void Net_Socket_listen__e(Net_Socket* self, mint* out) {
    mint sd = IO_Handle_fd(self->handle); 
    *out = listen(sd, 1);
}

void Net_Socket_accept__e(Net_Socket* self, Net_Socket** out) {
    mint sd = IO_Handle_fd(self->handle); 

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof struct sockaddr;

    mint fd = accept(sd, (struct sockaddr*)&addr, addrlen);
    if (fd == -1) {
        *out = 0; 
    }

    MALLOC(out, sizeof Net_Socket);
    MINCREF(*self);
    IO_Handle__init(&(*out)->handle, fd);
}

void Net_Socket_connect__e(Net_Socket* self, const Net_Address* address, mint* out) {
    mint sd = IO_Handle_fd(self->handle); 
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(address->port);
    addr.sin_addr.s_addr = htonl(address->ip);

    *out = connect(sd, (struct sockaddr*)&addr, sizeof struct sockaddr_in);
}

void Net_Socket_send__e(Net_Socket* self, Net_Packet* packet) {
    mint sd = IO_Handle_fd(self->handle); 
    Net_Address address;
    Net_Packet_address(packet, address);
    Range buffer = Net_Packet_buffer(packet);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(address.port);
    addr.sin_addr.s_addr = htonl(address.ip);

    *out = sendto(sd, buffer->offset, Range_length(buffer), 0, 
        (struct sockaddr*)&addr, sizeof struct sockaddr_in);
}

void Net_Socket_receive__e(Net_Socket* self, Net_Packet* packet) {
    mint sd = IO_Handle_fd(self->handle); 
    Net_Address address;
    Range buffer = Net_Packet_buffer(packet);

    struct sockaddr_in addr;

    *out = recvfrom(sd, buffer->offset, Range_length(buffer), 0,
        (struct sockaddr*)&adddr, sizeof struct sockaddr_in);

    address.port = ntohs(addr.sin_port);
    address.ip = ntohl(addr.sin_addr.s_addr);
    Net_Packet_address__e(&address);
}


typedef struct IO_Handle {
    mint fd;
} *IO_Handle;

typedef struct Range {
    Array* array;
    void* offset; 
} Range;

void IO_Handle_read__e(IO_Handle* self, const Range* buffer, mint* out) {
    *out = read(self->fd, buffer->offset, Range_length(buffer));
}

void IO_Handle_write__e(IO_Handle* self, const Range* buffer, mint* out) {
    *out = write(self->fd, buffer->offset, Range_length(buffer));
}

void IO_Handle_seek__e(IO_Handle* self, mint position, mint* out) {
    *out = lseek(self->fd, position, SEEK_SET);
}

void IO_Handle_stat(IO_Handle* self, IO_Stat** stat) {

    // Stat
    *stat = 0;
}

void IO_Handle_close__e(IO_Handle* self) {
    close(self->fd());
}
