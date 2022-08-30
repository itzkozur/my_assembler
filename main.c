
#include "main.h"

int addres_num = 100;

int reg_is_legal(int reg)
{
    if (reg > -1 && reg < 32)
    {
        return 1;
    }
    else
        return 0;
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int j = 0;
    int c_argv = 1;
    int flag_running = 1;
    int line_count = 0;
    char str[MAX_LINE];
    char nameFile[MAX_LABEL] = {'\0'};
    t_ptr mainTableHeader = NULL;
    ptr symbolTableHeader = NULL;
    int imageCommend = 0;
    int imageData = 0;
    int errorExcist = 0;
    int abortFile = 0;

    if (argc < 2)
    { /*check if there is any input file*/
        printf("EROR: THE PROGRAM NEED A SOURCE FILE '.AS'");
        exit(0);
    }

    while (c_argv < argc)
    { /*goes over the whole arguments*/
        while (argv[c_argv][j] != '\0')
        {
            if (j != 0 && argv[c_argv][j] == '.' && argv[c_argv][j + 1] == 'a' && argv[c_argv][j + 2] == 's' && argv[c_argv][j + 3] == '\0')
            { /*check if the input file with extention */
                fp = fopen(argv[c_argv], "r+");
                if (!(feof(fp)))
                {
                    while (flag_running < 3)
                    {
                        while (!(feof(fp)))
                        {
                            if (fgets(str, 81, fp) != NULL)
                            {  
                                validLenLine(str);
                                errorExcist = what_command(str, line_count, flag_running, &mainTableHeader, &symbolTableHeader );
                                if(errorExcist ==1){
                                    abortFile = 1;
                                }
                                line_count++;
                            }
                        }
                        if (flag_running == 1)
                        {
                            imageCommend = (addres_num)-100;
                        }
                        fseek(fp, 0, SEEK_SET);
                        line_count = 0;
                        flag_running++;
                    }
                    imageData = (addres_num)-imageCommend - 100;
                    fseek(fp, 0, SEEK_SET);
                    line_count = 0;
                    while (!(feof(fp)))
                    {
                        if (fgets(str, 81, fp) != NULL)
                        {
                            errorExcist = run2(str, line_count, symbolTableHeader, mainTableHeader);
                            if(errorExcist ==1){
                                abortFile = 1;
                            }
                            line_count++;
                        }
                    }
                    
                    printTableList(mainTableHeader);
                    
                    fclose(fp);
                    printExtern( nameFile ,symbolTableHeader , mainTableHeader);
                    compliteTable(symbolTableHeader, mainTableHeader);

                    printTableList(mainTableHeader);
                    printSymbolList(symbolTableHeader);
                    
                    /*if(abortFile==0){*/
                        printbinary2Hexadecimal(mainTableHeader, nameFile, imageCommend, imageData);
                        printEntry(symbolTableHeader ,nameFile);
                
                    
                    c_argv++;
                    break;

                    freeTableList(&mainTableHeader);
                    freeSymbolList(&symbolTableHeader);
                }
                else
                {
                    printf("cannot open file");
                }
            }
            nameFile[j] = argv[c_argv][j];
            j++;
        }
        /*check if the file with out extention
        if(argv[c_argv][j]!='\0'){
            printf("EROR:  with out extention '.as'");
        }*/
        c_argv++;
    }
    return 0;
} /*end of main*/


int validLenLine(char *line){
    if(strlen(line)>80){
        printf("\n%s",strlen(line));
        return 1;
    }
    else return 0;
}

/*int validFileName(char *namFile){
    int i = o;
    while(namFile[i] != '\0'){
        if(isalpha(namFile[i]){
            continue;
        }
        if (i != 0 && namFile[i] == '.' &&  namFile[i+1] == 'a' &&  namFile[i+2] == 's' &&  namFile[i+3] == '\0'){
            return 0;
        }
        else return 1;
    }
    
}*/

