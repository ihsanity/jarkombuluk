/* File : server.cpp
	Tujuan : Menerima sembarang string dari client.cpp
	Port : 5000
	Host : localhost
	Creator : Asistennya Baik :)
	Pesan : Kalo niat buat kelasnya sendiri (socket.cpp atau apalah)
	ntar kelompokkin masing2 fungsinya. biar gampang ngodingnya.
	Ini masih belum dikelompokin, kelompokin sendiri yah :v
*/

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main()
{
	int sock;
	int addr_len, bytes_read;
	char recv_data[1024];
	struct sockaddr_in server_addr , client_addr;

	int ServerPort = 2013;

	//Pembentukan socket UDP server
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
  		cout<<"Error saat pembentukan socket"<<endl;
   	exit(1);
	}

	//sama saja seperti client.cpp
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(ServerPort);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8);


	//Binding port, server akan mengecek apakah port tersebut bisa dipakai
	if (bind(sock,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1)
	{
   	cout<<"Error saat melakukan binding port, mungkin sudah dipakai"<<endl;
   	exit(1);
	}

	//mengukur size dari struktur sockaddr
	addr_len = sizeof(struct sockaddr);
	cout<<"UDPServer menunggu client pada port "<<ServerPort<<endl; //sudah siap
	fflush(stdout); //ini menjadi wajib pada socket UNIX C, mengapa? coba aja hapus kalo berani :v

	while (1)
	{
		//recvfrom merupakan metode menerima data dari sendTo client.cpp, menerima 1024 karakter buffer	
		bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
		recv_data[bytes_read] = '\0'; //karakter null

		//inet_ntoa konversi dari pengirim paket ke IP Address, nthos konversi dari pengirim paket ke Port
		cout<<"("<<(inet_ntoa(client_addr.sin_addr))<<","<<ntohs(client_addr.sin_port)<<") :"; //di print saja
 		cout<<recv_data<<endl; //data yang diterima
		fflush(stdout); //ini menjadi wajib pada socket UNIX C, mengapa? coba aja hapus kalo berani :v
	}
	return 0;
}
