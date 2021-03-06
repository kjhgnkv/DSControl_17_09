#include "../structs.h"
#include "../sockets.h"
#include "../../utils/utils.h"

sockets::tcp::tcp(u_char* args, const struct pcap_pkthdr* pkt_hdr, const u_char* packet)
: ip(args, pkt_hdr, packet)
, tcp_ptr((struct sniff_tcp*)(packet + SIZE_ETHERNET + sizeof(sniff_ip)))
{
}

std::string get_flag(u_char th_flags)
{
    if (th_flags & TH_FIN)
    {
        return "0x01, FIN";
    }
    if (th_flags & TH_SYN)
    {
        return "0x02, SYN";
    }
    if (th_flags & TH_RST)
    {
        return "0x04, RST";
    }
    if (th_flags & TH_PUSH)
    {
        return "0x08, PUSH";
    }
    if (th_flags & TH_ACK)
    {
        return "0x10, ACK";
    }
    if (th_flags & TH_URG)
    {
        return "0x20, URG";
    }
    if (th_flags & TH_ECE)
    {
        return "0x40, ECE";
    }
    if (th_flags & TH_CWR)
    {
        return "0x80, CWR";
    }
    return "Unknown";
}

std::string sockets::tcp::_get_type()
{
    return "TCP";
}

std::vector<sockets::detail_view> sockets::tcp::_to_view()
{
    std::vector<sockets::detail_view> data = ip::_to_view();

    sockets::detail_view view;

    view.preview = "Transmission Control Protocol";
    // view.properties.push_back(utils::make_pair("Source", utils::to_string(ntohs(tcp_ptr->th_sport))));
    // view.properties.push_back(utils::make_pair("Destination", utils::to_string(ntohs(tcp_ptr->th_dport))));
    // view.properties.push_back(utils::make_pair("Sequence Number", utils::to_string(tcp_ptr->th_seq)));
    // view.properties.push_back(utils::make_pair("Acknowledgment Number", utils::to_string(tcp_ptr->th_ack)));
    // view.properties.push_back(utils::make_pair("Flag", get_flag(tcp_ptr->th_flags)));
    // view.properties.push_back(utils::make_pair("Checksum", utils::to_string(tcp_ptr->th_sum)));
    view.properties.insert(MapPairString("Source", utils::to_string(ntohs(tcp_ptr->th_sport))));
    view.properties.insert(MapPairString("Destination", utils::to_string(ntohs(tcp_ptr->th_dport))));
    view.properties.insert(MapPairString("Sequence Number", utils::to_string(tcp_ptr->th_seq)));
    view.properties.insert(MapPairString("Acknowledgment Number", utils::to_string(tcp_ptr->th_ack)));
    view.properties.insert(MapPairString("Flag", get_flag(tcp_ptr->th_flags)));
    view.properties.insert(MapPairString("Checksum", utils::to_string(tcp_ptr->th_sum)));

    data.push_back(view);

    return data;
}

sockets::table_view sockets::tcp::_to_row()
{
    struct sockets::table_view view;

    view.source = utils::to_string(ntohs(tcp_ptr->th_sport));
    view.destination = utils::to_string(ntohs(tcp_ptr->th_dport));

    view.protocol = _get_type();
    view.size = pkt_hdr->len;

    std::stringstream ss;
    hex_dump(ss, sizeof(ether_header) + sizeof(sniff_ip) + sizeof(sniff_tcp) + (const char*)packet, pkt_hdr->len);

    view.info = ss.str();

    return view;
}

std::string sockets::tcp::source_layer_(int type)
{
    switch (type - 1)
    {
        case LayerType::Physic:
        case LayerType::Network:
            return sockets::ip::source_layer_(type);
        case LayerType::Transport:
            return utils::to_string(ntohs(tcp_ptr->th_sport));
        default:
            return "";
    }
}

std::string sockets::tcp::destination_layer_(int type)
{
    switch (type - 1)
    {
        case LayerType::Physic:
        case LayerType::Network:
            return sockets::ip::destination_layer_(type);
        case LayerType::Transport:
            return utils::to_string(ntohs(tcp_ptr->th_dport));
        default:
            return "";
    }
}

std::string sockets::tcp::protocol_layer_(int type)
{
    switch (type - 1)
    {
        case LayerType::Physic:
        case LayerType::Network:
            return sockets::ip::protocol_layer_(type);
        case LayerType::Transport:
            return "TCP";
        default:
            return "";
    }
}

sockets::tcp::~tcp()
{
    tcp_ptr = NULL;
}
