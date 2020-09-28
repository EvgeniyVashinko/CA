//F[i]=A[i]*C[i]+B[i]*D[i] , i=1...8;

#include <iostream>
using namespace std;

int toDec(string arr, int size);
void StringToDec(string str, int size);
string toBinary(__int64 num, int size);
void GetResult(int8_t* a, int8_t* b, int8_t* c, int16_t* d);

int main()
{
    int8_t A[8] = { -1, -2, -3, 4, 5, 6, 7, 8 };
    int8_t B[8] = { -8, 7, 6, 5, 4, 3, 2, 1 };
    int8_t C[8] = { 2, 2, 2, 2, 2, 2, 2, 2 };
    int16_t D[8] = { 10, 20, 30, 40, 50, 60, 70, 80 };

    __int64 m64_1 = 0;
    __int64 m64_2 = 0;
    __int64 m64_3 = 0;
    __int64 m64_4 = 0;
    __int64 m64_5 = 0;
    __int64 m64_6 = 0;
    __int64 m64_7 = 0;
    __int64 m64_8 = 0;

    __asm 
    {
        //loaded À
        movq mm0, A
        movq mm1, mm0
        movq m64_7, mm0

        pxor mm2, mm2
        pcmpgtb mm2, mm0
        movq m64_8, mm2

        punpckhbw mm0,mm2
        punpcklbw mm1, mm2

        movq m64_1, mm0
        movq m64_2, mm1
        
        //loaded Ñ
        lea esi, C
        movq mm3, [esi]
        movq mm4, mm3

        pxor mm2, mm2
        pcmpgtb mm2, mm3//
        punpckhbw mm3, mm2
        punpcklbw mm4, mm2

        movq m64_3, mm3
        movq m64_4, mm4

        //multiply
        pmullw mm4, mm1 
        movq m64_5, mm4

        movq mm5, mm4//A2*C2

        pmullw mm3, mm0
        movq m64_6, mm3

        movq mm6, mm3 // A1*C1
    }
    string s;
    //output
    {
        cout << "-----A------:" << endl;
        s = toBinary(m64_7, 8);
        cout << s << endl;
        StringToDec(s, 8);
        cout << endl;

        cout << "------<<<<-----:" << endl;
        s = toBinary(m64_8, 8);
        cout << s << endl;
        StringToDec(s, 8);
        cout << endl;

        cout << "A1:" << endl;
        s = toBinary(m64_1, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl;

        cout << "A2:" << endl;
        s = toBinary(m64_2, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl;

        cout << "C1:" << endl;
        s = toBinary(m64_3, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl;

        cout << "C2:" << endl;
        s = toBinary(m64_4, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl;

        cout << "A2*C2:" << endl;
        s = toBinary(m64_5, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl;

        cout << "A1*C1:" << endl;
        s = toBinary(m64_6, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl; 
    }

    __int64 res1;
    __int64 res2;
    __asm
    {
        lea esi, B
        movq mm1, [esi]
        movq mm2, mm1

        pxor mm0, mm0

        pcmpgtb mm0, mm1
        punpckhbw mm1, mm0

        punpcklbw mm2, mm0

        movq m64_1, mm1
        movq m64_2, mm2

        lea esi, D
        movq mm3, [esi+8]
        movq m64_3, mm3
        movq mm4, [esi]
        movq m64_4, mm4

        // multiply
        pmullw mm1, mm3
        movq m64_5, mm1

        pmullw mm2, mm4
        movq m64_6, mm2

        //mm6 - ac1, mm5 ac2, mm1 - bd1, mm2 - bd2
        movq m64_7, mm5

        paddsw mm6, mm1
        paddsw mm5, mm2

        movq res1, mm6
        movq res2, mm5
    }
    //output
    {
        cout << "B1:" << endl;
        s = toBinary(m64_1, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl;

        cout << "B2:" << endl;
        s = toBinary(m64_2, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl;

        cout << "D1:" << endl;
        s = toBinary(m64_3, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl;

        cout << "D2:" << endl;
        s = toBinary(m64_4, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl;

        cout << "B1*D1:" << endl;
        s = toBinary(m64_5, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl;

        cout << "B2*D2:" << endl;
        s = toBinary(m64_6, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl;

        cout << "res1:" << endl;
        s = toBinary(res1, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl;

        cout << "res2:" << endl;
        s = toBinary(res2, 16);
        cout << s << endl;
        StringToDec(s, 16);
        cout << endl; 
    }

    cout << endl;
    cout << "||||||||||" << endl;
    GetResult(A, B, C, D);
}

int toDec(string arr, int size) // Converting to decimal
{
    int result = 0;
    if (arr[0] == '0')
    {
        for (int i = size - 1; i >= 0; i--)
        {
            if (arr[i] == '1')
            {
                result += pow(2, size - i - 1);
            }
        }
    }
    else
    {
        for (int i = size - 1; i >= 0; i--)
        {
            if (arr[i] == '0')
            {
                result += pow(2, size - i - 1);
            }
        }
        result = -(result + 1);
    }

    return result;
}

void StringToDec(string str, int size)
{
    int s = (int)(64 / size);
    string s1;
    toDec("00001001", 8);
    for (int i = 0; i < s; i++)
    {
        s1.assign(str, i * (size + 1), size);
        cout << toDec(s1, size) << "  ";
    }
}

string toBinary(__int64 num, int size) // Converting to binary
{
    int ns = (int)(64 / size);
    string res = "0000000000000000000000000000000000000000000000000000000000000000";
    int i = 63;
    while (num != 0)
    {
        if (num % 2 == 1)
        {
            res[i] = '1';
        }

        num /= 2;
        i--;
    }
    for (int i = 1; i < ns; i++)
    {
        res.insert(i * size + (i - 1), " ");
    }
    return res;
}

void GetResult(int8_t* a, int8_t* b, int8_t* c, int16_t* d) {
    for (int i = 7; i >= 0; i--)
    {
        cout << a[i] * c[i] + b[i] * d[i] << "   ";
    }
}

