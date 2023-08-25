#include<bits/stdc++.h>

using namespace std;

vector<vector<string>> input_instructions; 
// stores the input instruction file in an array
// 0th index holds the address of that instruction
// 1st index stores the labels if any
// 2nd index stores the opcode/ byte/ word/ resb/ resw
// 3rd index stores the operand if any
string program_name, str, starting_address;
unordered_map <string, string> symbol_table; // symbols are mapped to address
vector <string> final_object_code; 
unordered_map <string, string> opcode_table; 
unordered_map<int, vector<string>> comments; // stores all the comments which start at a particular index
string output_upper;   // used for outputting in uppercase

// SET OPCODE TABLE
void set_opcode_table()
{
    // sets the opcode table
	opcode_table.insert(make_pair("LDA", "00"));
	opcode_table.insert(make_pair("AND", "40"));
	opcode_table.insert(make_pair("DIV", "24"));
	opcode_table.insert(make_pair("SUB", "1C"));
	opcode_table.insert(make_pair("ADD", "18"));
	opcode_table.insert(make_pair("LDL", "08"));
	opcode_table.insert(make_pair("RD", "D8"));
	opcode_table.insert(make_pair("WD", "DC"));
	opcode_table.insert(make_pair("LDCH", "50"));
	opcode_table.insert(make_pair("STX", "10"));
	opcode_table.insert(make_pair("JLT", "38"));
	opcode_table.insert(make_pair("TIX", "2C"));
	opcode_table.insert(make_pair("TD", "E0"));
	opcode_table.insert(make_pair("STCH", "54"));
	opcode_table.insert(make_pair("STL", "14"));
	opcode_table.insert(make_pair("LDX", "04"));
	opcode_table.insert(make_pair("RSUB", "4C"));
	opcode_table.insert(make_pair("STA", "0C"));
	opcode_table.insert(make_pair("J", "3C"));
	opcode_table.insert(make_pair("JEQ", "30"));
	opcode_table.insert(make_pair("COMP", "28"));  
	opcode_table.insert(make_pair("JSUB", "48"));
	opcode_table.insert(make_pair("JGT", "34"));
	opcode_table.insert(make_pair("MUL", "20"));
	opcode_table.insert(make_pair("OR", "44"));
	opcode_table.insert(make_pair("STSW", "E8"));
}

// PRINT OPCODE TABLE IN opcode_table.txt FILE
void print_opcode_table()
{
	// prints the opcode table
	ofstream myOpcode("opcode_table.txt");
	for(auto x: opcode_table)
	{
		myOpcode << x.first << " " << x.second << endl;
	}
	myOpcode.close();
}

// PRINT SYMBOL TABLE IN symbol_table.txt FILE
void print_symbol_table()
{
	// prints the symbol table
	ofstream mySymbol("symbol_table.txt");
	for(auto x: symbol_table){
		output_upper = x.second;
		transform(output_upper.begin(), output_upper.end(), output_upper.begin(), ::toupper);
		mySymbol << x.first << " " << output_upper << endl;
	}
	mySymbol.close();
}

string get_opcode_table(string str){
	// returns the opcode value if present else -1
	if(opcode_table.find(str) == opcode_table.end())
		return "-1";
	
	return opcode_table[str];
}

