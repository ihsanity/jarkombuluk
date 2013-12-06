/* File : client.cpp
	Tujuan : Mengirimkan sembarang string ke server.cpp
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
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main()
{
	int sock;
	struct hostent *host;
	char send_data[1024];
	int addr_len, bytes_read;
	char recv_data[1024];
	struct sockaddr_in server_addr , client_addr;

	//ganti alamat servernya disini kalo tidak mau local
	char ServerAddress[] = "127.0.0.1"; 
	int ServerPort = 2013;

	host = (struct hostent *) gethostbyname((char *)ServerAddress);

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		/* Step 1 : Pembuatan socket Datagram (UDP),
		menghasilkan nilai -1 apabila terjadi error */
		cout<<"Error saat pembentukan socket"<<endl;
		exit(1);
	}

	server_addr.sin_family = AF_INET; //AF_INET, Family Address, coba cari dokumentasinya
	server_addr.sin_port = htons(ServerPort); //port server
	server_addr.sin_addr = *((struct in_addr *)host->h_addr); //variabel host diatas
	bzero(&(server_addr.sin_zero),8); //bzero, coba cari dokumentasinya

	//sampai disini, socket UDP client sudah dibuat dan siap pakai
	cout<<"Berhasil melakukan koneksi, tuliskan string apapun untuk dikirim"<<endl;

	//mengukur size dari struktur sockaddr
	addr_len = sizeof(struct sockaddr);
	cout<<"UDPServer menunggu client pada port "<<ServerPort<<endl; //sudah siap
	fflush(stdout); //ini menjadi wajib pada socket UNIX C, mengapa? coba aja hapus kalo berani :v

   while (1)
   {
    	cout<<"Tuliskan sesuatu lalu tekan enter (q / Q untuk keluar):";
    	cin>>send_data; //send_data merupakan array of char dengan buffer 1024 karakter

		//break apabila ada "q" atau "Q"
    	if ((strcmp(send_data , "q") == 0) || strcmp(send_data , "Q") == 0) break;
		else
			//sendto, method yang digunakan untuk mengirimkan paket UDP
			//berisikan variabel informasi paket, server tujuan yang telah didefinisikan diatas
    		sendto(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));

	//recvfrom merupakan metode menerima data dari sendTo client.cpp, menerima 1024 karakter buffer	
	bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
	recv_data[bytes_read] = '\0'; //karakter null

	//inet_ntoa konversi dari pengirim paket ke IP Address, nthos konversi dari pengirim paket ke Port
	cout<<"("<<(inet_ntoa(client_addr.sin_addr))<<","<<ntohs(client_addr.sin_port)<<") :"; //di print saja
	cout<<recv_data<<endl; //data yang diterima
	fflush(stdout); //ini menjadi wajib pada socket UNIX C, mengapa? coba aja hapus kalo berani :v
	bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
	
   }
	return 0;
}
