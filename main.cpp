#include <iostream>
#include <cstring>
#include <cmath>
#include <fstream>

using namespace std;

bool prime(long int prime);
long int gcd(long int p, long int q);
int publickey(long int p, long int q, long int* exp, long int* mod);
int privatekey(long int p, long int q, long int pubexp, long int* exp, long int* mod);
int encrypt(string msg, long int, long int);
int decrypt(int arr[] , long int, long int,int size);
int char2long(char* in, long int* out);
int long2char(long int* in, char* out);


int main(int argc, char* argv[]) {
    long int p,q, pube, pubmod, prive, privmod;
    int msg[100];
    if(argc!=5){
        cout << "Not enough arguments, Try again!" << endl;
        exit(1);
    }
    p= atoi(argv[1]);
    q= atoi(argv[2]);
    if(!prime(p) || !prime(q)){
        cout << "Not a prime number. Try again!" << endl;
        exit(1);
    }
    publickey(p,q,&pube,&pubmod);
    std::cout << "public key: " << pube << ", " << pubmod << std::endl;
    privatekey(p,q,pube,&prive,&privmod);
    std::cout << "private key: " << prive << ", " << privmod << std::endl;


    if(strcmp(argv[3],"e")==0) {
        cout << "Entering Encryption zone!" << endl;
        ifstream inFile;
        inFile.open(argv[4]);
        if (!inFile) {
            cerr << "Unable to open file!";
            exit(1);
        }
        string line;

        getline(inFile, line, '\0');
        int msglen=strlen(line.c_str());
        cout<<"size =" <<msglen<<endl;
            cout << "Original text: " << line << std::endl;


            encrypt(line,pube,pubmod);

    }
    if(strcmp(argv[3],"d")==0) {
        cout << "Entering Decryption zone!" << endl;
        ifstream inFile;
        inFile.open(argv[4]);
        if (!inFile) {
            cerr << "Unable to open file!";
            exit(1);
        }
        int array[100];
        int size=0;
        while(size<100 && inFile>>array[size]){
            ++size;
        }
        decrypt(array,prive,privmod,size);
    }



    return 0;
}

bool prime(long int p) {
    long int j = sqrt(p);
    for (long int z=2;z<j;z++) if (0==p%z) return false;
    return true;
}
int publickey(long int p, long int q, long int *exp, long int *mod)
//Generates a public key pair
//The modulus is given by (p-1)*(q-1)
//The exponent is any integer coprime of the modulus
{

    *mod = (p-1)*(q-1);
    //Choose an integer near sqrt(mod)
    *exp = (int)sqrt(*mod);
    //Find a coprime near that number
    while (1!=gcd(*exp,*mod))
    {
        (*exp)++;
    }
    *mod = p*q;
    return 0;
}
int privatekey(long int p, long int q, long int pubexp, long int *exp, long int *mod)
//Generates a private key pair
//The modulus is given by (p-1)*(q-1)
//The exponent is the number, n, which satisfies (n * pubexp) % mod = 1
{
    *mod = (p-1)*(q-1);
    *exp = 1;
    long int tmp=pubexp;
    while(1!=tmp%*mod)
    {
        tmp+=pubexp;
        tmp%=*mod; //We can exploit the fact that (a*b)%c = ((a%c)*b)%c
        //   to keep the numbers from getting too large
        (*exp)++;
    }
    *mod = p*q;
    return 0;
}
long int gcd(long int p, long int q)
//greatest common devisor (AKA greatest common factor)
// by Euclid's method
{
    if (p<q) {long int tmp=p;p=q;q=tmp;}
    while (q!=0)
    {
        //In each step the new p is the old q and the new q is p%q
        //     p <- q
        //     q <- p%q
        //The last modular remainder will be 0.
        //The next to last modular remainder is the GCD.
        long int tmp = q;
        q = p%q;
        p = tmp;
    }

    return p;
}
int encrypt(string file, long int exp, long int mod)
//Encrypt an array of long ints
//exp and mod should be the public key pair
//Each number, c, is encrypted by
// c' = (c^exp)%mod
{
    cout<<"e="<<exp<<" mod= "<<mod<<endl;
    int msg[100];
    int i=0;
    for(char ascii : file){
        if(ascii!=' ') {
            msg[i]=(int) ascii - 63;
            std::cout << msg[i] << " ";
            i++;
        }
        else{
            msg[i]=28;
            std::cout << msg[i] << " ";
            i++;
        }
    }
    ofstream out("output.txt");
    cout<<endl;
    for(int j=0;j<i;j++){
        msg[j]= static_cast<int>(static_cast<long int>(pow(msg[j],exp))% mod);
        cout<<msg[j]<<" ";
       out<<msg[j]<<" ";
    }
    out.close();
    return 0;

}

int decrypt(int arr[], long int exp, long int mod,int size)
//Decrypt an array of long ints
//exp and mod should be the private key pair
//Each number, c', is decrypted by
// c = (c'^exp)%mod
{
    for(int j=0;j<size;j++){
        arr[j]= static_cast<int>(static_cast<long int>(pow(arr[j],exp))% mod);
        cout<<arr[j]<<" ";

    }
    cout<<endl;
    ofstream out("decrypt.txt");
    for(int j=0;j<size;j++){
        if(arr[j]!=28)
            out<< static_cast<char>(arr[j]+63);
        else
            out<<" ";
    }
    return 0;
}