#include "my_server.h"

int main() {
	int sk, ret;
	struct sockaddr_un name = {0};
	
	sk = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sk < 0) {
		perror("Unable to create socket");
		return 1;
	}
	
	name.sun_family = AF_UNIX;
	strncpy(name.sun_path, PATH, sizeof(PATH));
	
	ret = bind(sk, (struct sockaddr*)&name, sizeof(name));
	if (ret < 0) {
		perror("Unable to create socket");
		close(sk);
		return 1;
	}
	
	ret = listen(sk, 20);
	if (ret){
		perror("Unable to listen");
		close(sk);
		return 1;
	}
	
	while(1) {
		int client_sk;
		char buffer[BUFSZ] = {0};
		
		client_sk = accept (sk, NULL, NULL);
		if (client_sk < 0) {
			perror("Unable to accept");
			exit(1);
		}
		
		ret = read(client_sk, buffer, BUFSZ);
		if (ret < 0 || ret >= BUFSZ) {
			printf("Unexpected read error or overflow %d\n", ret);
			exit(1);
		}
		/*
		int j = 1; 
		for (int i = 0; i < BUFSZ && j; i++) {
			if (buffer[BUFSZ-1 - i] == '0' || isspace (buffer[BUFSZ-1 - i] ))
				buffer[BUFSZ-1 - i] = '\0';
			else
				j = 0;
		}
		*/
		if (strstr(buffer,"EXIT")) {
			close(client_sk);
			unlink (PATH);
			return 0;
		} else if (strstr(buffer,"PRINT")) {
			ret = read(client_sk, buffer, BUFSZ);
			if (ret < 0 || ret >= BUFSZ) {
				printf("Unexpected read error or overflow %d\n", ret);
				exit(1);
			}
			printf("%s", buffer);
			close(client_sk);
		} else {
			printf("Unknown comand: %s", buffer);
			close(client_sk);
		}
		
		
	}
	
	unlink (PATH);
	return 0;
}