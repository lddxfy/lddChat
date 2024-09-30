#include "MsgNode.h"

RecvNode::RecvNode(short max_len, short msgid) : MsgNode(max_len),msgid_(msgid)
{
}

SendNode::SendNode(const char* data, short max_len, short msgid) : MsgNode(max_len + HEAD_TOTAL_LEN),msgid_(msgid)
{
	short msg_id_host = boost::asio::detail::socket_ops::host_to_network_short(msgid);
	memcpy(data_,&msg_id_host, HEAD_ID_LEN);

	short msg_data_len = boost::asio::detail::socket_ops::host_to_network_short(max_len);
	memcpy(data_ + HEAD_ID_LEN, &msg_data_len, HEAD_DATA_LEN);
	memcpy(data_ + HEAD_ID_LEN+ HEAD_DATA_LEN, data, max_len);
}
