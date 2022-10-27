#include <bits/stdc++.h>
using namespace std;
const string kKeyWordName[32]={"auto","break","case","char","const","continue","default","do",\
	"double","else","enum","extern","float","for","goto","if","int","long","register","return",\
	"short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void",\
	"volatile","while"};


class Code_test
{
    public:
        Code_test(){}
        bool Divide(int &index);
        void T1(string divided_unit);
        void T2(int &index);
        void T_File(string filename);
        void Display(int level);
    private:
        string str_line,word;
		int Keyword_Count;
        ifstream Infile;
        ofstream OutFile;
        stack <int> if_else_Stack;
        queue <int> switch_Queue;
        bool Is_Previous_Else;
        
        int If_else_count;
        int If_elseif_else_count;
        int Switch_Count;
}; 

bool Code_test::Divide(int &index)
{
    if(str_line[index] == '"')
	{
		int j = index;
		for(index = index + 1; index < str_line.size(); index++)
		{
			if(str_line[index] == '"') return true;
		}
		index = j;
		return true;
	}
	if(str_line[index] == '/' && str_line[++index] == '*')
	{
		for(index = index + 1; index < str_line.size(); index++)
		{
			if(str_line[index] == '*' && str_line[++index] == '/') return true;
		}
		while(getline(Infile, str_line))
		{
			for(index = 0; index < str_line.size(); index++)
			{ 
				if(str_line[index] == '*' && str_line[++index] == '/') return true;
			}		
		}
	}
    if(str_line[index] == '(' || str_line[index] == ')') return true;  
	else if(str_line[index] == '<' || str_line[index] == '>') return true;
	else if(str_line[index] == '&' || str_line[index] == '*') return true;   
    else if(str_line[index] == '{' || str_line[index] == '}') return true;  
     
	
    else if(str_line[index] == '\n'|| str_line[index] == '\0' || str_line[index] == '\t') return true;  
    else if(str_line[index] == ' ' || str_line[index] == ',' || str_line[index] == ';' || str_line[index] == ':') return true;  
	return false;
}





void Code_test::T2(int &index)
{
	for(index; index < str_line.size(); index++) if (!(str_line[index] == ' ' || str_line[index] == '\t')) break;
	if(index == str_line.size())
	{
		int flag = 0;
		while (getline(Infile, str_line))
		{
			for(index = 0; index < str_line.size(); index++)
			{
				if (!(str_line[index] == ' ' || str_line[index] == '\t'))
				{
					flag = 1;
					break;
				}
			}
			if(flag) break;
		}	
	} 
	if(str_line[index] == 'i' && str_line[index+1] == 'f') 
	{
		if_else_Stack.push(1);
		index++;
		Keyword_Count++;
	}
	else
	{
		int ifelse_stack_count_ = 0;
		while(if_else_Stack.top() > 0 && !if_else_Stack.empty())
		{
			ifelse_stack_count_ += if_else_Stack.top();
			if_else_Stack.pop();
		}
		if(ifelse_stack_count_ > 1) If_elseif_else_count++;
		else if(ifelse_stack_count_ == 1) If_else_count++;
	}				
}

void Code_test::T1(string divided_unit)
{
	for(int i = 0; i < 32; i++) if(divided_unit == kKeyWordName[i]) Keyword_Count++;
	if(divided_unit == "switch" && Switch_Count)  
	{
		switch_Queue.push(Switch_Count);
		Switch_Count = 0;
	}
	if(divided_unit == "case") Switch_Count++;
	if(divided_unit == "if") 
	{
		if_else_Stack.push(-1);
		if_else_Stack.push(1);
	} 
	if(divided_unit == "else") Is_Previous_Else = true;
}

void Code_test::T_File(string filename)
{
    Is_Previous_Else = false;
    Keyword_Count = 0;
    If_else_count = 0;
    If_elseif_else_count = 0;
    Switch_Count = 0; 
    vector <string> outfilename_;
    Infile.open(filename, ios::in) ;
    OutFile.open("1out.txt", ios::trunc);
	if (!Infile.is_open())
	{
		cout << "File does not exist" << endl;
		exit (0);
	}
    while (getline(Infile, str_line))
    {
        word.clear();
        for (int i = 0; i <= str_line.size(); ++i)
        {
			if (Divide(i))
            {
                if (word.size() > 0)
                {
					T1(word);
					if(Is_Previous_Else)
					{
						T2(i);
						Is_Previous_Else = false;
					}
					outfilename_.push_back(word);
                    word.clear();
                }
                if(str_line[i] == '{') if_else_Stack.push(0); 
				else if(str_line[i] == '}')
				{
					while(if_else_Stack.top() != 0) if_else_Stack.pop();
					if_else_Stack.pop();	
				}
            }
            else word += str_line[i];
        }
    }
    if(Switch_Count)
	{
		switch_Queue.push(Switch_Count);
		Switch_Count = 0;
	}
    for (auto it:outfilename_) OutFile<<it<<endl;
}


void Code_test::Display(int level)
{
    if(level >= 1) cout<<"total num: "<<Keyword_Count<<endl;
    if(level >= 2)
    {
        cout<<"switch num: "<<switch_Queue.size()<<endl;
        cout<<"case num:";
		while(!switch_Queue.empty())
		{
			cout<<" "<<switch_Queue.front();
	        switch_Queue.pop();
		}
	    cout<<endl;
    }
    if(level >= 3) cout<<"if-else num: "<<If_else_count<<endl;
    if(level >= 4) cout<<"if-elseif-else num: "<<If_elseif_else_count<<endl;
}



int main()
{
    Code_test *to_text = new Code_test();  
    string infilename;  
    int level;
	cin>>infilename>>level;
    to_text->T_File(infilename); 
    to_text->Display(level);  
    return 0;
}
