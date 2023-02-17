#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
using namespace std;

class SymbolInfo{

        string name;
        string type;
       

    public:
        vector<SymbolInfo*> children;
        SymbolInfo* next;
        int sl=0;
        int el=0;
        bool isLeaf;

        //----------------------for_ICG---------------------
        int array_size=0;
        int stack_offset=0;
        //--------------------------------------------------

        SymbolInfo(){
            name="";
            type="";
            next=nullptr;

            sl=0;
            el=0;
            isLeaf=true;
        //---------------------------------------------------
            array_size=0;
        }

        SymbolInfo(string type,string name){

            this->name=name;
            this->type=type;
            next=nullptr;

            sl=0;
            el=0;
            isLeaf=true;
        //---------------------------------------------------
            array_size=0;

        }

       string getName(){
            return name;
        }
        void setName(string name){
            this->name=name;
        }
        string getType(){
            return type;
        }
        void setType(string type){
            this->type=type;
        }

};

class ScopeTable{


    SymbolInfo* node;
    int Size;
    int i=0;
    int this_scope_count;
    int line_pos=0;
    SymbolInfo* temp;
    SymbolInfo* prev;

    public:
        ScopeTable* parent_scope;

    int getScopeCount(){
    return this_scope_count;
    }

     ScopeTable(int Size, ScopeTable* parent_scope, int this_scope_count){
     this->Size=Size;
     this->parent_scope=parent_scope;
     this->this_scope_count=this_scope_count;
     node=new SymbolInfo[Size];
     }

      void Print(){

            for(int j=0;j<Size;j++){

                if(node[j].getName()!="")
                    cout<<"        "<<j+1<<"--> <"<<node[j].getName()<<","<<node[j].getType()<<"> ";
                else
                    cout<<"        "<<j+1<<"--> ";
                temp=node[j].next;
                while(temp!=nullptr)
                    {
                        cout<<"<"<<temp->getName()<<","<<temp->getType()<<"> ";
                        temp=temp->next;
                    }
                cout<<endl;
            }
        }


 SymbolInfo* Look_Up(string name){
  i=hashFunction(name,Size);

  if(node[i].getName()=="")
     return nullptr;


  if(node[i].getName()==name)
  {
       line_pos=1;
       
    return &node[i];
  }



  temp=node[i].next;
    line_pos=2;
  while(temp!=nullptr)
  {
    if(temp->getName()==name)
        {
              
            return temp;}
    else
        temp=temp->next;

    line_pos++;
  }
  return nullptr;
 }


  SymbolInfo* Look_UpI(string name){
  i=hashFunction(name,Size);

  if(node[i].getName()=="")
  {
    return nullptr;
   
  }
     

  if(node[i].getName()==name)
  {
    return &node[i];
 
  }

  temp=node[i].next;
    line_pos=2;
  while(temp!=nullptr)
  {
    if(temp->getName()==name)
            return temp;
    else
        temp=temp->next;

    line_pos++;
  }
  return nullptr;
 }





 bool Insert(string name, string type){
    if(Look_UpI(name)!=nullptr)
    {
        
        return false;
    }

      i=hashFunction(name,Size);
      
      if(node[i].getName()=="")
      {
          line_pos=1;
          node[i].setName(name);
          node[i].setType(type);
          
          return true;
      }

       if(node[i].next==nullptr)
        {
            line_pos=2;
            node[i].next=new SymbolInfo();
            node[i].next->setType(type);
            node[i].next->setName(name);
          
            return true;
        }

        prev=node[i].next;
        temp=prev->next;
        line_pos=3;
    while(temp!=nullptr)
    {
        line_pos++;
        prev=temp;
        temp=temp->next;
    }

    prev->next=new SymbolInfo();
    prev->next->setType(type);
    prev->next->setName(name);

 

    return true;
 }


bool Delete(string name){

if(Look_UpI(name)==nullptr)
{
 
  return false;
}

i=hashFunction(name,Size);

    if(node[i].getName()==name)
        if(node[i].next!=nullptr)
            {
            line_pos=1;
            node[i].setType(node[i].next->getType());
            node[i].setName(node[i].next->getName());
            node[i].next=node[i].next->next;
        
            delete node[i].next;
            return true;}
        else
           {line_pos=1;
          
            node[i].setName("");
            node[i].setType("");
            return true;}


        if(node[i].next->getName()==name)
        {
            line_pos=2;
          
            node[i].next=node[i].next->next;
            delete node[i].next->next;
            return true;
        }


        prev=node[i].next;
        temp=prev->next;
        line_pos=2;

    while(temp!=nullptr){
        line_pos++;
       if(temp->getName()==name)
           {
              
            prev->next=temp->next;
            delete temp;
            return true;
           }
           prev=temp;
           temp=temp->next;
    }
 }


 ~ScopeTable(){

    delete[] node;

 }


 unsigned long long int SDBMHash(string str) {
	unsigned long long int hash = 0;
	unsigned int i = 0;
	unsigned int len = str.length();

	for (i = 0; i < len; i++)
	{
		hash = (str[i]) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

int hashFunction(string name, int Size){
    return SDBMHash(name)%Size;
}











};

class SymbolTable{

    ScopeTable* current;
    ScopeTable* temp;
    int Size;
    int current_count=0;
    public:
        SymbolTable(int Size){

          
            this->Size=Size;
            current=nullptr;
            Enter_Scope();
        }

        void Enter_Scope(){
            current_count++;
            temp=new ScopeTable(Size,current,current_count);
    
            current=temp;
        }
        void Exit(){
            if(current->getScopeCount()!=1){
           
                temp=current;
                current=current->parent_scope;
                delete temp;
            }
        
        }
        bool Insert(string name,string type){
            return current->Insert(name,type);
        }
        bool Remove(string name){
            return current->Delete(name);
        }

        SymbolInfo* Look_Up(string name){
            temp=current;
            do{
                if(temp->Look_UpI(name)!=nullptr)
                    return temp->Look_Up(name);
                temp=temp->parent_scope;
            }while(temp!=nullptr);

            return nullptr;
        }


 void Print_Current_Scope_Table(){
            cout<<"        ScopeTable# "<<current->getScopeCount()<<endl;
            current->Print();
        }
        void Print_All_Scope_Table(){

            temp=current;
            do{
                cout<<"        ScopeTable# "<<temp->getScopeCount()<<endl;
                temp->Print();
                temp=temp->parent_scope;
            }while(temp!=nullptr);
        }

        void Terminate(){
                while(current!=nullptr){
                 
                    temp=current;
                    current=current->parent_scope;
                    delete temp;
                }

         

        }

        ~SymbolTable(){}
};
