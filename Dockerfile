# syntax=docker/dockerfile:1

# Stage 1 - C++ compilation
FROM alpine:3.22.4 AS builder

# Install required tools
RUN apk add --no-cache g++ cmake make upx git openssh-client

# Add GitHub to known SSH hosts
RUN mkdir -p -m 0700 ~/.ssh && ssh-keyscan github.com >> ~/.ssh/known_hosts

# Create and change working directory
WORKDIR /app

# Clone repository using SSH mount
RUN --mount=type=ssh git clone git@github.com:SzymonKowalik/pawcho_zadanie1_kod.git .

# Compile code statically with size optimizations
RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DCMAKE_C_FLAGS="-Os -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -fvisibility=hidden" \
    -DCMAKE_EXE_LINKER_FLAGS="-static -Wl,--gc-sections -s" ../code && \
    cmake --build . --config MinSizeRel && \
    upx --ultra-brute weatherApp # Compress binary

# Stage 2 - Run compiled program
FROM scratch

# Author info
LABEL org.opencontainers.image.authors="Szymon Kowalik"

# Create and change working directory
WORKDIR /app

# Copy binary file and static html from Stage 1
COPY --from=builder /app/build/weatherApp /app/server
COPY --from=builder /app/static /app/static

# Inform about the port app listens on
EXPOSE 8000

# Change user id
USER 1000:1000

# Check if script is still running
HEALTHCHECK --interval=30s --timeout=3s \
  CMD ["./server", "--check"]

# Execute program as PID=1
ENTRYPOINT ["./server"]
