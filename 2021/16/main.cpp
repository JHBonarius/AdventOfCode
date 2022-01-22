#include "../my_algorithm.hpp"
#include "../readinputdata.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

static constexpr auto testInput1{"D2FE28"};
static constexpr auto testInput2{"38006F45291200"};
static constexpr auto testInput3{"EE00D40C823060"};
static constexpr auto testInput4{
    "8A004A801A8002F478\n"             // 16
    "620080001611562C8802118E34\n"     // 12
    "C0015000016115A2E0802F182340\n"   // 23
    "A0016C880162017C3686B18A3D4780\n" // 31
};
static constexpr auto testInput5{
    "C200B40A82\n"                 // 3
    "04005AC33890\n"               // 54
    "880086C3E88112\n"             // 7
    "CE00C43D881120\n"             // 9
    "D8005AC2A8F0\n"               // 1
    "F600BC2D8F\n"                 // 0
    "9C005AC2F8F0\n"               // 0
    "9C0141080250320F1802104A08\n" // 1
};

using Value_t = int64_t;

enum class PacketType {
  SUM = 0,
  PRODUCT = 1,
  MINIMUM = 2,
  MAXIMUM = 3,
  LITERAL = 4,
  GREATER_THEN = 5,
  LESS_THEN = 6,
  EQUAL_TO = 7,
};

struct Literal {
  Value_t value{};
};

struct Packet;

struct Operator {
  PacketType operation{};
  std::vector<Packet> sub_packets;
};

struct Packet {
  int version{};
  std::variant<Literal, Operator> contents{};
};

template <int N> auto take_N_bits(std::vector<bool>::const_iterator &it) {
  auto retVal{static_cast<Value_t>(0)};
  for (auto i{0}; i < N; ++i) {
    retVal <<= 1;
    retVal |= *it++;
  }
  // std::cout << retVal << ": ";
  return retVal;
}

Packet parse_packet(std::vector<bool>::const_iterator &it);

auto parse_literal(std::vector<bool>::const_iterator &it) {
  auto value{static_cast<Value_t>(0)};
  while (true) {
    auto const isLast{!static_cast<bool>(take_N_bits<1>(it))};
    auto const group{take_N_bits<4>(it)};
    // std::cout << "Value: " << group << std::endl;
    value <<= 4;
    value += group;
    if (isLast) {
      break;
    }
  }
  // std::cout << "Final value: " << value << std::endl;
  return Literal{value};
}

auto parse_operator(std::vector<bool>::const_iterator &it,
                    PacketType operation) {
  auto op{Operator{operation, {}}};
  if (/*length_type_id*/ take_N_bits<1>(it)) {
    auto const nrSubPackets{take_N_bits<11>(it)};
    // std::cout << "nr of sub-packets: " << nrSubPackets << std::endl;
    op.sub_packets.reserve(nrSubPackets);
    generate_n(back_inserter(op.sub_packets), nrSubPackets,
               [&] { return parse_packet(it); });
  } else {
    auto const length{take_N_bits<15>(it)};
    // std::cout << "packet length in bits: " << length << std::endl;
    // if (length == 0)
    //   exit(-1);
    auto const end{next(it, length)};
    while (it != end) {
      op.sub_packets.push_back(parse_packet(it));
    }
  }
  return op;
}

Packet parse_packet(std::vector<bool>::const_iterator &it) {
  auto packet{Packet{}};
  packet.version = take_N_bits<3>(it);
  // std::cout << "Packet version: " << packet.version << std::endl;
  auto const type_id{static_cast<PacketType>(take_N_bits<3>(it))};
  if (type_id == PacketType::LITERAL) {
    // std::cout << "Literal packet" << std::endl;
    packet.contents = parse_literal(it);
  } else { // PacketType::Operator
    // std::cout << "Operator packet" << std::endl;
    packet.contents = parse_operator(it, type_id);
  }
  return packet;
}

int get_accumulated_versions(Packet const &packet);

int get_accumulated_versions(Operator const &o) {
  return transform_reduce(
      cbegin(o.sub_packets), cend(o.sub_packets), 0, std::plus<>{},
      [](Packet const &packet) { return get_accumulated_versions(packet); });
}