int run2(char file[], int line_count, ptr symbolTableHeader, t_ptr mainTableHeader)
{
    char data[MAX_LABEL] = {'\0'};
    int arg_counter = 0;
    int i = 0;
    int j = 0;
    int sighn = 0;
    char arg3_lable[MAX_LABEL] = {'\0'};
    while (file[i] != '\0')
    {
        if (file[i++] == '.')
        {
            while (file[i] != '\0')
            {
                if (strcmp(data, "asciz") == 0 || strcmp(data, "db") == 0 || strcmp(data, "dh") == 0 || strcmp(data, "dw") == 0)
                {
                    return 0;
                }
                if ((strcmp(data, "entry") == 0 || strcmp(data, "extern") == 0) && isspace(file[i]))
                {
                    while (file[i] != '\0')
                    {
                        if (isalpha(file[i]) && sighn == 0)
                        {
                            while (file[0] != '\0' && (isalpha(file[i]) || isdigit(file[i])))
                            {
                                arg3_lable[arg_counter++] = file[i++];
                            }
                            arg3_lable[arg_counter] = '\0';
                            sighn = 4;
                        }
                        else if (isspace(file[i]))
                        {
                            i++;
                            continue;
                        }
                        else
                        {
                            printf("Line %d: is not a commanddd\n", line_count);
                            return 1;
                        }
                    }
                    data[j] = '\0';
                }
                else if (isalpha(file[i]))
                {
                    while (file[0] != '\0' && isalpha(file[i]))
                    {
                        data[j++] = file[i++];
                    }
                    continue;
                }
                else
                {
                    printf("Line %d: is not a command\n", line_count);
                    return 1;
                }
            }
        }
        else if (isspace(file[i]))
        {
            i++;
            continue;
        }
        else
            return 0;
    }
    updateSymbol_Table(arg3_lable, data, line_count, symbolTableHeader);
    return 0;
} /*end run2*/

int updateSymbol_Table(char *lable, char *type, int line_count, ptr symbol_TableHeader)
{
    int flagFound = isSymbolExists(symbol_TableHeader, lable);
    symbol newSymbol;
    if (flagFound == 0)
    {
        addAttributeToSymbol(symbol_TableHeader, lable, type);
    }
    else if (flagFound == 1 && strcmp(type, "extern") == 0)
    {
        strcpy(newSymbol.name, lable);
        strcpy(newSymbol.attributes[0], type);
        newSymbol.value = 0;
        addToSymbolList(&symbol_TableHeader, newSymbol);
    }
    else
    {
        printf("Line %d: label no exists \n", line_count);
        return 1;
    }
    return 0;
}

int printExtern( char nameFile[] , ptr symbolTable , t_ptr mainTable)
{
    FILE *p2f;
    t_ptr p;
    /*int i = 0;*/
    char exNameFile[MAX_LABEL];
    strcpy(exNameFile,nameFile);
    p2f = fopen(strcat(exNameFile, ".ext"), "w+");
    
    while(symbolTable)
	{
        p = mainTable;
    
		if(strcmp(symbolTable->data.attributes[0],"extern")==0)
		{
			while(p)
			{
				if(strcmp(p->data.label, symbolTable->data.name)==0)
					fprintf(p2f ,"%s 0%d\n",(symbolTable->data.name),p->data.address );
				p = p->next;
			}
		}
		symbolTable = symbolTable->next;
	}

    fclose(p2f);
    return 0;    
}

void createExtFile(t_ptr mainTable, ptr symbolTable)
{
	t_ptr p;
	while(symbolTable)
	{
		p = mainTable;
		if(strcmp(symbolTable->data.attributes[0],"extern")==0)
		{
			while(p)
			{
				if(strcmp(p->data.label, symbolTable->data.name)==0)
					printf("%s 0%d\n",(symbolTable->data.name),p->data.address );
				p = p->next;
			}
		}
		symbolTable = symbolTable->next;
	}
}


int printEntry(ptr symbolTableHeader, char nameFile[])
{
    FILE *p2f;
    char entNameFile[30];
    strcpy(entNameFile,nameFile);
    p2f = fopen(strcat(entNameFile, ".ent"), "w+");

    while (symbolTableHeader)
    {
        if (strcmp(symbolTableHeader->data.attributes[0], "entry")==0)
        {
            fprintf(p2f, "%s 0%d\n", symbolTableHeader->data.name, symbolTableHeader->data.value);  
        }
        else if (strcmp(symbolTableHeader->data.attributes[1], "entry")==0)
        {
            fprintf(p2f, "%s 0%d\n", symbolTableHeader->data.name, symbolTableHeader->data.value);   
        }
        symbolTableHeader = symbolTableHeader->next;
    }
    fclose(p2f);
    return 0;    
}

