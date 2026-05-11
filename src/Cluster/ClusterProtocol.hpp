#pragma once

#include <cstdint>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

namespace Cluster {

constexpr uint8_t MSG_HELLO = 0x01; // slave to server: ready
constexpr uint8_t MSG_JOB =
    0x02; // server to slave: render params + scene content
constexpr uint8_t MSG_TILE_ASSIGN = 0x03; // server to slave: list of tile idx
constexpr uint8_t MSG_FRAME_UPDATE =
    0x04; // slave to server: completed tile pixels
constexpr uint8_t MSG_REQUEST_MORE =
    0x05; // slave to server: request more tiles
constexpr uint8_t MSG_NO_MORE =
    0x06; // server to slave: no more tiles available
constexpr uint8_t MSG_ALL_DONE =
    0x07; // slave to server: all assigned tiles rendered
constexpr uint8_t MSG_RERENDER =
    0x08; // server to slave: camera moved, start fresh render

constexpr int TILE_SIZE = 16;
constexpr int DISPLAY_INTERVAL_MS = 4;
constexpr int SLAVE_UPDATE_INTERVAL_MS = 100;
constexpr int TILES_PER_BATCH = 64;

inline void writeU32(std::vector<uint8_t> &buf, uint32_t v) {
  buf.push_back(v & 0xFF);
  buf.push_back((v >> 8) & 0xFF);
  buf.push_back((v >> 16) & 0xFF);
  buf.push_back((v >> 24) & 0xFF);
}

inline uint32_t readU32(const uint8_t *p) {
  return uint32_t(p[0]) | (uint32_t(p[1]) << 8) | (uint32_t(p[2]) << 16) |
         (uint32_t(p[3]) << 24);
}

inline bool sendAll(int fd, const void *data, size_t len) {
  const uint8_t *p = static_cast<const uint8_t *>(data);
  while (len > 0) {
    ssize_t n = ::send(fd, p, len, MSG_NOSIGNAL);
    if (n <= 0)
      return false;
    p += n;
    len -= static_cast<size_t>(n);
  }
  return true;
}

inline bool recvAll(int fd, void *data, size_t len) {
  uint8_t *p = static_cast<uint8_t *>(data);
  while (len > 0) {
    ssize_t n = ::recv(fd, p, len, 0);
    if (n <= 0)
      return false;
    p += n;
    len -= static_cast<size_t>(n);
  }
  return true;
}

inline bool sendMsg(int fd, uint8_t type, const std::vector<uint8_t> &payload) {
  uint8_t header[5];
  header[0] = type;
  uint32_t len = static_cast<uint32_t>(payload.size());
  header[1] = len & 0xFF;
  header[2] = (len >> 8) & 0xFF;
  header[3] = (len >> 16) & 0xFF;
  header[4] = (len >> 24) & 0xFF;
  if (!sendAll(fd, header, 5))
    return false;
  if (!payload.empty() && !sendAll(fd, payload.data(), payload.size()))
    return false;
  return true;
}

inline bool recvMsg(int fd, uint8_t &type, std::vector<uint8_t> &payload) {
  uint8_t header[5];
  if (!recvAll(fd, header, 5))
    return false;
  type = header[0];
  uint32_t payloadLen = uint32_t(header[1]) | (uint32_t(header[2]) << 8) |
                        (uint32_t(header[3]) << 16) |
                        (uint32_t(header[4]) << 24);
  payload.resize(payloadLen);
  if (payloadLen > 0 && !recvAll(fd, payload.data(), payloadLen))
    return false;
  return true;
}

} // namespace Cluster
