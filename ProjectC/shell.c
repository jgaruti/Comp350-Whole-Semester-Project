void main(){
while(1){
	char command[80];
	char read[512];
	char file[6];
	int i;
	syscall(0,"shell>");
	syscall(1,command);
	if(command[0]=='t' && command[1]=='y' && command[2]=='p' && command[3]=='e' && command[4]==' '){
		for(i=0;i<6;i++){
			if(command[i+5]=='\0'){
				file[i]='\0';
				break;
			}
			else{
				file[i]=command[i+5];
			}
		}
		syscall(3,file,read,0);
		syscall(0,read);
	}
	else if(command[0]=='e' && command[1]=='x' && command[2]=='e' && command[3]=='c' && command[4]==' '){
		for(i=0;i<6;i++){
			if(command[i+5]=='\0'){
				file[i]='\0';
				break;
			}
			else{
				file[i]=command[i+5];
			}
		}
		syscall(4,file);
	}
	else{
		syscall(0,"command does not exist\r\n");
	}
}
}