int printbinary2Hexadecimal(t_ptr tableHeader, char nameFile[], int dataCommend, int dataImage)
{
    FILE *p2f;
    long int first, second;
    char subbuff[5] = {'\0'};
    int byteCounter;
    int countCouple = 0;
    char *eptr;
    int lenCode = 0;
    int address = 100;
    char tempNameFile[MAX_LABEL];
    strcpy(tempNameFile,nameFile);
    p2f = fopen(strcat(tempNameFile, ".ob"), "w+");
    fprintf(p2f, "     %d %d\n", dataCommend, dataImage);
    while (tableHeader)
    {
        lenCode = strlen(tableHeader->data.machineCode);
        byteCounter = lenCode - 4;

        for (byteCounter; byteCounter > 0; byteCounter -= 4)
        {
            if (countCouple == 0)
            {
                fprintf(p2f, "0%d ", address);
                address += 4;
            }
            memcpy(subbuff, &tableHeader->data.machineCode[byteCounter], 4); /* <--- copy 4 byts only*/
            subbuff[4] = '\0';
            second = strtol(subbuff, &eptr, 2);
            byteCounter -= 4;
            memcpy(subbuff, &tableHeader->data.machineCode[byteCounter], 4); /* <--- copy 4 byts only*/
            subbuff[4] = '\0';
            first = strtol(subbuff, &eptr, 2);
            fprintf(p2f, "%X%X ", first, second);
            countCouple++;

            if (countCouple == 4)
            {
                fprintf(p2f, "\n");
                countCouple = 0;
            }
        }
        tableHeader = tableHeader->next;
    }
    fclose(p2f);
    return 0;
}

