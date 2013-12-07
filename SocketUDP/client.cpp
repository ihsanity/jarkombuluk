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
#include <string>
#include <vector>
#include <sstream>
using namespace std;

vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

string hapusLineSeparator(string s){
	string s2 = "";
	for (int i=0;i<s.length();i++){
		if (s[i] == '\r' || s[i] == '\n')
			break;
		s2 = s2 + s[i]; 
	}
	return s2;
}

string convertGaris(string s){
	string s2 = "";
	for (int i=0;i<s.length();i++){
		if (s[i] == 'O'){
			s2 = s2 + ' ';	
		}
		else {
			s2 = s2 + s[i];
		}
	}
	return s2;
}

int main()
{
	bool halo = false;
	int sock;
	struct hostent *host;
	char send_data[1024];
	char exitkode[4];
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
    	cout<<">> ";
		string strr;    	
		//getline(cin,strr); //send_data merupakan array of char dengan buffer 1024 karakter
		//send_data=strr.c_str();
		gets(send_data);
		string sendDataStr = send_data;
		vector<string> v = split(send_data,' ');
		int width, height, framecount;
		if (v[0] != "GET" && v[0] != "RANDOM" && v[0] != "QUIT"){
			if (v[0] != "HALO" && v[0] != "LIST" && v[0] != "LEN" && halo) {
				// Send whatever request
				sendto(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
				
				// Get response of whatever request
				bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
				recv_data[bytes_read] = '\0'; //karakter null
				string response = recv_data;
				cout << response << endl;
				fflush(stdout);
				//Trailer Eater
				bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
				
				bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
				recv_data[bytes_read] = '\0'; //karakter null
				response = recv_data;
				cout << response << endl;
				fflush(stdout);
				//Trailer Eater
				bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
								
			}
			else {
				if (v[0] == "HALO" || halo) {
					halo = true;
					// Send whatever request
					sendto(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));

					// Get response of whatever request
					bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
					recv_data[bytes_read] = '\0'; //karakter null
					string response = recv_data;
					cout << response << endl;
					fflush(stdout);
					// Trailer Eater
					bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
				}
			}
		}
		else if (v[0] == "GET" && halo){
			// Send LEN request 
			string lenCommand = "LEN " + v[1];
			sendto(sock, lenCommand.c_str(), lenCommand.length(), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
			
			// Get response of LEN request
			bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
			recv_data[bytes_read] = '\0'; //karakter null
			string lenResponse = recv_data;
			bool start = true;
			int counter = 0;
			width = 0;
			height = 0;
			framecount = 0;
			for (int i=0;i<lenResponse.length();i++){
				if (lenResponse[i] >= '0' && lenResponse[i] <= '9'){
					start = false;
					if (counter == 0){
						width*=10;
						width+=(lenResponse[i]-'0');
					}
					else if (counter == 1){
						height*=10;
						height+=(lenResponse[i]-'0');
					}
					else if (counter == 2){
						framecount*=10;
						framecount+=(lenResponse[i]-'0');
					}
				}
				else {
					if (!start){
						counter++;
					}
					start = true;
				}
			}
			// Trailer Eater
			bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
			
			while (1) {
			// Send GET Request
			lenCommand = "GET " + v[1];
			sendto(sock, lenCommand.c_str(), lenCommand.length(), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
			system("clear");
			// Get response of GET Request
			
				for (int i=0;i<framecount;i++){
					for (int j=0;j<height;j++){
						bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
						recv_data[bytes_read] = '\0'; //karakter null
						string getResponse = recv_data;
						getResponse = hapusLineSeparator(getResponse);
						cout << convertGaris(getResponse) << endl;
						// Trailer Eater
						bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
					}
					fflush(stdout);
					usleep(500 * 1000);
					system("clear");
				}
						bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
						bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
				
			}
			// Trailer Eater
			bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
			bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
		}
		else if (v[0] == "RANDOM" && halo){
			while (1) {
			
			// Send RANDOM request
			sendto(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
			
			// Get METADATA
			bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
			recv_data[bytes_read] = '\0'; //karakter null
			string response = recv_data;
			bool start = true;
			int counter = 0;
			width = 0;
			height = 0;
			framecount = 0;
			for (int i=0;i<response.length();i++){
				if (response[i] >= '0' && response[i] <= '9'){
					start = false;
					if (counter == 0){
						width*=10;
						width+=(response[i]-'0');
					}
					else if (counter == 1){
						height*=10;
						height+=(response[i]-'0');
					}
					else if (counter == 2){
						framecount*=10;
						framecount+=(response[i]-'0');
					}
				}
				else {
					if (!start){
						counter++;
					}
					start = true;
				}
			}
			// Trailer Eater
			bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
			
			system("clear");
			// Get response of request
				for (int i=0;i<framecount;i++){
					for (int j=0;j<height;j++){
						bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
						recv_data[bytes_read] = '\0'; //karakter null
						string getResponse = recv_data;
						getResponse = hapusLineSeparator(getResponse);
						cout << convertGaris(getResponse) << endl;
						// Trailer Eater
						bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
					}
					fflush(stdout);
					usleep(500 * 1000);
					system("clear");
				}
				bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
				bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
			}
			// Trailer Eater
			bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
			bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
		}
		else if (v[0] == "QUIT" && halo){
			// Send quit request
			sendto(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
			
			// Get response
			bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
			recv_data[bytes_read] = '\0';
			string response = recv_data;
			cout << response << endl;
			sleep(1);
			//Trailer Eater
			bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
			break;
		}
		
	
   }
	return 0;
}
