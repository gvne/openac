namespace oac {
namespace ip {
namespace udp {

class SubSocket {
 public:
  using protocol_type = asio::ip::udp;
  using endpoint_type = typename protocol_type::endpoint;

  SubSocket(asio::io_context& context);
  void Open(const protocol_type& protocol, asio::error_code& err);

  void Connect(const endpoint_type& endpoint, asio::error_code& err);

  template <typename MutableBufferSequence>
  void AsyncReceiveFrom(
      const MutableBufferSequence& buffers,
      const endpoint_type& endpoint,
      std::function<void(const asio::error_code& err, std::size_t size)> fct) {
    data_socket_.async_receive_from(buffers, endpoint, std::move(fct));
  }

 private:
  void Listen();
  void Reconnect();

 private:
  asio::ip::udp::socket data_socket_;
  asio::ip::tcp::socket control_socket_;
};

}  // namespace udp
}  // namespace ip
}  // namespace oac
