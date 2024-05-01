#include <iostream>
#include <string>
#include <cctype>

using namespace std;


/*
HashElement는 Hash table을 이루는 원소의 자료형이다. private member variable로 hashValue, symbol, link를 가지고 있다. 
main() 함수에서 HashTable이 초기화되는데, hashValue를 0부터 -100까지로 설정된다.
data encapsulation을 위해서 getHashElement_HashValue(읽기), getHashElement_HashSymbol(읽기), getHashElement_Link(읽기), setHashElement_HashValue(쓰기), setHashElement_HashSymbol(쓰기), setHashElement_Link(쓰기) 함수를 사용하여
hashValue와 symbol을 읽고 쓰는 작업을 수행한다. Link를 따로 설정해 주지 않을 경우, 이를 구분하기 위해서 link를 INT_MAX로 설정하였다.
*/
class HashElement{
    public:
        HashElement():hashValue(INT_MIN),symbol(""),link(INT_MAX){}
        HashElement(int hashValue, string symbol, int link): hashValue(hashValue), symbol(symbol), link(link) {}

        int getHashElement_HashValue(){return hashValue;}
        string getHashElement_HashSymbol(){return symbol;}
        int getHashElement_Link(){return link;}

        void setHashElement_HashValue(int newHashValue){hashValue=newHashValue;}
        void setHashElement_HashSymbol(string newHashSymbol){symbol=newHashSymbol;}
        void setHashElement_Link(int newLink){link=newLink;};
    private:    
        int hashValue;
        string symbol;
        int link;
};

/*
NodeElement는 Node array를 이루는 원소의 자료형이다. private member variable로 lchild와 rchild를 가지고 있다.
main() 함수에서 Node array가 초기화되는데, lchild는 0으로, rchild는 바로 다음 node를 가리키도록 하였다. 
main() 함수에서 쓰인 Node array는 dynamic array로 구현하였다. dynamic array 의 index는 0부터 시작하지만 Node array의 index는 개념상 1부터 시작하도록 하였다.
따라서 dynamic array의 index에 +2를 한 값이 rchild 값으로 들어간다.
data encapsulation을 위해서 getLchild(읽기), getRchild(읽기), setLchild(쓰기), setRchild(쓰기) 함수를 이용하여 lchild와 rchild를 읽고 쓰는 작업을 수행한다.
*/
class NodeElement{
    public:
        NodeElement():lchild(0),rchild(0){}
        NodeElement(int lchild, int rchild): lchild(lchild), rchild(rchild){}

        int getLchild(){return lchild;}
        int getRchild(){return rchild;}

        void setLchild(int newLchild){lchild=newLchild;}
        void setRchild(int newRchild){rchild=newRchild;}
    private:
        int lchild;
        int rchild;
};

/*
stringVector는 scheme 명령어 단위를 나누어 저장해 두는 자료형이다. private member variable로 vec과 size를 가지고 있다.
vec은 string에 대한 dynamic array이며,입력된 명령어가 "(" 와 ")" 와 " "을 기준으로 나뉘어질 때 분절된 이들을 모두 vec에 담아야 하므로 넉넉하게 size를 300으로 잡았다.
size는 vec에서 내용이 채워진 node의 개수를 말한다. 
push_back 함수는 새로운 string을 vec에 연속적으로 채워넣는 함수이며, size 변수를 활용한다.
*/
class stringVector{
    public:
        stringVector(){
            vec=new string[300];
            size=0;
        };
        void push_back(string str){
            vec[size]=str;
            size++;
        };

        string* getVec(){return vec;}
        int getSize(){return size;}

    private:
        string* vec;
        int size;
};

/*
intPair class는 VariableStack class에 사용될 class로 variable의 hash value와 variable이 가리키는 값의 hash value를 각각 name과 link라는 member variable로 가지고 있는다.
*/
class intPair{
    public:
        intPair(int newName, int newLink){
            name=newName;
            link=newLink;
        }
        intPair(){
            name=0;
            link=0;
        }
        int getName(){return name;}
        int getLink(){return link;}

    private:
        int name;
        int link;
};

/*
VariableStack class는 함수에서의 parameter로 기존에 define 되어 있던 variable을 사용할 때 사용된다. 예를 들어 기존에 (define x 3)을 통해 x를 3으로 설정해 놓았는데 x를 parameter로 갖는 함수 square가 (square 5)
로 사용되면 x와 3이라는 pair가 VariableStack class에 stack으로 쌓이게 된다. 함수의 evaluation이 끝나면 stack에서 다시 pair를 꺼내 x를 3으로 다시 설정해 놓는다.
이때 x와 3이라는 pair는 x의 hashvalue와 3의 hashvalue를 member variable로 갖는다.
*/
class VariableStack{
    public:
        VariableStack(){
            stack=new intPair[300];
            size=0;
        };
        void push(int i, int k){
            stack[size]=intPair(i,k);
            size++;
        };
        intPair pop(){
            intPair returnValue=stack[size-1];
            size--;
            return returnValue;
        }

        intPair* getStack(){return stack;}
        int getSize(){return size;}
    private:
        intPair* stack;
        int size;
};