int get_accumulated_versions(Packet const &packet) {
  return packet.version +
         std::visit(
             overloaded{
                 []([[maybe_unused]] Literal const &l) { return 0; },
                 [](Operator const &o) { return get_accumulated_versions(o); }},
             packet.contents);
}

Value_t get_result(Packet const &packet);

Value_t get_result(Operator const op) {
  auto const packetResultGetter{
      [](Packet const &packet) { return get_result(packet); }};
  switch (op.operation) {
  case PacketType::SUM:
    // REALLY missing a pipable left-fold operation in std::ranges
    return std::transform_reduce(cbegin(op.sub_packets), cend(op.sub_packets),
                                 static_cast<Value_t>(0), std::plus<>{},
                                 packetResultGetter);
    break;
  case PacketType::PRODUCT:
    // REALLY missing a pipable left-fold operation in std::ranges
    return std::transform_reduce(cbegin(op.sub_packets), cend(op.sub_packets),
                                 static_cast<Value_t>(1), std::multiplies<>{},
                                 packetResultGetter);
    break;
  case PacketType::MINIMUM:
    return std::ranges::min(op.sub_packets |
                            std::views::transform(packetResultGetter));
    break;
  case PacketType::MAXIMUM:
    return std::ranges::max(op.sub_packets |
                            std::views::transform(packetResultGetter));
    break;
  case PacketType::GREATER_THEN:
    return get_result(op.sub_packets[0]) > get_result(op.sub_packets[1]);
    break;
  case PacketType::LESS_THEN:
    return get_result(op.sub_packets[0]) < get_result(op.sub_packets[1]);
    break;
  case PacketType::EQUAL_TO:
    return get_result(op.sub_packets[0]) == get_result(op.sub_packets[1]);
    break;
  default:
    return false;
    break; // error
  }
}

Value_t get_result(Packet const &packet) {
  return std::visit(overloaded{[](Literal const &l) { return l.value; },
                               [](Operator const &o) { return get_result(o); }},
                    packet.contents);
}

int main() {
  auto const strVec{readinputdata<std::string>( // std::stringstream{testInput5}
      std::fstream("input")                     //
      )};

  auto const bitVecs{[&] {
    auto bitVecs{std::vector<std::vector<bool>>{}};
    bitVecs.reserve(size(strVec));
    transform(cbegin(strVec), cend(strVec), back_inserter(bitVecs),
              [](auto const &str) {
                auto bitVec{std::vector<bool>{}};
                bitVec.reserve(size(str) * 4);
                for (auto it{cbegin(str)}; it != cend(str); advance(it, 1)) {
                  auto charVal{hex_char_to_int(*it)};
                  for (auto i{3}; i >= 0; --i) {
                    bitVec.push_back(static_cast<bool>(charVal & (1 << i)));
                  }
                }
                // for (auto b : bitVec) {
                //   std::cout << (b ? "1" : "0");
                // }
                // std::cout << std::endl; // we want flush
                return bitVec;
              });
    return bitVecs;
  }()};

  auto const packets{[&] {
    auto packets{std::vector<Packet>{}};
    transform(cbegin(bitVecs), cend(bitVecs), back_inserter(packets),
              [](auto const &bitVec) {
                auto it = cbegin(bitVec); // need lvalue
                return parse_packet(it);
              });
    return packets;
  }()};

  auto const versions{[&] {
    auto versions{std::vector<int>{}};
    versions.reserve(size(packets));
    transform(
        cbegin(packets), cend(packets), back_inserter(versions),
        [](auto const &packet) { return get_accumulated_versions(packet); });
    return versions;
  }()};
  auto const versionsTotal{std::reduce(cbegin(versions), cend(versions))};
  std::cout << "Accumulated versions: " << versionsTotal << '\n';

  auto const results{[&] {
    auto results{std::vector<Value_t>{}};
    results.reserve(size(packets));
    transform(cbegin(packets), cend(packets), back_inserter(results),
              [](auto const &packet) { return get_result(packet); });
    return results;
  }()};
  for (auto result : results) {
    std::cout << "Result: " << result << '\n';
  }
}
