#include <iostream>
#include <cstring>
#include <cmath>
#include <fstream>

using namespace std;


int encrypt(string msg, long int, long int);
int decrypt(int arr[] , long int, long int,int size);
long int modinv(long int,long int);
long int phiFinder(long int n);


int main(int argc, char* argv[]) {
    long int e,n;
    if(argc!=5){
        cout << "Not enough arguments, Try again!" << endl;
        exit(1);
    }
    e= atoi(argv[1]);
    n= atoi(argv[2]);





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


            encrypt(line,e,n);

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
        decrypt(array, e, n, size);
    }



    return 0;
}

int encrypt(string file, long int exp, long int mod)
//Encrypt an array of long ints
//exp and mod should be the public key pair
//Each number, c, is encrypted by
// c' = (c^exp)%mod
{
    cout<<"e="<<exp<<" n= "<<mod<<endl;
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
    ofstream out("cipher.txt");
    cout<<endl;
    for(int j=0;j<i;j++){
        int tmp=msg[j];
        for(int t=0;t<exp-1;t++){
            msg[j]= static_cast<int>(static_cast<long>(tmp*msg[j])%mod);
        }

        cout<<msg[j]<<" ";
       out<<msg[j]<<" ";
    }
    out.close();
    return 0;

}

int decrypt(int arr[], long int exp, long int mod,int size)
{
    long int phi=phiFinder(mod);
    long int d=modinv(exp,phi);
    cout<<"d is= "<<d<<endl;
    for(int j=0;j<size;j++){
        int tmp=arr[j];
        for(int t=0;t<d-1;t++){
            arr[j]= static_cast<int>(static_cast<long int>(arr[j]*tmp)% mod);
        }

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
long int modinv(long int e, long int phin)
{
    long int inv, u1, u3, v1, v3, t1, t3, q;
    long int iter;
    u1 = 1;
    u3 = e;
    v1 = 0;
    v3 = phin;
    iter = 1;
    while(v3 != 0)
    {
        q = u3 / v3;
        t3 = u3 % v3;
        t1 = u1 + q * v1;
        u1 = v1;
        v1 = t1;
        u3 = v3;
        v3 = t3;
        iter = -iter;
    }
    if(u3 != 1)
        return  0;
    if(iter < 0)
        inv = phin - u1;
    else
        inv = u1;
    return inv;
}
long int phiFinder(long int n)
{
    long int result = n; // Initialize result as n

    // Consider all prime factors of n and subtract their
    // multiples from result
    for (int p = 2; p * p <= n; ++p) {

        // Check if p is a prime factor.
        if (n % p == 0) {

            // If yes, then update n and result
            while (n % p == 0)
                n /= p;
            result -= result / p;
        }
    }

    // If n has a prime factor greater than sqrt(n)
    // (There can be at-most one such prime factor)
    if (n > 1)
        result -= result / n;
    return result;
}