/*
StringToInt 함수는 string s를 parameter로 받아서 Hash value를 return하는 Hash function이다. 
여기서 Hash value는 양수이다. GetNextToken이라는 함수에서 StringToInt 함수의 return 값에 -1을 곱하여 음수로 만든다.
Hash table의 0번 노드에는 항상 empty list "()" 가 할당되므로
어떤 string s에 대하여 hash value를 0으로 계산하면 open addressing을 따라서 1을 반환한다.  
*/
unsigned int StringToInt(string s){
    int length=(int)s.length();
    unsigned int answer=0;
    if(length%2==1){
        answer=s.at(length-1);
        length--;
    }
    for(int i=0; i<length;i+=2)
    {
        answer+=s.at(i);
        answer+=((int)s.at(i+1))<<8;
    }
    if(answer%101==0){
        return 1;
    }
    return answer%101;
}

/*
divide 함수는 parameter로 받은 string str을 "(" , ")" , " "을 기준으로 나누는 함수이다. 
stringVector자료형의 변수 vec을 만든 후, string str를 나누어 vec의 memeber variable인 vec에 넣는다. "(" 와 ")"는 vec에 포함되지만, " "은 vec에 포함되지 않는다.
이때 str의 모든 글자는 tolower 함수를 이용해서 소문자로 변환한다.
*/

stringVector divide(string str){
    int size=str.length();
    string buffer;
    stringVector vec;
    for(int i=0; i<size; i++){
        if(str[i]!='(' && str[i]!=')' && str[i]!=' '){
            str[i]=tolower(str[i]);
            buffer+=str[i];
        }
        else if(str[i]!=' ' && buffer!=""){
            vec.push_back(buffer);
            buffer="";
            string charToString(1, str[i]);
            vec.push_back(charToString);
        }
        else if(str[i]!=' '){
            string charToString(1, str[i]);
            vec.push_back(charToString);
        }
        else if(str[i]==' ' && buffer!=""){
            vec.push_back(buffer);
            buffer="";
        }
    }
    if(buffer!=""){//괄호 없이 들어온 경우를 처리하기 위한 if문
        vec.push_back(buffer);
    }
    return vec;
}

/*
divideForPreprocessing 함수는 Preprocessing 함수에 사용될 함수이다. 위의 divide 함수와 비슷한 역할을 하지만 여기에서는 띄어쓰기도 vec에 들어간다.
*/
stringVector divideForPreprocessing(string str){
    int size=str.length();
    string buffer;
    stringVector vec;
    string apostrophe="'";
    for(int i=0; i<size; i++){
        if(str[i]!='(' && str[i]!=')' && str[i]!=' ' && str[i]!=apostrophe[0]){
            str[i]=tolower(str[i]);
            buffer+=str[i];
        }
        else if(str[i]!=' ' && buffer!=""){
            vec.push_back(buffer);
            buffer="";
            string charToString(1, str[i]);
            vec.push_back(charToString);
        }
        else if(str[i]!=' '){
            string charToString(1, str[i]);
            vec.push_back(charToString);
        }
        else if(str[i]==' ' && buffer!=""){
            vec.push_back(buffer);
            buffer="";
            string charToString(1, str[i]);
            vec.push_back(charToString);
        }
        else{
            string charToString(1, str[i]);
            vec.push_back(charToString);
        }
    }
    if(buffer!=""){//괄호 없이 들어온 경우를 처리하기 위한 if문
        vec.push_back(buffer);
    }
    return vec;
}

/*
GetNextToken 함수는 다음 statement를 return 하는 함수이다. parameter로 HashTable[], dividedStr, tokenNum을 받는다. 
dividedStr은 divide함수를 통해 인풋으로 받은 문자열을 stringVector 형식으로 변환한 것이다. tokenNum은 dividedStr에서의 index를 의미한다.
StringToInt 함수를 이용해서 hashValue의 후보값인 hashValueCand를 생성한다. open addressing을 사용하기 위해 move라는 변수와 while문을 사용하였다.
while 문 내에서는 if 문을 이용하여 hash table내에서 -hashValueCand+move라는 index에 있는 HashElement의 hash symbol이 현재 token의 symbol과 일치하는 지, 
혹은 일치하는 symbol이 없는 지 확인한다. 첫 if 문에 해당된다면 그대로 break를 하고 tokenNum에 해당하는 symbol을 return 한다.
두 번째 if 문에 해당된다면 tokenNum에 해당하는 statement를 symbol로 가지는 Hash element를 hash table에 추가하고 그 symbol을 return 한다.
GetNextToken 함수는 다음 statement를 받는 동시에 Hash table을 update하는 함수이기도 하다.
*/
string GetNextToken(HashElement HashTable[], stringVector dividedStr, int& tokenNum){
    if(tokenNum>=dividedStr.getSize()){
        return NULL;
    }
    int hashValueCand=(-1)*StringToInt(dividedStr.getVec()[tokenNum]);
    int move=0;
    while(true){
        if((-hashValueCand+move)%101!=0 && HashTable[(-hashValueCand+move)%101].getHashElement_HashSymbol()==dividedStr.getVec()[tokenNum]){
            break;
        }
        if((-hashValueCand+move)%101!=0 && HashTable[(-hashValueCand+move)%101].getHashElement_HashSymbol()==""){
            HashTable[(-hashValueCand+move)%101].setHashElement_HashSymbol(dividedStr.getVec()[tokenNum]);
            break;
        }
        move++;
    }
    return dividedStr.getVec()[tokenNum++];
}

