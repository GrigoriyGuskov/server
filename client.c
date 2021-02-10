#include "my_server.h"

#define DUMMY_STR "London is the capital of Great Britain"

int main() {
	
	int sk, ret;
	struct sockaddr_un name = {0};
	
	sk = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sk < 0) {
		perror("Unable to create socket");
		exit(1);
	}
	
	name.sun_family = AF_UNIX;
	strncpy(name.sun_path, PATH, sizeof(PATH));
	
	char buffer[BUFSZ] = {0};
	
	ret = read(STDIN_FILENO, buffer, BUFSZ-1);			//SIZE-1
		if (ret < 0 || ret >= BUFSZ) {
			printf("Unexpected read error or overflow %d\n", ret);
			exit(1);
		}
	
	ret = connect(sk, (struct sockaddr*)&name, sizeof(name));
	if (ret) {
		perror("Unable to connect to socket");
		exit(1);
	}
	
	ret = write(sk, buffer, BUFSZ-1);
	if (ret < 0) {
		perror("Unable to write");
	}
	if (strstr(buffer,"PRINT")){
		ret = read(STDIN_FILENO, buffer, BUFSZ-1);			//SIZE-1
		if (ret < 0 || ret >= BUFSZ) {
			printf("Unexpected read error or overflow %d\n", ret);
			exit(1);
		}
		ret = write(sk, buffer, BUFSZ-1);
		if (ret < 0) {
			perror("Unable to write");
		}
	}
	
	
	return 0;
}