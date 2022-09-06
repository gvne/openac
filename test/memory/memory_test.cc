#include <gtest/gtest.h>
#include "oac/memory/circular_buffer.h"

TEST(Memory, CircularBuffer_Syntax) {
  const auto message_size = 882;
  const auto message_index = 8592;
  oac::mem::CircularBuffer<int16_t> buffer(44100 * 1);

  std::vector<int16_t> data(message_size);

  buffer.set_push_index(message_size * message_index);
  buffer.Push(data.data(), data.size());
  buffer.Clear();
  buffer.Pop(data.data(), data.size());
}

TEST(Memory, CircularBuffer_Bounds) {
  oac::mem::CircularBuffer<int16_t> buffer(100);

  for (int16_t index = 0; index < 10; index++) {
    std::vector<int16_t> data(12, index);
    buffer.Push(data.data(), data.size());
  }

  // Read the content as a complete buffer
  std::vector<int16_t> content(100);
  buffer.Pop(content.data(), content.size());

  // we pushed 120 samples (10 * 12). We get 20 samples overriden
  for (int16_t buff_index = 0; buff_index < 10; buff_index++) {
    for (int16_t buff_content_index = 0; buff_content_index < 12; buff_content_index++) {
      auto index = buff_index * 12 + buff_content_index;
      if (index < 20 || index >= 100) {
        continue;
      }
      ASSERT_EQ(content[index], buff_index);
    }
  }
  // for overriden samples
  // - The first 8 should be 8
  for (std::size_t index = 0; index < 8; index++) {
    ASSERT_EQ(content[index], 8);
  }
  // - The next 12 should be 9
  for (std::size_t index = 8; index < 8 + 12; index++) {
    ASSERT_EQ(content[index], 9);
  }
}