/*
SetHashValue 함수는 parameter str을 HashTable에서 search 하고 str이 없다면 HashTable에 str을 HashSymbol로 가지는 element를 추가하는 기능을 하는 함수이다. 만약 str 변수가 이미 있다면 아무 작업도 하지 않는다. 
*/
void SetHashValue(HashElement HashTable[], string str){
    int hashValueCand=(-1)*StringToInt(str);
    int move=0;
    int hashValue=0;
    while(move<101){
        if((-hashValueCand+move)%101!=0 && HashTable[(-hashValueCand+move)%101].getHashElement_HashSymbol()==str){
            //hashValue=-((-hashValueCand+move)%101);
            break;
        }
        if((-hashValueCand+move)%101!=0 && HashTable[(-hashValueCand+move)%101].getHashElement_HashSymbol()==""){
            HashTable[(-hashValueCand+move)%101].setHashElement_HashSymbol(str);
            //hashValue=-((-hashValueCand+move)%101);
            break;
        }
        move++;
    }
}

/*
GetHashValue 함수는 parameter로 받은 string str을 symbol로 가지는 HashElement의 hash value를 return한다.
*/
int GetHashValue(HashElement HashTable[], string str){
    for(int i=0; i<101; i++){
        if(str==HashTable[i].getHashElement_HashSymbol()){
            return HashTable[i].getHashElement_HashValue();
        }
    }
    return INT_MAX;
    // int hashValueCand=(-1)*StringToInt(str);
    // int move=0;
    // int hashValue=0;
    // while(move<101){
    //     if((-hashValueCand+move)%101!=0 && HashTable[(-hashValueCand+move)%101].getHashElement_HashSymbol()==str){
    //         hashValue=-((-hashValueCand+move)%101);
    //         break;
    //     }
    //     if((-hashValueCand+move)%101!=0 && HashTable[(-hashValueCand+move)%101].getHashElement_HashSymbol()==""){
    //         HashTable[(-hashValueCand+move)%101].setHashElement_HashSymbol(str);
    //         hashValue=-((-hashValueCand+move)%101);
    //         break;
    //     }
    //     move++;
    // }
    // return hashValue;
}

/*
GetHashSymbol 함수는 parameter로 받은 int HashValue를 hash value로 가지는 HashElement의 symbol을 return한다.
*/
string GetHashSymbol(HashElement HashTable[], int HashValue){
    for(int i=0; i<101; i++){
        if(HashValue==HashTable[i].getHashElement_HashValue()){
            return HashTable[i].getHashElement_HashSymbol();
        }
    }
    return "";
}

/*
allocate 함수는 Node array의 free list가 어디서 부터 시작되는지 확인하고 그 index를 return하는 함수이다. 이 함수는 parse tree를 만들 때 사용된다.
dynamic array의 index에 1을 더한 값을 return 하는 이유는 node array의 node가 1부터 시작한다고 개념적으로 설정하였기 때문이다.
*/
int allocate(NodeElement NodeArray[]){
    int available;
    for(int i=0; i<101; i++){
        if((NodeArray[i].getLchild()==0 && NodeArray[i].getRchild()==i+2)&&i<100){//i가 100 이전일 경우에는 lchild가 0이고 rchild가 다음 node를 가리키는 node가 free list의 node이라는 점을 사용
            available=i+1;
            return available;
        }
        else if(i==100){
            if(NodeArray[i].getLchild()==0){//i가 100일 경우에는 lchild만 비어 있어도 free list임을 확인할 수 있다.
                available=i+1;
                return available;
            }
        }
    }
}

/*
read 함수는 문자열 전체를 읽고 parse tree를 구현한 후 root node를 반환하는 함수이다. parameter string str로 문자열을 받고 문자열의 어느 부분부터 읽어야 하는 지 알기 위해 parameter int& tokenNum을 받는다.
이때 call by reference로 tokenNum을 받은 것은 recursion을 하고 난 뒤, 기존의 read 함수로 돌아왔을 때에 어디까지 읽었는지 알기 위해서이다.
first 변수는 read 함수 내의 while loop를 처음 도는 것인지 확인하기 위한 것이다. hashSymbol 변수는 tokenNum에 해당하는 scheme statement를 가리킨다.
if(hashSymbol=="(")문 내부의 while문에 의해 parse tree가 만들어진다. 내부에서 또다른 "(" 이 나타나면 read 함수를 recursion하여 ")"이 나타날 때까지
문자열을 읽고 parse tree를 만든다. 368번째 줄 명령어에서 NodeArray[temp-1]의 lchild에 INT_MAX를 부여해 놓는 것은 recursion 된 read 함수 내부에서 root를 정할 때에 temp-1 노드를 건너 뛰도록 하게 하기 위해서이다.
369번째 줄에서 --tokenNum을 하는 것은 recursion된 read 함수 내부에서 hashSymbol이 "("부터 받아져야 하기 때문이다. pseudo code에서의 PushBack 함수 역할을 하는 것이다. 
386번째 줄의 else문은 괄호 없이 그냥 variable을 입력하였을 경우를 위한 명령어이다. 기존에 입력하지 않았던(Hash table에 없는) 변수가 입력될 경우, 에러의 의미로 0이 반환되고 입력되었던 변수가 입력된 경우, 그 변수에 해당하는 Hashvalue가 return 된다.
*/
int read(NodeElement NodeArray[], HashElement HashTable[], string str, int& tokenNum){
    int root=1;
    int temp=1;
    bool first=true;
    bool firstChange=false;
    bool while_Statement_pass=false;
    stringVector dividedStr=divide(str);
    string hashSymbol=GetNextToken(HashTable,dividedStr,tokenNum);
    if(hashSymbol=="("){
        hashSymbol=GetNextToken(HashTable,dividedStr,tokenNum);
        while(hashSymbol!=")"){
            while_Statement_pass=true;
            if(first==true){
                temp=allocate(NodeArray);
                root=temp;
                first=false;
            }
            else{
                NodeArray[temp-1].setRchild(allocate(NodeArray));
                temp=NodeArray[temp-1].getRchild();
            }
            if(hashSymbol=="("){
                NodeArray[temp-1].setLchild(INT_MAX);
                NodeArray[temp-1].setLchild(read(NodeArray,HashTable, str,--tokenNum));
            }
            else{
                NodeArray[temp-1].setLchild(GetHashValue(HashTable, hashSymbol));
            }
            if(first==false){
                NodeArray[temp-1].setRchild(0);
            }
            hashSymbol=GetNextToken(HashTable,dividedStr,tokenNum);
        }
        if(while_Statement_pass){
            return root;
        }
        else{
            return 0;
        }
    }
    else{
        int hashValue=GetHashValue(HashTable, hashSymbol);
        if(hashValue==INT_MAX){
            return 0;
        }
        return hashValue;
    }
}