int what_command(char file[], int line_count, int flag_run2, t_ptr *mainTableHeader, ptr *symbolTableHeader )
{
    rowTable r;
    symbol s;
    int i = 0;
    int t = 0;
    int arg_counter = 0;
    int sighn = 0;
    int sighn_immed = 0;
    int numOfArg = 0;
    char data[MAX_LABEL] = {'\0'};
    char arg3_lable[MAX_LABEL] = {'\0'};
    char *machinCode ;
    int num_com[3] = {'\0'};
    int flag_symbol = 0; /*if there is symbol is 1 else is 0 */
    int flag_data = 0;   /*if is data commend is 1 else is 0 */
    int flag_guide = 0;  /*if is guide commend is 1 else is 0 */
    int islable = -1;
    int op_code = -1;
    int min_value;
    int max_value;
    strcpy(s.name, "");
    strcpy(r.label, "");
    /*      value sighn meaning:
0 - we are in the first letter/number and looking for $/letter
1 - we are in a register and looking for legal num after '$'
2 - we found num register legal and looking for ',' 
3 - we found a letter and we'r in middle a lable
4 - we found num register legal/lable and new just space is legal /is stop

        value sighn_immed meaning:
0 - we not found a value immed
1 - we found one value immed

        value islable meaning:
-1 - we not found a lable
1 - we have a lable (need to check if his legal)*/
    while (file[i] != '\0')
    {
        if (isalpha(file[i]) || file[i] == '.')
        {

            while (file[i] != '\0' || file[i - 1] == 'p')
            { 
                if (file[i] == ':' && flag_symbol == 0)
                {
                    if (t > 31)
                    { /*max of lable is 31 char*/
                        printf("Line %d: wrong index cooooommand\n", line_count);
                        return 1;
                    }
                    data[t + 1] = '\0';
                    strcpy(s.name, data);
                    t = 0;
                    i++;
                    flag_symbol = 1;
                    printf("\n%s\n", data);
                    break;
                }
                else if (isspace(file[i]) || file[i] == '\0')
                {
                    data[t] = '\0';
                    t = 0;
                    /*if the type commend is R--------------------------*/
                    if ((getTypeByName(data)) == 'R' && flag_run2 == 1)
                    {
                        printf("\nit's R command: %s\n", data);
                        numOfArg = (getOpcodeByName(data) == 0) ? 3 : 2;
                        i++;
                        while (file[i] != '\0' && arg_counter < numOfArg + 1)
                        {
                            if (file[i] == '$' && sighn == 0)
                            {
                                i++;
                                sighn = 1;
                                continue;
                            }
                            if (isdigit(file[i]) && sighn == 1)
                            {
                                num_com[arg_counter] = file[i++] - '0';
                                while (isdigit((int)file[i]))
                                {
                                    num_com[arg_counter] *= 10;
                                    num_com[arg_counter] += file[i++] - '0';
                                }
                                if (reg_is_legal(num_com[arg_counter]) == 0)
                                {
                                    printf("Line %d: wrong index command\n", line_count);
                                    return 1;
                                }
                                arg_counter++;
                                sighn = 2;
                                continue;
                            }
                            if (file[i] == ',' && sighn == 2)
                            {
                                i++;
                                sighn = 0;
                                continue;
                            }
                            if (isspace((int)file[i]))
                            {
                                i++;
                                continue;
                            }
                            else
                                printf("Line %d: wrong index command\n", line_count);
                            return 1;
                        }

                        if (arg_counter != numOfArg)
                        {
                            printf("Line %d: wrong index command\n", line_count);
                            return 1;
                        }

                        if (numOfArg == 3)
                        {
                            machinCode = getBinCode(r.address, data, numOfArg, islable, num_com[0], num_com[1], num_com[2]);
                            strcpy(r.machineCode, machinCode );
                            /*free(machinCode);*/
                        }
                        else
                        {
                            machinCode = getBinCode(r.address, data, numOfArg, islable, num_com[0], num_com[1]);
                            strcpy(r.machineCode, machinCode);
                            /*free(machinCode);*/
                        }
                    }
                    else /*if the type commend is I --------------------------*/
                        if (getTypeByName(data) == 'I' && flag_run2 == 1)
                    {
                        printf("\nit's I command: %s\n", data);
                        numOfArg = 3;
                        i++;
                        op_code = getOpcodeByName(data);

                        while (file[i] != '\0')
                        {
                            if (isalpha(file[i]) && arg_counter == 2 && (op_code > 14 && op_code < 19) && sighn == 0)
                            { /*for arg 3 - string*/
                                while (isalpha(file[i]) || isdigit(file[i]))
                                {
                                    arg3_lable[t++] = file[i++];
                                }
                                arg3_lable[t] = '\0';
                                islable = 1;
                                sighn = 4;
                                strcpy(r.label, arg3_lable);
                                continue;
                            }

                            if (file[i] == '$' && sighn == 0)
                            {
                                i++;
                                sighn = 1;
                                continue;
                            }

                            if ((isdigit(file[i]) || file[i] == '-' || file[i] == '+') && (sighn == 1 || sighn_immed == 0))
                            {
                                if (sighn == 0)
                                { /*for arg immed*/
                                    sighn_immed = 1;
                                }

                                if ((file[i] == '-' || file[i] == '+') && (!(isdigit(file[i + 1]))))
                                { /*if is- whith out digit after +/-*/
                                    printf("Line %d: wrong index ", line_count);
                                    return 1;
                                }
                                else if (file[i] == '-')
                                { /*if immed is minus*/
                                    i++;
                                    num_com[arg_counter] = file[i++] - '0';
                                    num_com[arg_counter] *= -1;

                                    while (isdigit((int)file[i]))
                                    {
                                        num_com[arg_counter] *= 10;
                                        num_com[arg_counter] -= file[i++] - '0';
                                    }
                                }
                                else if (file[i] == '+')
                                { /*if immed is pluse*/
                                    i++;
                                    num_com[arg_counter] = file[i++] - '0';

                                    while (isdigit((int)file[i]))
                                    {
                                        num_com[arg_counter] *= 10;
                                        num_com[arg_counter] += file[i++] - '0';
                                    }
                                }
                                else
                                {
                                    num_com[arg_counter] = file[i++] - '0';

                                    while (isdigit((int)file[i]))
                                    {
                                        num_com[arg_counter] *= 10;
                                        num_com[arg_counter] += file[i++] - '0';
                                    }
                                }
                                if (sighn_immed == 0 && reg_is_legal(num_com[arg_counter]) == 0)
                                {
                                    printf("Line %d: wrong index command\n", line_count);
                                    return 1;
                                }
                                arg_counter++;
                                sighn = 2;
                                continue;
                            }

                            if (file[i] == ',' && sighn == 2)
                            {
                                i++;
                                sighn = 0;
                                continue;
                            }
                            if (isspace((int)file[i]))
                            {
                                i++;
                                continue;
                            }
                            else
                            {
                                printf("Line %d: wrong index commanded\n", line_count);
                                return 1;
                            }
                        }

                        if (sighn_immed == 0 && (op_code > 9 && op_code < 15 || op_code > 18 && op_code < 25))
                        { /*for arg immed*/
                            printf("Line %d: wrong index command\n", line_count);
                            return 1;
                        }
                        strcpy(r.commend, data);
                        machinCode = getBinCode(r.address, data, numOfArg, islable, num_com[0], num_com[1], num_com[2]);
                        strcpy(r.machineCode, machinCode );
                        /*free(machinCode);*/
                    }
                    else
                        /*if the type commend is J --------------------------*/
                        if (getTypeByName(data) == 'J' && flag_run2 == 1)
                    {
                        op_code = getOpcodeByName(data);
                        sighn = (op_code == 63) ? 4 : 0; /*for stop command that his without arg*/
                        numOfArg = 1;
                        i++;
                        t = 0;
                        while (file[i] != '\0')
                        {
                            if (sighn == 0 && isalpha((int)file[i]))
                            {
                                while (file[i] != '\0' && (isalpha((int)file[i]) || isdigit(file[i])))
                                {
                                    arg3_lable[t++] = file[i++];
                                }
                                arg3_lable[t] = '\0';
                                strcpy(r.label, arg3_lable);
                                islable = 1;
                                sighn = 4;
                            }

                            if (file[i] == '$' && sighn == 0)
                            {
                                i++;
                                sighn = 1;
                                continue;
                            }

                            if (isdigit(file[i]) && sighn == 1)
                            {
                                num_com[arg_counter] = file[i++] - '0';
                                while (isdigit((int)file[i]))
                                {
                                    num_com[arg_counter] *= 10;
                                    num_com[arg_counter] += file[i++] - '0';
                                }
                                if (op_code == 32 && num_com[arg_counter] != 0)
                                {
                                    printf("Line %d: wwwrong index command\n", line_count);
                                    return 1;
                                }
                                if (reg_is_legal(num_com[arg_counter]) == 0)
                                {
                                    printf("Line %d: wrong index cooommand\n", line_count);
                                    return 1;
                                }
                                arg_counter++;
                                sighn = 4;
                                continue;
                            }
                            if ((isspace((int)file[i])) && (sighn == 0 || sighn == 4))
                            {
                                i++;
                                continue;
                            }
                            if (file[i] == '\0')
                            {
                                break;
                            }
                            else
                            {
                                printf("Line %d: wrong index cccommand\n", line_count);
                                return 1;
                            }
                        }

                        if (islable == 1)
                        {
                            machinCode =  getBinCode(r.address, data, numOfArg, islable, arg3_lable);
                            strcpy(r.machineCode,machinCode );
                            strcpy(r.commend, data);
                            /*free(machinCode);*/
                        }
                        else
                            machinCode =  getBinCode(r.address, data, numOfArg, islable, num_com[0]);
                            strcpy(r.machineCode,machinCode);
                            /*free((void *)machinCode);*/
                    }
                    else if ((strcmp(data, ".dw") == 0 || strcmp(data, ".db") == 0 || strcmp(data, ".dh") == 0) && flag_run2 == 2)
                    { /*is Guidance sentences*/
                        printf("it is hanchaya : %s\n", data);
                        i++;
                        while (file[i] != '\0')
                        {
                            if ((isdigit(file[i]) || file[i] == '-' || file[i] == '+') && sighn == 0)
                            {
                                if ((file[i] == '-' || file[i] == '+') && (!(isdigit(file[i + 1]))))
                                { /*if is- whith out digit after +/-*/
                                    printf("Line %d: invalid arguments for the command\n ", line_count);
                                    return 1;
                                }
                                else if (file[i] == '-')
                                {
                                    i++;
                                    num_com[arg_counter] = file[i++] - '0';
                                    num_com[arg_counter] *= -1;

                                    while (isdigit((int)file[i]))
                                    {
                                        num_com[arg_counter] *= 10;
                                        num_com[arg_counter] -= file[i++] - '0';
                                    }
                                }
                                else if (file[i] == '+')
                                {
                                    i++;
                                    num_com[arg_counter] = file[i++] - '0';

                                    while (isdigit((int)file[i]))
                                    {
                                        num_com[arg_counter] *= 10;
                                        num_com[arg_counter] += file[i++] - '0';
                                    }
                                }
                                else
                                {
                                    num_com[arg_counter] = file[i++] - '0';

                                    while (isdigit((int)file[i]))
                                    {
                                        num_com[arg_counter] *= 10;
                                        num_com[arg_counter] += file[i++] - '0';
                                    }
                                }
                                arg_counter++;
                                sighn = 2;
                                continue;
                            }
                            if (file[i] == ',' && sighn == 2)
                            {
                                i++;
                                sighn = 0;
                                continue;
                            }
                            if (isspace((int)file[i]))
                            {
                                i++;
                                continue;
                            }
                            else
                            {
                                printf("Line %d: invalid arguments for the command\n", line_count);
                                return 1;
                            }
                        }
                        flag_data = 1;
                        addNumbersGuidanceToTables(symbolTableHeader, mainTableHeader, file, data, s.name, arg_counter, num_com);
                    }
                    else if (strcmp(data, ".asciz") == 0 && flag_run2 == 2)
                    {
                        while (file[i] != '\0')
                        {
                            if (file[i] == '"' && sighn == 0)
                            {
                                i++;
                                sighn = 3;
                            }
                            else if (file[i] == '"' && sighn == 3)
                            {
                                i++;
                                sighn = 4;
                            }
                            else if ((isalpha(file[i]) || isspace(file[i]) || isprint(file[i])) && sighn == 3)
                            {
                                arg3_lable[arg_counter++] = file[i++];
                            }
                            else if (isspace(file[i]))
                            {
                                i++;
                                continue;
                            }
                            else
                            {
                                printf("Line %d: invalid arguments for the command\n", line_count);
                                return 1;
                            }
                        }
                        arg3_lable[arg_counter] = '\0';
                        flag_data = 1;
                        addGuidanceToTables(symbolTableHeader, mainTableHeader, addres_num, file, data, s.name, arg_counter + 1, arg3_lable);
                    }

                    else if (strcmp(data, ".extern") == 0 || strcmp(data, ".entry") == 0)
                    {
                        return 0;
                    }
                    else if ((strcmp(data, ".asciz") == 0 || strcmp(data, ".dw") == 0 || strcmp(data, ".db") == 0 || strcmp(data, ".dh") == 0) && flag_run2 == 1)
                    {
                        return 0;
                    }
                    else if (isCommend(data) == 0 && flag_run2 == 2)
                    {
                        return 0;
                    }
                    else
                    {
                        printf("Line %d: invalid command or label\n", line_count);
                        return 1;
                    }
                }
                /*else if(file[i]=='.'){
                            return 0;
                    }*/
                else
                {
                    data[t++] = file[i++];
                }
            }
        }
        else if (file[i] == ';' || file[i] == '.')
        { /*is a note or data for run2*/
            return 0;
        }
        else if (isspace(file[i++]))
        {
            while (file[i] != '\0' && isspace(file[i]))
            {
                i++;
            }
            if (file[i] == '\0')
            {
                return 0;
            }
            continue;
        }
        else if (isCommend(data) == 0 && flag_run2 == 2)
        {
            return 0;
        }
        else
        {
            i++;
        }
    } /*end of while*/

    r.address = addres_num;

    if (flag_symbol == 1 && flag_data == 0)
    {
        s.value = addres_num;
        if (flag_data == 1)
        {
            strcpy(s.attributes[0], "data");
        }
        else
        {
            strcpy(s.attributes[0], "code");
        }
        addToSymbolList(symbolTableHeader, s);
    }

    if (flag_data == 0)
    {
        strcpy(r.sourceCode, file);
        addToTablelList(mainTableHeader, r);
        addres_num += 4;
    }

    printf("\n%s",r.label);
    printf("bincode     = %s\n", r.machineCode);
    printf("source     = %s\n", r.sourceCode);
    printf("address     = %d\n", r.address);
    printf("arg lable    = %s\n", arg3_lable);
    printf("number    = %d %d %d %d\n", num_com[0], num_com[1], num_com[2], num_com[3]);

    return 0;
} /*end of what_command*/

