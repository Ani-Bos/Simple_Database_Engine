FROM alpine:latest

WORKDIR /app

RUN apk add --no-cache build-base

COPY mmap_create.cpp .

RUN g++ -std=c++17 -o mmap_create mmap_create.cpp

CMD ["./mmap_create"]