/*
findFreelistRoot 함수는 Freelist의 root를 찾는 함수이다.
parse tree가 node array의 앞에서부터 (index가 커지는 방향으로) 만들어지는 것을 이용해서 lchild가 0으로 비어있는 가장 앞의(index가 가장 작은) node가 freelist root가 됨을 추론했다.
i+1을 return 하는 것은 node array가 index 1부터 시작하기 때문이다.
*/
int findFreelistRoot(NodeElement NodeArray[]){
    for(int i=0; i<101; i++){
        if(i<100){
            if(NodeArray[i].getLchild()==0 && NodeArray[i].getRchild()==i+2){//i가 100 이전일 경우에는 lchild가 0이고 rchild가 다음 node를 가리키는 node가 free list의 node이라는 점을 사용
                return i+1;
            }
        }
        else{
            if(NodeArray[i].getLchild()==0){//i가 100일 경우에는 lchild만 비어 있어도 free list임을 확인할 수 있다.
                return i+1;
            }
        }
    }
}

/*
printNodeArray는 Node array를 출력하는 함수이다. index, lchild, rchild를 한 행에 출력한다.
*/
void printNodeArray(NodeElement NodeArray[]){
    for(int i=0; i<101; i++){
        cout<<i+1<<" "<<NodeArray[i].getLchild()<<" "<<NodeArray[i].getRchild()<<endl;
    }
}

/*
printHashTable은 Hash table을 출력하는 함수이다. hash value, symbol, link를 한 행에 출력한다.
*/
void printHashTable(HashElement HashTable[]){
    for(int i=0; i<101; i++){
        if(HashTable[i].getHashElement_HashSymbol()!=""){
            cout<<HashTable[i].getHashElement_HashValue()<<" "<<HashTable[i].getHashElement_HashSymbol()<<" "<<HashTable[i].getHashElement_Link()<<endl;
        }
    }
}

/*
print 함수는 parse tree로 부터 입력한 문자열을 재구성하고 출력하는 함수이다. index는 node array의 index일 수도 있고 hash table의 hash value가 될 수도 있다. 
startList 변수는 List의 시작에 "("을 출력하기 위해서 parameter로 넣었다. wasStart 변수는 띄어쓰기를 출력하기 위해서 parameter로 넣었다.
wasStart 변수는 symbol이 "(" 바로 뒤에 나타난 것인지 나타내는 변수이다. "(" 바로 뒤의 statement에만 앞에 띄어쓰기가 붙지 않는 것을 이용한 것이다.
*/
void print(int index, bool startList, bool wasStart, NodeElement NodeArray[], HashElement HashTable[]){
    if(index==0){
        cout<<"()";
    }
    else if(index<0){
        if(!wasStart){
            cout<<" ";
        }
        cout<<GetHashSymbol(HashTable,index);
    }
    else if(index>0){
        if(startList){
            if(!wasStart){
                cout<<" ";
            }
            cout<<"(";
            wasStart=true;
        }
        print(NodeArray[index-1].getLchild(), true, wasStart, NodeArray, HashTable);
        if(NodeArray[index-1].getRchild()!=0){
            print(NodeArray[index-1].getRchild(), false, false, NodeArray, HashTable);
        }
        else{
            cout<<")";
        }
    }
}

/*
output 함수는 과제2 의 출력 전체를 총괄하는 함수이다.
*/
void output(int root, int freelistRoot, NodeElement NodeArray[], HashElement HashTable[]){
    cout<<"] Free list's root = "<<freelistRoot<<endl;
    cout<<"Parse tree's root = "<<root<<endl;
    cout<<endl;

    cout<<"Node array ="<<endl;
    printNodeArray(NodeArray);
    cout<<endl;

    cout<<"Hash table ="<<endl;
    printHashTable(HashTable);
    cout<<endl;

    print(root, true, true, NodeArray, HashTable);
}