void addGuidanceToTables(ptr *symbolTable, t_ptr *mainTable, int address, char *source, char *commend, char *label, int howMuchArgs, ...)
{
    int isAsciz = (strcmp(commend, ".asciz") == 0) ? 1 : 0;
    va_list p;
    rowTable row;
    va_start(p, howMuchArgs);
    int i, j;
    char **res;
    char *n = va_arg(p, char *);
    res = getBinCodesForGuidanceAsciz(n);
    /* add the first line to the main table*/
    strcpy(row.sourceCode, source);
    strcpy(row.commend, commend);
    strcpy(row.label, "");
    strcpy(row.machineCode, res[0]);
    row.address = address;
    addToTablelList(mainTable, row);

    /* Add the first line to symbol table*/
    if ((strcmp(label, "")) != 0)
    {
        symbol sym;
        strcpy(sym.name, label);
        sym.value = address;
        strcpy((char *)sym.attributes[0], "data");
        sym.attrNum = 1;
        addToSymbolList(symbolTable, sym);
    }

    for (i = 1; i < howMuchArgs; i++)
    {
        rowTable row;
        strcpy(row.sourceCode, "");
        strcpy(row.commend, "");
        strcpy(row.label, "");
        strcpy(row.machineCode, res[i]);
        row.address = address + i;
        addToTablelList(mainTable, row);
    }
    addres_num = address + i;
	for (i=0; i<howMuchArgs; i++) {
        free((void *)res[i]);
    }
    free(res);

}

