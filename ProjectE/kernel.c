//Giuseppe Carchidi, Federico Read, JoeyGaruti, Fall 2019


void printChar(char);
void printString(char*);
void readString(char*);
void readSector(char*, int);
void readFile(char*, char*, int*);
void executeProgram(char*);
void terminate();
void writeSector(char*,int);
void deleteFile(char*);
void writeFile(char* buffer, char* filename, int numberOfSectors);
void handleTimerInterrupt(int segment, int sp);
void handleInterrupt21(int, int, int, int);

int processActive[8];
int processStackPointer[8];
int currentProcess;

void main(){
    //printChar('x');
    //printString("Hello World");
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
	int i =0;
	for(i=0;i<8;i++){
		processActive[i]=0;
		processStackPointer[i]=0xff00;
	}
	currentProcess = -1;
	makeInterrupt21();
	//interrupt(0x21,8,"this is a test message\r\n","testmg",3);
	makeTimerInterrupt();
	interrupt(0x21, 4, "shell", 0, 0);
	while(1); 

}

void printChar(char character){
	char al = character;
	char ah = 0xE;
	int ax = ah*256+al;
        interrupt(0x10, ax, 0, 0, 0); //what does interrupt 0x10 do?
}

void printString(char* chars){
	int i;
	int l;
    for(i=0; chars[i]!='\0'; i++){
		// for(l=0; l<i; l++){
			// printChar(chars[l]);
			// //interrupt(0x10, 0xe*256+chars[l], 0, 0, 0);
		// }
		printChar(chars[i]);
	}
}

void readString(char* chars){
	char read=0;
	int i;
	//for(i=0; i<512; i++){chars[i]=' ';}
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
	interrupt(0x13, ah*256+al, buffer, ch*256+cl, dh*256+dl); //what does interrupt 0x13 do?
}

void readFile(char* filename, char* buffer, int* sector){
	char dir[512];
	int fileEntry;
	int i;
	int match;
	int fileFound = 0;
	int sectorNumber;
	int bufferAddress;
	//int matchLoop = 0;
	*sector = 0;
	readSector(dir,2);
	for(fileEntry=0; fileEntry<512; fileEntry+=32){	//loops through possible file locations in sector
		//printChar(filename[fileEntry/32]);
		match = 1;
		//matchLoop = 0;
		//printString("looped no?\r\n");
		for(i=0;i<6;i++){
			//printString("looped\r\n");
			if(filename[i]=='\0'&& dir[fileEntry+i]=='\0'){
				break;
			}
			else if(filename[i]!=dir[fileEntry+i]){
				match = 0;
				break;
			}
			// if(filename[i]==dir[fileEntry+i]){
				// matchLoop++;
			// }
			// if(matchloop!=0){
			// printchar(matchloop+'0');
			// printchar('\r');
			// printchar('\n');
			// }
		}
		if(match==1){
			//printString("looped found\r\n");
			//*sector = dir[fileEntry+6];
			//sectorNumber = dir[fileEntry+6+fileFound];
			//printChar('0');
			//printChar('\r');
			//printChar('\n');
			for(fileFound=0;fileFound<13312;fileFound++){
				buffer[fileFound]='\0';
			}
			for(fileFound=0;fileFound<26;fileFound++){
				//printChar(fileFound+'0');
				//printChar('\r');
				//printChar('\n');
				//printString("looped found 1\r\n");
				sectorNumber = dir[fileEntry+6+fileFound];
				//printChar(filename[fileFound]);
				// printChar(*sector+'0');
				// printChar('\r');
				// printChar('\n');
				bufferAddress = buffer+fileFound*512;
				if(sectorNumber!=0){
					//printString("\r\nlooped found 2\r\n");
					readSector(bufferAddress,sectorNumber);
					*sector = *sector + 1;	//changes sector
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

void writeSector(char* buffer, int sector){
	int ah=3;
	int al=1;
	int ch=0;
	int cl=sector+1;
	int dh=0;
	int dl=0x80;
	interrupt(0x13, ah*256+al, buffer, ch*256+cl, dh*256+dl);
}

void deleteFile(char* filename){
	char dir[512];
	char map[512];
	int fileEntry;
	int i;
	int match;
	int fileFound = 0;
	readSector(dir,2);
	readSector(map,2);
	for(fileEntry=0; fileEntry<512; fileEntry+=32){	//loops through possible file locations in sector
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
				break;
			}
		}
		if(match==1){ //file has been found
			dir[fileEntry]='\0';	//sets first byte of the file name in directory to 0	line 310	needs to be after for loop
			//map[fileEntry]='\0';
			//map[0]='\0';
			for(fileFound=6;fileFound<32;fileFound++){	//start at seventh byte (1st after file name section.)
									//do I even want to start here?
			//for(fileFound=0;fileFound<26;fileFound++){
				//want to set corresponding section of the map to 0 for each byte until I read \0
				//jump to map byte, set to 0, loop
				if(dir[fileEntry+fileFound]!=0){
					match=dir[fileEntry+fileFound];
					map[match]=0;
				}
				else{
					break;	//do I want to add fileEntry here?
//					map[fileFound]='\0';	//do I want to add fileEntry here?
				}
			}
			//dir[fileEntry]='\0';
			writeSector(dir,2);
			writeSector(map,1);
			break;
		}
//		writeSector(dir,2);
//		writeSector(map,1);	//not sure if this is the right place (probably not)
	}
}

void writeFile(char* buffer, char* filename, int numberOfSectors){
	char dir[512];
	char map[512];
	int i;
	int j;
	int sector;
	int fileEntry;
	int bufferAddress;
	readSector(dir,2);
	readSector(map,1);
	for(fileEntry=0; fileEntry<512; fileEntry+=32){
		if(dir[fileEntry]=='\0'){
			for(i=0;i<6;i++){
				if(filename[i]=='\0'){
					for(j=0;j<(6-i);j++){
						dir[fileEntry+i+j]='\0';
					}
					break;
				}
				else{
					dir[fileEntry+i]=filename[i];
				}
			}
			j=6;
			for(i=0;i<numberOfSectors;i++){
				for(sector=3;sector<512;sector++){ //kernel starts at 3, finds the first open sector
					if(map[sector]==0){
						map[sector]=255;
						break;
					}
				}
				dir[fileEntry+j] = sector;
				bufferAddress=buffer+(i*512);
				writeSector(bufferAddress,sector);
				sector++;
				j++;
			}
			for(j=j+1;j<32;j++){
				dir[fileEntry+j]=0;
			}
			writeSector(map,1);
			writeSector(dir,2);
			break;
		}
	}
}

void handleTimerInterrupt(int segment, int sp){
	//printChar('T');
	//printChar('i');
	//printChar('c');
	
	returnFromTimer(segment, sp);
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
	else if(AX==6){
		writeSector(BX,CX);
	}
	else if(AX==7){
		deleteFile(BX);
	}
	else if(AX==8){
		writeFile(BX, CX, DX);
	}
	else{
		printString("Error: unauthorized access!");
	}
}