/*
Preprocessing 함수는 입력된 code를 말 그대로 preprocessing 해주는 함수이다. 함수를 define 하는데 lambda가 쓰이지 않는 형식으로 define할 경우, lambda가 쓰이는 형식으로 고쳐준다. 
한편, quote를 의미하는 "'"가 쓰였을 경우에는 quote라는 함수를 사용한 것처럼 명령어를 바꾸어준다. Preprocessing 함수는 command로 입력값을 받은 뒤, preprocessing을 끝낸 후에는 string으로 그 값을 내보낸다.
*/
string Preprocessing(string command, int& tokenNum){
    //newcommand is an empty string when this procedure is first called
    string newcommand;
    stringVector dividedCommand=divideForPreprocessing(command);
    string token=dividedCommand.getVec()[tokenNum++];
    while (tokenNum<=dividedCommand.getSize()){
        if (token=="define"){
            newcommand+="define ";
            token=dividedCommand.getVec()[tokenNum++];
            while(token==" "){
                token=dividedCommand.getVec()[tokenNum++];
            }
            if(token=="("){
                token=dividedCommand.getVec()[tokenNum++];
                newcommand=newcommand+token+"(lambda ("+Preprocessing(command,tokenNum)+")";
            }
            else{
                tokenNum--;
            }
        }
        else if(token=="'"){
            newcommand+="(quote ";
            int num_of_left_paren=0;
            do{
                token=dividedCommand.getVec()[tokenNum++];
                newcommand+=token;
                if(token=="("){
                    num_of_left_paren+=1;
                }
                else if(token==")"){
                    num_of_left_paren-=1;
                }
            }
            while(num_of_left_paren>0);
            newcommand+=")";
        }
        else {
            newcommand+=token;
        }
        token=dividedCommand.getVec()[tokenNum++];
    }
    return newcommand;
}

//isNumber 함수는 parameter string이 숫자인지 확인하는 함수이다. 
bool isNumber(const string& string){
    int dot=0;
    for(char const&c:string){
        if(isdigit(c)==0 && c!='.') return false;
        else if(c=='.'){
            dot++;
        }
    }
    if(dot<=1) return true;
    else return false;
}

//IsDefinedFunc 함수는 hashSymbol을 함수 이름으로 하는 user defined 함수가 있는지 hash table에서 체크하는 역할을 한다.
bool IsDefinedFunc(NodeElement* NodeArray, HashElement* HashTable, string hashSymbol){
    int HashValue=GetHashValue(HashTable, hashSymbol);
    int link=HashTable[-HashValue].getHashElement_Link();
    if(link>0 && link != INT_MAX){
        if(GetHashSymbol(HashTable, NodeArray[link-1].getLchild())=="lambda"){
            return true;
        }
    }
    return false;
}

int Eval(NodeElement NodeArray[], HashElement HashTable[], int root);

bool CheckStructure(NodeElement* NodeArray, HashElement* HashTable, int first_Operand, int second_Operand){
    if(first_Operand==second_Operand) return true; //우선 first_Operand와 second_Operand가 같으면 true를 return
    if(first_Operand*second_Operand<0) return false; //1. 부호가 다르면 false
    //int first_Operand_Lchild_Eval=Eval(NodeArray, HashTable, NodeArray[first_Operand-1].getLchild());
    //int second_Operand_Lchild_Eval=Eval(NodeArray, HashTable, NodeArray[second_Operand-1].getLchild());
    // int first_Operand_Rchild_Eval=Eval(NodeArray, HashTable, NodeArray[first_Operand-1].getRchild());
    // int second_Operand_Rchild_Eval=Eval(NodeArray, HashTable, NodeArray[second_Operand-1].getRchild());
    else if(first_Operand>0 && second_Operand>0){//2. 둘다 양수
        //if(first_Operand==second_Operand) return true;//2-1. 둘이 같은 것을 가리키는 지 체크
        if(CheckStructure(NodeArray, HashTable, NodeArray[first_Operand-1].getLchild(), NodeArray[second_Operand-1].getLchild())){//2-2. 같은 리스트 구조를 가졌는 지 체크
            if(NodeArray[first_Operand-1].getRchild() != 0 && NodeArray[second_Operand-1].getRchild() != 0){
                if(CheckStructure(NodeArray, HashTable, NodeArray[first_Operand-1].getRchild(), NodeArray[second_Operand-1].getRchild())){
                    return true;
                }
                else return false;
            }
            else if(NodeArray[first_Operand-1].getRchild()==NodeArray[second_Operand-1].getRchild()){
                return true;
            }
            else return false;
        }
        else return false;
    }
    else if(first_Operand<0 && second_Operand<0){
        if(first_Operand==second_Operand) return true;
        else return false;
    }
    else return false;
}

