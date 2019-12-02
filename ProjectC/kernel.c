//GiuseppeCarchidi, Federico Read, JoeyGaruti, Fall 2019

void printChar(char);
void printString(char*);
void readString(char*);
void readSector(char*, int);
void readFile(char*, char*, int*);
void executeProgram(char*);
void terminate();
void handleInterrupt21(int, int, int, int);

void main(){
//        printChar('x');
//        printString("Hello World");
	// char line[80];
	// char buffer[512];
	// makeInterrupt21();
	// printString("Enter a line: ");
	// interrupt(0x21, 1, line, 0, 0);
	// interrupt(0x21, 0, line, 0, 0);
	// interrupt(0x21, 2, buffer, 30, 0);
	// interrupt(0x21, 0, buffer, 0, 0);
	// while(1);
	 // char buffer[13312];   /*this is the maximum size of a file*/
	 // int sectorsRead;
	 // makeInterrupt21();
	 // interrupt(0x21, 0, "System has Finished booting\r\n\n\n", 0, 0);  //finished boot message
	// interrupt(0x21, 3, "tstpr1", buffer, &sectorsRead);   /*read the file into buffer*/ 
	// //printChar(sectorsRead+'0');
	// //printChar('\r');
	// //printChar('\n');
	// //sectorsRead=0;
	// if (sectorsRead>0){
		// printChar(sectorsRead+'0');
		// printChar('\r');
		// printChar('\n');
		// interrupt(0x21, 0, buffer, 0, 0);   /*print out the file*/ 
	// }
	// else{
		// interrupt(0x21, 0, "messag not found\r\n", 0, 0);		/*no sectors read? then print an error*/
	// }
	// while(1);   
	makeInterrupt21();
	interrupt(0x21, 4, "shell", 0, 0);
	while(1); 

}

void printChar(char character){
	char al = character;
	char ah = 0xE;
	int ax = ah*256+al;
        interrupt(0x10, ax, 0, 0, 0);
}

void printString(char* chars){
	int i;
	int l;
    for(i=0; chars[i]!='\0'; i++);{
		for(l=0; l<i; l++){
			printChar(chars[l]);
			//interrupt(0x10, 0xe*256+chars[l], 0, 0, 0);
			}
	}
}

void readString(char* chars){
	char read=0;
	int i;
	for(i=0; i<80; i++){chars[i]=' ';}
	i=0;
	while(read != 0xd){
		read=0;
		read=interrupt(0x16, 0, 0, 0, 0);
		i++;
		if(read==0x8){
			if(i>1){
				i--;
				printChar(0x8);
				//chars[i-1] = ' ';
				printChar(' ');
				//chars[i] = '\0';
				if(i>0){
					printChar(0x8);
				}
			}
			if(i>0){
				i--;
			}
		}
		else if(read==0xd){
			printChar(read);
			printChar(0xa);
			chars[i-1]=read;
			chars[i]=0xa;
			i++;
			chars[i]=0x0;
			i++;
			chars[i]='\0';
		}
		else{
			chars[i-1]=read;
			printChar(chars[i-1]);
		}
	chars[i]='\0';
	}
}

void readSector(char* buffer, int sector){
	int ah=2;
	int al=1;
	int ch=0;
	int cl=sector+1;
	int dh=0;
	int dl=0x80;
	interrupt(0x13, ah*256+al, buffer, ch*256+cl, dh*256+dl);
}

void readFile(char* filename,char* buffer, int* sector){
	char dir[512];
	int fileEntry;
	int i;
	int match;
	int fileFound = 0;
	int sectorNumber;
	int bufferAddress;
	*sector = 0;
	readSector(dir,2);
	for(fileEntry=0; fileEntry<512; fileEntry+=32){
		//printChar(filename[fileEntry/32]);
		match = 1;
		//printString("looped no?\r\n");
		for(i=0;i<6;i++){
			//printString("looped\r\n");
			if(filename[i]=='\0'&& dir[fileEntry+i]=='\0'){
				break;
			}
			else if(filename[i]!=dir[fileEntry+i]){
				match = 0;
			}
		}
		if(match==1){
			//printString("looped found\r\n");
			//*sector = dir[fileEntry+6];
			//sectorNumber = dir[fileEntry+6+fileFound];
			//printChar(sectorNumber+'0');
			//printChar('\r');
			//printChar('\n');
			for(fileFound=0;fileFound<26;fileFound++){
				//printChar(fileFound+'0');
				//printChar('\r');
				//printChar('\n');
				//printString("looped found 1\r\n");
				sectorNumber = dir[fileEntry+6+fileFound];
				//printChar(filename[fileFound]);
				//printChar(sectorNumber+'0');
				bufferAddress = buffer+fileFound*512;
				if(sectorNumber!=0){
					//printString("\r\nlooped found 2\r\n");
					readSector(bufferAddress,sectorNumber);
					*sector = *sector + 1;
				}
				else{
					break;
				}
			}
			//printString("\r\nlooped found ending\r\n");
			break;
		}
	}
}

void executeProgram(char* name){
	char buffer[13312];   /*this is the maximum size of a file*/
	int sectors;
	int i;
	//for(i=0; i<13312; i++){buffer[i]='\0';}
	i=0;
	readFile(name, buffer, &sectors);
	if(sectors==0){return;}
	for(i=0;i<13312;i++){
		//if(buffer[i]=='\0'){
			//break;
		//}
		//printString("looped\r\n");
		putInMemory(0x2000,i,buffer[i]);
	 }
	//putInMemory(0x2000,i, buffer);
	//printString("starting program\r\n\n");
	launchProgram(0x2000);
}

void terminate(){
	char shell[6];
	shell[0]='s';
	shell[1]='h';
	shell[2]='e';
	shell[3]='l';
	shell[4]='l';
	shell[5]='\0';
	interrupt(0x21, 4, shell, 0, 0);
}

void handleInterrupt21 (int AX, int BX, int CX, int DX){
	if(AX==0){
		printString(BX);
	}
	else if(AX==1){
		readString(BX);
	}
	else if(AX==2){
		readSector(BX, CX);
	}
	else if(AX==3){
		readFile(BX, CX, DX);
	}
	else if(AX==4){
		executeProgram(BX);
	}
	else if(AX==5){
		terminate();
	}
	else{
		printString("Error: unauthorized access!");
	}
}
