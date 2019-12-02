void main()
{
	int startVidMem = 0xb800;
	int vidMemOffset = 0x0;
	int color = 0x7;
	char* letters = "hello world\0";
	
	while(*letters != 0x0){
		putInMemory(startVidMem,vidMemOffset,*letters);
		vidMemOffset++;
		putInMemory(startVidMem,vidMemOffset,color);
		vidMemOffset++;
		
		letters++;
	}
	
	while(1);
}