/*
Eval 함수는 root로 받은 주소에 대해 evaluation을 하는 함수이다.
만약 root로 0보다 작거나 같은 값이 들어왔다면 그 값이 가리키는 값(link된 값)을 return하거나, 가리키는 값이 없을 경우, root를 바로 return한다.
root로 0보다 큰 값이 들어왔다면 root 노드의 lchild를 확인하여 함수 call 인지 확인하고 함수 call이라면(if 와 else if문들) 각각의 함수 operation을 따라 그 결과 값을 가지는 주소를 return한다.
한편 root 노드의 lchild가 함수 call이 아닌 경우(else 문)에는 root를 return한다. 
*/
int Eval(NodeElement NodeArray[], HashElement HashTable[], int root){//nodearray가 한칸씩 밀려있는 것 생각해서 다시 코드 짜야함.
    if(root<=0){//Eval 에 들어온 root 가 음수이면 root 자체를 return 하거나 root가 link되어 있는 것을 return한다.
        if(HashTable[-root].getHashElement_Link()!=INT_MAX){
            return Eval(NodeArray, HashTable, HashTable[-root].getHashElement_Link());
        }
        else return root;
    }

    if(NodeArray[root-1].getLchild()>0){
        return root;
    }
    
    string hashSymbol=GetHashSymbol(HashTable, NodeArray[root-1].getLchild());
    if(hashSymbol=="="){
        int first_Operand=Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getRchild()-1].getLchild());
        int second_Operand=Eval(NodeArray, HashTable, NodeArray[NodeArray[NodeArray[root-1].getRchild()-1].getRchild()-1].getLchild());
        if(first_Operand<0 && isNumber(GetHashSymbol(HashTable, first_Operand))){
            if(second_Operand<0 && isNumber(GetHashSymbol(HashTable, second_Operand))){
                if(GetHashSymbol(HashTable, first_Operand)==GetHashSymbol(HashTable, second_Operand)){
                    return GetHashValue(HashTable, "#t");
                }
                else return GetHashValue(HashTable, "#f");
            }
            else{
                cout<<"error: you have to input number when you use ="<<endl;
                return 0;
            }
        }
        else{
            cout<<"error: you have to input number when you use ="<<endl;
            return 0;
        }
    }
    else if(hashSymbol=="eq?"){
        int first_Operand=Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getRchild()-1].getLchild());
        int second_Operand=Eval(NodeArray, HashTable, NodeArray[NodeArray[NodeArray[root-1].getRchild()-1].getRchild()-1].getLchild());
        if(first_Operand==second_Operand){
            return GetHashValue(HashTable, "#t");
        }
        else return GetHashValue(HashTable, "#f");
    }
    else if(hashSymbol=="equal?"){
        int first_Operand=Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getRchild()-1].getLchild());
        int second_Operand=Eval(NodeArray, HashTable, NodeArray[NodeArray[NodeArray[root-1].getRchild()-1].getRchild()-1].getLchild());
        bool equal=CheckStructure(NodeArray, HashTable, first_Operand, second_Operand);
        if(equal){
            return GetHashValue(HashTable, "#t");
        }
        else return GetHashValue(HashTable, "#f");
    }
    else if(hashSymbol=="+"){
        int first_Operand=Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getRchild()-1].getLchild());
        int second_Operand=Eval(NodeArray, HashTable, NodeArray[NodeArray[NodeArray[root-1].getRchild()-1].getRchild()-1].getLchild());
        double returnValue=stod(GetHashSymbol(HashTable, first_Operand)) + stod(GetHashSymbol(HashTable, second_Operand));
        int returnInt=returnValue;
        if(returnValue-returnInt>0){
            SetHashValue(HashTable, to_string(returnValue));//returnValue 가 HashTable에 없는 값일 수 있으므로 returnValue를 HashTable에 추가
            return GetHashValue(HashTable,to_string(returnValue));
        }
        else{
            SetHashValue(HashTable, to_string(returnInt));//returnInt 가 HashTable에 없는 값일 수 있으므로 returnInt를 HashTable에 추가
            return GetHashValue(HashTable,to_string(returnInt));
        }
    }
    else if(hashSymbol=="-"){
        int first_Operand=Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getRchild()-1].getLchild());
        int second_Operand=Eval(NodeArray, HashTable, NodeArray[NodeArray[NodeArray[root-1].getRchild()-1].getRchild()-1].getLchild());
        double returnValue=stod(GetHashSymbol(HashTable, first_Operand)) - stod(GetHashSymbol(HashTable, second_Operand));
        int returnInt=returnValue;
        if(returnValue-returnInt>0){
            SetHashValue(HashTable, to_string(returnValue));//returnValue 가 HashTable에 없는 값일 수 있으므로 returnValue를 HashTable에 추가
            return GetHashValue(HashTable,to_string(returnValue));
        }
        else{
            SetHashValue(HashTable, to_string(returnInt));//returnInt 가 HashTable에 없는 값일 수 있으므로 returnInt를 HashTable에 추가
            return GetHashValue(HashTable,to_string(returnInt));
        }
    }
    else if(hashSymbol=="*"){
        int first_Operand=Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getRchild()-1].getLchild());
        int second_Operand=Eval(NodeArray, HashTable, NodeArray[NodeArray[NodeArray[root-1].getRchild()-1].getRchild()-1].getLchild());
        double returnValue=stod(GetHashSymbol(HashTable, first_Operand)) * stod(GetHashSymbol(HashTable, second_Operand));
        int returnInt=returnValue;
        if(returnValue-returnInt>0){
            SetHashValue(HashTable, to_string(returnValue));//returnValue 가 HashTable에 없는 값일 수 있으므로 returnValue를 HashTable에 추가
            return GetHashValue(HashTable,to_string(returnValue));
        }
        else{
            SetHashValue(HashTable, to_string(returnInt));//returnInt 가 HashTable에 없는 값일 수 있으므로 returnInt를 HashTable에 추가
            return GetHashValue(HashTable,to_string(returnInt));
        }
    }
    else if(hashSymbol=="number?"){
        int arg=Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getRchild()-1].getLchild());
        if(arg<0 && isNumber(GetHashSymbol(HashTable, arg))){
            return GetHashValue(HashTable, "#t");
        }
        else return GetHashValue(HashTable, "#f");
    }
    else if(hashSymbol=="symbol?"){
        int arg=Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getRchild()-1].getLchild());
        if(arg<0 && !isNumber(GetHashSymbol(HashTable,arg))){
            return GetHashValue(HashTable, "#t");
        }
        else return GetHashValue(HashTable, "#f");
    }
    else if(hashSymbol=="null?"){
        int arg=Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getRchild()-1].getLchild());
        if(!arg){
            return GetHashValue(HashTable, "#t");
        }
        else return GetHashValue(HashTable, "#f");
    }
    else if(hashSymbol=="cons"){
        int newNode=allocate(NodeArray);
        NodeArray[newNode-1].setLchild(Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getRchild()-1].getLchild()));
        NodeArray[newNode-1].setRchild(Eval(NodeArray, HashTable, NodeArray[NodeArray[NodeArray[root-1].getRchild()-1].getRchild()-1].getLchild()));
        return newNode;
    }
    else if(hashSymbol=="cond"){
        while(NodeArray[NodeArray[root-1].getRchild()-1].getRchild()){
            root=NodeArray[root-1].getRchild();
            if(Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getLchild()-1].getLchild())==GetHashValue(HashTable,"#t")){
                return Eval(NodeArray, HashTable, NodeArray[NodeArray[NodeArray[root-1].getLchild()-1].getRchild()-1].getLchild());
            }
        }
        if(GetHashSymbol(HashTable,NodeArray[NodeArray[NodeArray[root-1].getRchild()-1].getLchild()-1].getLchild())!="else"){
            cout<<"error: there should be else!"<<endl;
            return 0;
        }
        return Eval(NodeArray, HashTable, NodeArray[NodeArray[NodeArray[NodeArray[root-1].getRchild()-1].getLchild()-1].getRchild()-1].getLchild());//첫 번째 getLchild없어도 되지 않나?
    }
    else if(hashSymbol=="car"){
        return NodeArray[Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getRchild()-1].getLchild())-1].getLchild();
    }
    else if(hashSymbol=="cdr"){
        return NodeArray[Eval(NodeArray, HashTable, NodeArray[NodeArray[root-1].getRchild()-1].getLchild())-1].getRchild();
    }
    else if(hashSymbol=="define"){
        //defining function
        if(GetHashSymbol(HashTable, NodeArray[NodeArray[NodeArray[NodeArray[root-1].getRchild()-1].getRchild()-1].getLchild()-1].getLchild())=="lambda"){
            int function_Body=NodeArray[NodeArray[NodeArray[root-1].getRchild()-1].getRchild()-1].getLchild();
            HashTable[-NodeArray[NodeArray[root-1].getRchild()-1].getLchild()].setHashElement_Link(function_Body);
        }

        //defining variable value
        else{
            int var_Value=Eval(NodeArray, HashTable, NodeArray[NodeArray[NodeArray[root-1].getRchild()-1].getRchild()-1].getLchild());
            HashTable[-NodeArray[NodeArray[root-1].getRchild()-1].getLchild()].setHashElement_Link(var_Value);
        }
        return root;
    }
    else if(hashSymbol=="quote"){
        return NodeArray[NodeArray[root-1].getRchild()-1].getLchild();
    }

    else if(IsDefinedFunc(NodeArray, HashTable, hashSymbol)){
        VariableStack VariableStack;
        
        //push current values to stack
        int pushedElem_Num=0;
        for(int i=0; i<101; i++){
            if(HashTable[i].getHashElement_Link()!=INT_MAX){
                VariableStack.push(i,HashTable[i].getHashElement_Link());
                pushedElem_Num++;
            }
        }

        //set parameter by function argument
        int HashValue=GetHashValue(HashTable, hashSymbol);
        int param=NodeArray[NodeArray[HashTable[-HashValue].getHashElement_Link()-1].getRchild()-1].getLchild();//함수의 parameter 리스트의 root 노드
        int arg=NodeArray[root-1].getRchild();//함수 call 의 변수에 대입할 argument의 root 노드
        while(true){//변수와 argument를 대응시키는 while 문
            int real_arg=Eval(NodeArray, HashTable, NodeArray[arg-1].getLchild());//실제 argument 노드(argument의 root 노드 아님!)
            while(real_arg<0 && HashTable[-real_arg].getHashElement_Link()>0 && HashTable[-real_arg].getHashElement_Link()!=INT_MAX){
                real_arg=Eval(NodeArray, HashTable, real_arg);
            }
            HashTable[-NodeArray[param-1].getLchild()].setHashElement_Link(real_arg);
            arg=NodeArray[arg-1].getRchild();
            param=NodeArray[param-1].getRchild();
            if(arg==0 || param==0){
                break;
            }
        }
        
        int function_body=HashTable[-NodeArray[root-1].getLchild()].getHashElement_Link();
        int result=Eval(NodeArray, HashTable, NodeArray[NodeArray[NodeArray[function_body-1].getRchild()-1].getRchild()-1].getLchild());

        for(int i=0; i<pushedElem_Num; i++){//pop the values from stack
            intPair popelem=VariableStack.pop();
            int Pairname=popelem.getName();
            int Pairlink=popelem.getLink();
            HashTable[Pairname].setHashElement_Link(Pairlink);
        }

        return result;
    }
    else{
        return root;
    }
}