void compliteTable(ptr symbolTable, t_ptr mainTable)
{
    int i;
    t_ptr p;
    int opCode;
    int labelAddress;
    int currAddress;
    char *machineCode;
    int mcLength;
    
    while (mainTable)
    {
        if (strcmp(mainTable->data.label, "") != 0)
        {
            opCode = getOpcodeByName((mainTable)->data.commend);
            labelAddress = getValueBySmboleName(symbolTable, (mainTable)->data.label);
            currAddress = (mainTable)->data.address;
            machineCode = (mainTable)->data.machineCode;

            mcLength = strlen(machineCode);
            if (opCode > 14 && opCode < 19)
            {
                machineCode[strlen(machineCode) - 1] = '\0';
                mcLength = mcLength -1;
                strcpy(((mainTable)->data.machineCode), strcat(machineCode, presentNumberAsBinary(32 - mcLength, labelAddress - currAddress)));
            }
            if ((opCode > 29 && opCode < 33) || opCode == 63)
            {
                
                machineCode[mcLength - 1] = '\0';
                if (hasExternAttribute(symbolTable, mainTable->data.label) == 1)
                {
                    strcpy((mainTable)->data.machineCode, strcat(machineCode, presentNumberAsBinary(32 - mcLength, 0)));
                }
                else
                {
                    strcpy((mainTable)->data.machineCode, strcat(machineCode, presentNumberAsBinary(32 - mcLength, labelAddress)));
                }
            }
        }
        mainTable = mainTable->next;
    }
}