// PARSE INPUT AND STORE IT IN A VECTOR
void input()
{
	set_opcode_table();
	print_opcode_table();
	char ch;
	string str, temp;
	int index = 0;
	while(!cin.eof())
	{
	    str.clear();
		input_instructions.push_back({"\t","\t","\t","\t"});
		getline(cin, str);
		int k = 0, flag = 0, found_opcode_table = 0, i=0;
		// each iteration stores the new line of the file in str
		while(i < str.size())
		{
			while(i < str.size() && str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
			{
				temp.push_back(str[i]);
				flag = 1;
				i++;
			}
			
			if(flag)
			{
				if(temp == "."){
					// comments[index] stores all the comments which start at index in the object code
					// comments[index] will appear before final_object_code[index]
					comments[index].push_back(str);
					index--;
					input_instructions.pop_back();
					temp.clear();
			   		flag = 0;
					break;
				}
				
			    if(get_opcode_table(temp) == "-1" && ( temp != "BYTE" && temp != "WORD" && temp != "RESW" && temp != "RESB"
				&& temp != "END"))
			    {
			    	if(!found_opcode_table)
			    		input_instructions[index][1] = temp; // if opcode not found yet, then temp is label
			    	else
			    		input_instructions[index][3] = temp; // if opcode found, then temp is operand
			    }
			    else
			    {
			    	input_instructions[index][2] = temp;  // temp is opcode/byte/word/resw/resb
			    	found_opcode_table = 1;
			    }
			    temp.clear();
			    flag = 0;
			}
			i++;
	    }
	    index++;
	}
}

int hexToDecimal(string str)
{
	// convert hex string str to int decimal
    int y;
    stringstream stream;
    stream << str;
    stream >> hex >> y;
    return y;
}

string decimalToHex(int num)
{
	// convert decimal string str to int hex
    stringstream stream;
    stream << hex << num;
    return stream.str();
}

string add_hex(string str, string adder)
{
	// adder is in hex
	int num1 = hexToDecimal(str);
	int num2 = hexToDecimal(adder);
	int sum = num1 + num2;
	return decimalToHex(sum);
}

string add_dec(string str, string adder)
{
	// adder is in dec
	int num1 = hexToDecimal(str);
	int num2 = atoi(adder.c_str());
	int sum = num1 + num2;
	return decimalToHex(sum);
}

// SET THE ADDRESS OF EACH INSTRUCTION
void set_address()
{
	// sets address which is the first index in input_instructions
	input_instructions[0][0] = starting_address;
	if(input_instructions[0][1].size() > 0 && input_instructions[0][1]!="\t")
		symbol_table[input_instructions[0][1]] = input_instructions[0][0];
	
	int i = 1;
	while(i < input_instructions.size() )
	{
		string label2 = input_instructions[i-1][2];  // mnemonic of previous instruction 
		string prev_addr = input_instructions[i-1][0];  // address of previous instruction
		
		if(label2 == "BYTE")
		{
			int bytes_to_incr;
			string label3 = input_instructions[i-1][3];
			if(label3[0] == 'C')
			{
				// each char gets one byte
				// subtract 3 to remove ',C,' characters
				bytes_to_incr = (label3.size() - 3);
			}
			else
			{
				// input_instructions[i-1][3][0] == 'X'
				// every 2 characters in size gets one byte. If odd, remaining char gets one byte.
				bytes_to_incr = ((label3.size() - 3 + 1) / 2);
			}
			string c =  std::to_string(bytes_to_incr);
			input_instructions[i][0] = add_dec(prev_addr,c); // c is in decimal
		}
		else if(label2 == "RESB")
		{
			// 3rd index contains number of bytes to reserve
			int reserve_byte = atoi(input_instructions[i-1][3].c_str());
			string hexaReserve_byte = decimalToHex(reserve_byte);
			input_instructions[i][0] = add_hex(prev_addr, hexaReserve_byte);
		}
		else if(label2 == "RESW")
		{
			// 3rd index contains number of words to reserve, each word being 3 bytes
			int reserve_word = 3 * atoi(input_instructions[i-1][3].c_str());
			string hexaReserve_word = decimalToHex(reserve_word);
			input_instructions[i][0] = add_hex(prev_addr, hexaReserve_word);
		}
		else{
			// all the other instructions take 3 bytes each
			input_instructions[i][0] = add_dec(prev_addr, "3");
		}
		
		if(input_instructions[i][1].size() > 0 && input_instructions[i][1]!="\t")
			symbol_table[input_instructions[i][1]] =input_instructions[i][0];   // setting the address for all the labels we encouter
		i++;
	}
	print_symbol_table();
}

// GENERATE THE OBJECT CODE
void generate_final_object_code()
{
	
    FILE* myIntermediate = fopen("intermediate.txt", "w");   // name of the file where the intermediate code will be printed
	string objectCode = "", label2, label3, label_address;
	fprintf(myIntermediate,"%-5s%-8s%-8s%s\n",starting_address.c_str(),program_name.c_str(),"START",starting_address.c_str());
	
	// each iteration will generate the object code for each instruction, and also output the intermediate code of that instruction, along with comments 
	for(int i = 0; i < input_instructions.size() - 1; i++)
	{
		// output comments present at the index i
		if(comments.find(i)!=comments.end()){
			for(auto com:comments[i]) 
				fprintf(myIntermediate,"%-5s\n", com.c_str());
		}
		output_upper = input_instructions[i][0];
		transform(output_upper.begin(), output_upper.end(), output_upper.begin(), ::toupper);
		if(input_instructions[i][1]=="\t") 
			fprintf(myIntermediate,"%-5s%-8s%-8s%s\n",output_upper.c_str(),"",input_instructions[i][2].c_str(),input_instructions[i][3].c_str());
		else 
			fprintf(myIntermediate,"%-5s%-8s%-8s%s\n",output_upper.c_str(),input_instructions[i][1].c_str(),input_instructions[i][2].c_str(),input_instructions[i][3].c_str());
		

		int flag = 0;
		objectCode.clear();
		label2 = input_instructions[i][2];
		if(label2 == "RESW" || label2 == "RESB" || label2 == "END")
		{
			// no object code for these assembly directives
			final_object_code.push_back("\t");
			continue;
		}
		label3.clear();  // stores the first operand
		int l = 0;
		while( l < input_instructions[i][3].size() && input_instructions[i][3][l] != ',')
		{
			label3 += input_instructions[i][3][l];
			l++;
		}
		// flag is set to 1 if there are more than one operands
		if(l < input_instructions[i][3].size() && input_instructions[i][3][l] == ',') flag=1;  

		if(label2 == "BYTE")
		{
			if(label3[0] == 'C')
			{
				for(int i = 2; i < label3.size()-1; i++)
				{
					int ascii = label3[i];
					objectCode += (decimalToHex(ascii));
				}
			}
			else
			{
				for(int i = 2; i < label3.size()-1; i++)
				{
					objectCode += (label3[i]);
				}
			}
			final_object_code.push_back(objectCode);
		}
		else if(label2 == "WORD")
		{
			objectCode += decimalToHex(atoi(label3.c_str()));
			
			// padding with zeros to make length = 6
			string zero="";
			for(int i = 0; i < 6 - objectCode.size(); i++)
			{
				zero += "0";
			}
			if(zero!="") objectCode.insert(0, zero);
			
			final_object_code.push_back(objectCode);
		}
		else{
			// executable instructions, i.e. no assembly directives
			objectCode += get_opcode_table(label2);
			if(label3 == "\t")
			{
				// instruction contains no operands
				objectCode += "0000";
				final_object_code.push_back(objectCode);
				continue;
			}

			// add label address to the objectCode
			label_address = symbol_table[label3];
			if(label_address[0] > '7')
			{
				if(label_address[0] >= 'A')
					label_address[0] -= 15;
				else
					label_address[0] -= 8;
			}
			objectCode += label_address;

			if(flag) objectCode = add_hex(objectCode, "8000"); // more than one operands

			// padding with zeros to make length of objectCode = 6
			string zero="";
			for(int i = 0; i < 6 - objectCode.size(); i++)
			{
				zero += "0";
			}
			if(zero!="") objectCode.insert(0, zero);
			
			final_object_code.push_back(objectCode);
		}
	}

	// output last instruction, "END"
	output_upper = input_instructions.back()[0];
	transform(output_upper.begin(), output_upper.end(), output_upper.begin(), ::toupper);
	fprintf(myIntermediate,"%-5s%-8s%-8s%s\n",output_upper.c_str(),input_instructions.back()[1].c_str(),input_instructions.back()[2].c_str(),input_instructions.back()[3].c_str());
	fclose(myIntermediate);
}

// GENERATE THE MACHINE CODE
void generate_machine_code()
{
	ofstream myOutput("output.txt");   // name of the file where the final machine code will be printed
	
	string size = decimalToHex(hexToDecimal(input_instructions[input_instructions.size()-1][0]) - hexToDecimal(starting_address));
	// difference between the address of the last instruction and the first instruction (starting address)
	// prints the header record
	output_upper = size;
	transform(output_upper.begin(), output_upper.end(), output_upper.begin(), ::toupper);
	myOutput << "H" << program_name << "  00" << starting_address << "00" << output_upper << endl;
	
	int i = 0, save = 0, first_exe_flag = 0;
	string first_Executable_Address;
	// prints the text record
	// each iteration in the while loop prints each line in the machine code
	while(1)
	{
		int flag = 0, obj_code_len = 0;
		i = save;  // save stores the index of the starting instruction of the current line
		while(i < input_instructions.size()-1 && final_object_code[i] != "\t" && obj_code_len < 30)    // not resb,resw,end instructions
		{
			
			if(!flag && get_opcode_table(input_instructions[i][2]) != "-1")   // first executable instruction of the current line
			{
				myOutput << "T00";
				flag = 1;

				// output the address of the first executable instruction of the current line
				output_upper = input_instructions[i][0];
				transform(output_upper.begin(), output_upper.end(), output_upper.begin(), ::toupper);
				myOutput << output_upper;

				if(!first_exe_flag)   
				{
					// store the first executable instruction of the entire instruction set in first_Executable_Address
					first_Executable_Address = input_instructions[i][0];
					first_exe_flag = 1;
				}
			}

			// object code length must be less than 60 (every two characters increases obj_code_len by 1)
			if(obj_code_len + (final_object_code[i].size() / 2) > 30) break;
			else obj_code_len += (final_object_code[i].size() / 2);
			i++;

		}

		if(!flag)
		{
			// none of the instructions found were executable, i.e. not assembly directives
			save++;
			continue;
		}

		// output length of the current line 
		string len = decimalToHex(obj_code_len);
		for(int i=0; i<2-len.length();i++) myOutput<< "0";   // padding so that length size is 2
		output_upper = len;
		transform(output_upper.begin(), output_upper.end(), output_upper.begin(), ::toupper);
		myOutput << output_upper;
		
		i = save;
		obj_code_len = 0;
		while(obj_code_len < 30 && i < input_instructions.size() - 1)
		{
			if(final_object_code[i] != "\t")
			{
				// the instruction is not resb, resw or end
				obj_code_len += (final_object_code[i].size() / 2);
				if(obj_code_len > 30) break;
					 
				// output the object code of the particular instruction
				output_upper = final_object_code[i];
				transform(output_upper.begin(), output_upper.end(), output_upper.begin(), ::toupper);
				myOutput << output_upper ;
			}
			else
			{
				i++;
				break;
			}
			i++;
		}
		save = i;
		if(i >= input_instructions.size()-1) break;
		myOutput << endl;
	}

	// prints the end record
	output_upper=first_Executable_Address;
	transform(output_upper.begin(), output_upper.end(), output_upper.begin(), ::toupper);
	myOutput << endl << "E00" << output_upper << endl;
	myOutput.close();
}

int main()
{
	cin >> program_name >> str >> starting_address;
	cin.ignore();
	input();                        // parses and stores the input from the file in a vector named input_instructions
	set_address();                  // sets address of each instruction and also sets the symbol table  
	generate_final_object_code();   // generates the object code, and output the intermediate code in a file
	generate_machine_code();	    // generates the machine code, and outputs it in a file
}