int readForCodeSize(string str, int& tokenNum){
    int size=1;
    int root=1;
    int temp=1;
    bool first=true;
    bool firstChange=false;
    bool while_Statement_pass=false;
    stringVector dividedStr=divide(str);
    string hashSymbol=dividedStr.getVec()[tokenNum++];
    //GetNextToken(HashTable,dividedStr,tokenNum);
    if(hashSymbol=="("){
        hashSymbol=dividedStr.getVec()[tokenNum++];
        //GetNextToken(HashTable,dividedStr,tokenNum);
        while(hashSymbol!=")"){
            while_Statement_pass=true;
            if(first==true){
                //temp=allocate(NodeArray);
                //root=temp;
                first=false;
            }
            else{
                size++;
                //NodeArray[temp-1].setRchild(allocate(NodeArray));
                //temp=NodeArray[temp-1].getRchild();
            }
            if(hashSymbol=="("){
                //NodeArray[temp-1].setLchild(INT_MAX);
                //NodeArray[temp-1].setLchild(read(NodeArray,HashTable, str,--tokenNum));
                size+=readForCodeSize(str,--tokenNum);
            }
            else{
                //NodeArray[temp-1].setLchild(GetHashValue(HashTable, hashSymbol));
            }
            if(first==false){
                //NodeArray[temp-1].setRchild(0);
            }
            hashSymbol=dividedStr.getVec()[tokenNum++];
            //hashSymbol=GetNextToken(HashTable,dividedStr,tokenNum);
        }
        return size;
        // if(while_Statement_pass){
        //     return root;
        // }
        // else{
        //     return 0;
        // }
    }
    else{
        return size;
        // int hashValue=GetHashValue(HashTable, hashSymbol);
        // if(hashValue==INT_MAX){
        //     return 0;
        // }
        // return hashValue;
    }
}

