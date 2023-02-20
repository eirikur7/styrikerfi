
int main(int argc, char **argv) {
	int blubb[16*4096];
	for(int j=0; j<10; j++) {
		for(int i=0; i<16*4; i++) {
			blubb[i*1024] = i;
		}
	}
}
