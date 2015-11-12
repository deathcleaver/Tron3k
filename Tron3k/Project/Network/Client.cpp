#include "Client.h"

Client::~Client()
{
	
}

void Client::init()
{
	multipacket = 0;

	isClient = true;

	firstPackageSent = false;
	packageRecieved = false;
	mapLoaded = false;
	joined = false;

	conID = -1;

	con = new Connection();
	con->init();
}

void Client::network_IN(float dt)
{
	IN(con, conID);
}

void Client::network_OUT(float dt)
{
	if (msg_out != "")
	{
		Packet* out;
		out = new Packet();
		*out << Uint8(NET_INDEX::MESSAGE) << conID << scope_out << msg_out;
			con->send(out);
		delete out;

		msg_out = "";
	}
}

bool Client::new_connection()
{
	// test connect
	if (con->isConnected() == false)
	{
		if (con->connect(IpAddress::LocalHost, PORT))
		{
			return true;
		}
		return false; //failed
	}
	return true; //??? already connected
}

void Client::new_connection_packet()
{
	Packet* out;
	out = new Packet();
	*out << Uint8(NET_INDEX::NEW_CONNECTION);
	con->send(out);
	delete out;
}

void Client::in_new_connection(Packet* rec, Uint8 _conID)
{
	packageRecieved = true;

	string map;
	*rec >> conID;
	Uint32 size;
	*rec >> map;
	
	printf("My connection ID : %d \n", conID);
	//can i load this map?
	printf("Map in use : %s \n", map.c_str());

}

void Client::in_event(Packet* rec, Uint8 _conID)
{

}

void Client::in_frame(Packet* rec, Uint8 conID)
{

}

void Client::in_message(Packet* rec, Uint8 conID)
{
	*rec >> conID_in;
	*rec >> scope_in;
	*rec >> msg_in;
}