int strcmp(char* a,char* b){
	int cmp_i = 0;
	while(a[cmp_i]!='\0' && a[cmp_i]!=' '){
		if(a[cmp_i]!=b[cmp_i]){ return 1;}
		cmp_i++;
	}
	if(b[cmp_i]!='\0' && b[cmp_i]!=(char)8 && b[cmp_i] != (char)32){return 1;}
	return 0;
}

