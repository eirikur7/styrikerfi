#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCATION "./"

#define SECRETFILE (LOCATION "secret.txt")
#define SUCCESSFILE (LOCATION "success.txt")


// Check if the password matches the one in file "secret.txt"
int check_authentication(char *password) {
	char secret[9];

	// Read password from secret file
	FILE *f = fopen(SECRETFILE, "r");
	if(f==NULL) {
		perror("Could not open secret.txt");
		exit(1);
	}
	// read one 8-byte secret word
	int n = fread(secret, 8, 1, f);
	if(n!=1) {
		fprintf(stderr,"Could not read secret from secret.txt");
		exit(1);
	}
	secret[8]=0;
	fclose(f);
	// returning 1 for correct secret
	return strcmp(password, secret)==0 ? 1 : 0;
}

// Log message to file success.txt
void success(char *user, char *message) {
	FILE *f = fopen(SUCCESSFILE, "a");
	if(!f) { perror("Cannot append to success.txt"); exit(1); }
	fprintf(f, "User %s: Message %.32s\n", user, message);
	fclose(f);
}

int main(int argc, char **argv) {
	char username[32];
	char authenticated = 0;
	char message[32];

	printf("Some useless(?) debug info: %p %p %p\n", &username, &message, &authenticated);

	// Check if there are two parameters: secret password and answer message
	if(argc<3) {
		fprintf(stderr, "Usage: %s <secret> <answer>\n", argv[0]);
		exit(1);
	}
	
	// check if user sent me the right password
	if( check_authentication(argv[1]) ) { authenticated = 1; }

	// get the user name
	int res = getlogin_r(username, sizeof(username));
	if(res) { perror("failed to get user name"); exit(1); }

	// copy the answer message from the user
	for(int i=0; i<=strlen(argv[2]); i++) { message[i] = argv[2][i]; }

	printf("Some more useless debug info:  User %s [authenticated: %s]: message is %s\n", username, authenticated?"Yes":"No", message);
	
	if(authenticated) {
		success(username, message);
	}
	else {
		fprintf(stderr, "ERROR: User %s (with secret %s) not allowed to post message %s\n", username, argv[1], message);
		exit(1);
	}
	return 0;
}
