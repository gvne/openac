namespace oac {
namespace ip {
namespace udp {

class PubSocket {
 public:
  using protocol_type = asio::ip::udp;
  using endpoint_type = typename protocol_type::endpoint;

  PubSocket(asio::io_context& context,
            std::chrono::milliseconds&& ttl = std::chrono::seconds(1),
            std::optional<uint16_t> max_connection_count = {});
  void Open(const protocol_type& protocol, asio::error_code& err);

  void Bind(const endpoint_type& endpoint, asio::error_code& err);

  template <typename ConstBufferSequence>
  void Send(const ConstBufferSequence& buffers, asio::error_code& err) {
    auto now = std::chrono::system_clock::now();
    for (const auto& sub : subs_) {
      if (sub.second + ttl_ < now) {
        // TODO: remove from the subs
        // TODO: log
        continue;
      }
      data_socket_.send_to(buffers, sub.first, 0, err);
    }
  }

  endpoint_type local_endpoint() const;

  const uint8_t kControlVersion = 1;
  ASTRUCT(ControlRequest,
    (uint8_t, version),
    (uint8_t, subscriber_ipv4_part0),
    (uint8_t, subscriber_ipv4_part1),
    (uint8_t, subscriber_ipv4_part2),
    (uint8_t, subscriber_ipv4_part3),
    (uint16_t, subscriber_port),
    (uint8_t, operation_id)  // 0 = start or keep alive. 1 = stop
  );
  ASTRUCT(ControlResponse,
    (uint8_t, version),
    (uint8_t, error),
    (uint16_t, ttl_ms)
  );

 private:
  void Listen();

 private:
  asio::ip::udp::socket data_socket_;
  asio::ip::tcp::socket control_socket_;
  std::chrono::milliseconds ttl_;
  std::optional<uint16_t> max_connection_count_;

  std::map<
    asio::ip::udp::endpoint,
    std::chrono::system_clock::time_point> subs_;
};


}  // namespace udp
}  // namespace ip
}  // namespace oac