// int dividedCodeSize(stringVector vec){
//     int size=0;
//     for(int i=0; i<vec.getSize(); i++){
//         if(vec.getVec()[i]!="(" && vec.getVec()[i]!=")" && vec.getVec()[i]!=" "){
//             size++;
//         }
//     }
//     return size;
// }

int availableNodeSize(NodeElement* NodeArray, HashElement* HashTable){
    int size=0;
    for(int i=1; i<=101; i++){
        if(i<101){
            if(NodeArray[i-1].getLchild()==0 && NodeArray[i-1].getRchild()==i+1){
                size++;
            }
        }
        else{
            if(NodeArray[i-1].getLchild()==0 && NodeArray[i-1].getRchild()==0){
                size++;
            }
        }
    }
    return size;
}

void NodeArrayCheck(NodeElement* NodeArray, int link, bool* used){
    used[link-1]=true;
    if(NodeArray[link-1].getLchild()>0){
        NodeArrayCheck(NodeArray, NodeArray[link-1].getLchild(), used);
    }
    if(NodeArray[link-1].getRchild()>0){
        NodeArrayCheck(NodeArray, NodeArray[link-1].getRchild(), used);
    }
}

void GarbageCollection(NodeElement* NodeArray, HashElement* HashTable){
    bool* used=new bool[101];
    for(int i=0; i<101; i++){
        used[i]=false;
    }
    for(int i=0; i<101; i++){
        int link=HashTable[i].getHashElement_Link();
        if(link>0 && link!=INT_MAX){
            NodeArrayCheck(NodeArray, link, used);
        }
    }
    for(int j=0; j<101; j++){
        if(!used[j]){
            NodeArray[j].setLchild(0);
            if(j!=100){
                NodeArray[j].setRchild(j+2);
            }
            else NodeArray[j].setRchild(0);
        }
    }
    cout<<"garbage collection is done!"<<endl;
}



/*
main 에서는 Hash table과 Node array의 초기화, 그리고 입력 받기 기능이 구현되어 있다. 무한 반복문을 이용해서 계속해서 scheme 명령어를 받을 수 있도록 하였다. NodeArray를 freelist에 반환하지 않도록 하여서 함수와 변수 선언이 
계속해서 유효하도록 설정하였다.
*/
int main(void){
    HashElement* HashTable=new HashElement[101];

    //HashTable 초기화
    for(int i=0; i<101; i++){
        HashTable[i].setHashElement_HashValue((-1)*i);
        if(i==0){
            HashTable[i].setHashElement_HashSymbol("()");
        }
    }

    //hashtable에 #t를 미리 넣어두는 작업
    SetHashValue(HashTable, "#t");

    //hashtable에 #f를 미리 넣어두는 작업
    SetHashValue(HashTable, "#f");

    //NodeArray 초기화
    NodeElement* NodeArray=new NodeElement[101];
    for(int i=0; i<101; i++){
        if(i==100){
            NodeArray[i]=NodeElement(0,0);
        }
        else{
            NodeArray[i]=NodeElement(0,i+2);
        }
    }

    while(true){
        //GarbageCollection(NodeArray, HashTable);
        string code;
        cout<<"> ";
        getline(cin, code);
        int tokenNum=0;
        string processedCode=Preprocessing(code, tokenNum);
        tokenNum=0;
        
        int codeSize=readForCodeSize(processedCode,tokenNum);
        tokenNum=0;
        if(codeSize>availableNodeSize(NodeArray, HashTable)){
            GarbageCollection(NodeArray, HashTable);
        }
        else{
            int root=read(NodeArray, HashTable, processedCode, tokenNum);
        
            //int freelistRoot=findFreelistRoot(NodeArray);
            //output(root, freelistRoot, NodeArray, HashTable);
            //cout<<endl;

            //Eval 이후
            int result=Eval(NodeArray, HashTable, root);
            if(GetHashSymbol(HashTable, NodeArray[result-1].getLchild())!="define"){
                print(result, true, true, NodeArray, HashTable);
            }
            cout<<endl;
        }
    }
}