dockerfile
# Use official Alpine Linux with OpenSSL
FROM alpine:latest

# Install dependencies
RUN apk add --no-cache openssl libc-dev make gcc

# Set work directory
WORKDIR /app

# Copy source code and Makefile
COPY . /app

# Build the application
RUN make

# Set entrypoint
ENTRYPOINT ["./hello_world"]