void addNumbersGuidanceToTables(ptr *symbolTable, t_ptr *mainTable, char *source, char *commend, char *label, int counter, int *numbers)
{
    char *res;
    res = presentNumberAsBinary(getSizeBytsByGuidance(commend), numbers[0]);
    int size = getSizeBytsByGuidance(commend) / 8;
    int i;
    /* add the first line to the main table*/
    rowTable row;
    strcpy(row.sourceCode, source);
    strcpy(row.commend, commend);
    strcpy(row.label, "");
    strcpy(row.machineCode, res);
    row.address = addres_num;
    addToTablelList(mainTable, row);

    /* Add the first line to symbol table*/
    if ((strcmp(label, "")) != 0)
    {
        symbol sym;
        strcpy(sym.name, label);
        sym.value = addres_num;
        strcpy(sym.attributes[0], "data");
        sym.attrNum = 1;
        addToSymbolList(symbolTable, sym);
    }

    for (i = 1; i < counter; i++)
    {

        res = presentNumberAsBinary(getSizeBytsByGuidance(commend), numbers[i]);
        rowTable row;
        strcpy(row.sourceCode, "");
        strcpy(row.commend, "");
        strcpy(row.label, "");
        strcpy(row.machineCode, res);
        row.address = addres_num + (i * size);
        addToTablelList(mainTable, row);
    }
    addres_num = addres_num + (i * size);
}

int isCommend(char commend[])
{
    if (getTypeByName(commend) == 'R' || getTypeByName(commend) == 'J' || getTypeByName(commend) == 'I')
    {
        return 0;
    }
    else
        return 1;
}



