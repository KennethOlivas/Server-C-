#include <iostream>
#include <string>
#include <sstream>
#include "mongoose.h"

#pragma comment (lib, "ws2_32.lib")

using namespace std;


// estructura que contendra las configuraciones del servidor http
static struct mg_serve_http_opts s_http_server_opts;


static void ev_handler(struct mg_connection* nc, int e, void *p) {
	// peticion http

	if (e == MG_EV_HTTP_REQUEST)
	{
		mg_serve_http(nc, (struct http_message *) p , s_http_server_opts);
	}
}

int initServer(int port) {
	//Eventos moongose
	struct mg_mgr mgr;

	struct mg_connection* nc;

	//covertir puerto en caracteres

	string portChar = to_string(port);
	static char const* sPort = portChar.c_str();
 
	//inicar server

	mg_mgr_init(&mgr, NULL);

	cout << "Iniciando servidor en el puerto: " << sPort << endl;

	nc = mg_bind(&mgr, sPort, ev_handler);

	if (nc == NULL)
	{
		cout << "Error al escuchar puerto" << endl;
		return 1;
	}


	// configurar http server
	mg_set_protocol_http_websocket(nc);

	s_http_server_opts.document_root = ".";

	s_http_server_opts.enable_directory_listing = "yes";

	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}

	//liberar memoria
	mg_mgr_free(&mgr);

	return 0;

}

int main(void)
{
	int port;
	cout << "Seleciona el puerto del sevidor" << endl;
	cin >> port;

	if (cin.fail())
	{
		port = 1000;

	}

	initServer(port);

	return 0;


}