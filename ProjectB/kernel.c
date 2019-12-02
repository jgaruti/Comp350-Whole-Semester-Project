//GiuseppeCarchidi, Federico Read, JoeyGaruti, Fall 2019

void printChar(char);
void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);

void main(){
//        printChar('x');
//        printString("Hello World");
	char line[80];
	char buffer[512];
	makeInterrupt21();
	printString("Enter a line: ");
	interrupt(0x21, 1, line, 0, 0);
	interrupt(0x21, 0, line, 0, 0);
	interrupt(0x21, 2, buffer, 30, 0);
	interrupt(0x21, 0, buffer, 0, 0);
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
        for(i=0; chars[i]!='\0'; i++);
	for(l=0; l<i; l++){
		printChar(chars[l]);
                //interrupt(0x10, 0xe*256+chars[l], 0, 0, 0);
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
	else{
		printString("Error: unauthorized access!");
	}
}