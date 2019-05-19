#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <conio.h>
using namespace std;

typedef unsigned long long LL;

int log_b(LL a, LL b)
{
	int res = log(a) / log(b);
	return res;
}

LL powmod(LL a, LL b, LL p)
{
	LL res = 1;
	while (b)
		if (b & 1)
			res = LL(res * 1ll * a % p), --b;
		else
			a = LL(a * 1ll * a % p), b >>= 1;
	return res;
}

bool test_M_R(LL n, int num_iters)
{
	LL t = n - 1, s = 0;
	while ((t % 2) == 0)
	{
		t /= 2;
		s++;
	};
	t = (n - 1) / pow(2, s);

	for (int i = num_iters; i != 0; i--)
	{
		LL a = LL((n - 2) * (1.0 * rand() / RAND_MAX) + 2);
		LL x = powmod(a, t, n);
		if ((x == 1) || (x == (n - 1)))
			continue;
		LL dop_iters = s - 1;
		while (dop_iters != 0)
		{
			x = powmod(x, 2, n);
			if (x == 1)
				return false;
			if (x == (n - 1))
				break;
			dop_iters--;
		};
		if (x == (n - 1))
			continue;
		return false;
	};
	return true;
}

LL prim_root(LL p)
{
	vector <LL> fact;
	LL phi = p - 1, n = phi;
	for (LL i = 2; i * i <= n; ++i)
		if (n % i == 0)
		{
			fact.push_back(i);
			while (n % i == 0)
				n /= i;
		}
	if (n > 1)
		fact.push_back(n);

	for (LL res = 2; res <= p; ++res)
	{
		bool ok = true;
		for (size_t i = 0; i < fact.size() && ok; ++i)
			ok &= (powmod(res, phi / fact[i], p) != 1);
		if (ok)
			return res;
	}
	return -1;
}

void gen_keys(LL &p, LL &g, LL &x, LL &y)
{
	LL p_;
	bool *a = new bool[256];
	for (int i = 0; i < 256; i++)
		a[i] = 1;
	a[0] = a[1] = 0;
	for (int i = 2; i < 256; i++)
	{
		if (a[i])
		{
			for (int j = i + i; j < 256; j += i)
			{
				a[j] = 0;
			};
		};
	};

	int ok = 1;
	while (ok)
	{
		ok = 1;

		p_ = rand() * 1000000000 + 999999999;
		for (int i = 2; i < 256; i++)
		{
			if (a[i])
			{
				if ((p_ % i) == 0)
				{
					ok = 2;
				};
			};

			if (ok == 2)
				break;
		};

		if (ok == 2)
			continue;
		if (test_M_R(p_, log_b(p_, 2)))
		{
			ok = 0;
		};
		if (!ok)
			break;
	};

	delete[] a;

	p = p_;
	g = prim_root(p);
	x = LL((p - 4) * (1.0 * rand() / RAND_MAX) + 2);
	y = powmod(g, x, p);
}

void coding(LL p, LL g, LL y, int *M, LL *&a, LL *&b, int n)
{
	for (int i = 0; i < n; i++)
	{
		LL k = LL((p - 4) * (1.0 * rand() / RAND_MAX) + 2);
		a[i] = powmod(g, k, p);
		b[i] = powmod(y, k, p);
		b[i] = (b[i] * M[i]) % p;
	};
}

void decoding(LL *a, LL *b, LL p, LL x, int *&M, int n)
{
	for (int i = 0; i < n; i++)
	{
		M[i] = powmod(a[i], (p - 1 - x), p);
		M[i] = (M[i] * b[i]) % p;
	};
}

void code(char *m, int *&M, int n)
{
    int j = 0;
	for (int i = 0; i < n; i += 3)
	{
		int a = m[i] + '0', b = 0, c = 0;
		if ((i+1) < n)
            int b = m[i+1] + '0';
		if ((i+2) < n)
            int c = m[i+2] + '0';
		M[j] = a*1000000 + b*1000 + c;
		j++;
	};
}

void decode(char *&m, int *M, int s)
{
    int j = 0;
	for (int i = 0; i < s; i++)
	{
		m[j+2] = (M[i] % 1000) - '0';
		M[i] /= 1000;
		m[j+1] = (M[i] % 1000) - '0';
		M[i] /= 1000;
		m[j] = M[i] - '0';
		j++;
	};
}

/*void ElGamal (LL &p, LL &g, LL &x, LL &y, string &str, int &n, char* &m, int &size_, int* &M, LL* &a, LL* &b)
{
    while(1)
    {
        cout << "What do you want?" << endl;
        cout << "1) To generate keys" << endl << "2) To cipher some text" << endl << "3) To decipher some text";
        int temp;
        cin >> temp;
        system("cls");
        if (temp == 1)
            {
                gen_keys(p, g, x, y);
                cout << "Your open keys:" << endl << "p = " << p << endl << "g = " << g << endl << "y = " << y << endl;
                cout << "Your closed key:" << endl << "x = " << x << endl;
            }
        else if (temp == 2)
            {
                cout << "Please, enter your open keys (p, g, y):" << endl;
                cin >> p >> g >> y;
                cout << "Enter your text:";

                cin.ignore();
                getline(cin, str);
                n = str.length();
                cout << n << " ";
                m = new char[n];
                for (int i = 0; i < n; i++)
                {
                    m[i] = str[i];
                };
                size_ = floor(log(p));
                size_ /= 3;
                int temp = size_;
                size_ = (n/temp) + 1;
                cout << size_ << endl;

                M = new int[size_];
                a = new LL[size_];
                b = new LL[size_];

                code(m, M, n);
                coding(p, g, y, M, a, b, n);

                cout << "This is your cipher: " << endl;
                cout << "a:  ";
                for(int i = 0; i < size_; i++)
                {
                    cout << a[i] << " ";
                };

                cout << endl;
                cout << "b:  ";
                for(int i = 0; i < size_; i++)
                {
                    cout << b[i] << " ";
                };
            }
        else if (temp == 3)
            {
                cout << "Please, enter your cipher (a (when you will finish, click enter), then b (similarly):" << endl;
                int i = 0, j = 0;
                while(1)
                {
                    cin >> a[i];
                    i++, j++;
                    int d = getch();
                    if (d == 13) break;
                };
                i = 0;
                while(1)
                {
                    cin >> a[i];
                    i++;
                    int d = getch();
                    if (d == 13) break;
                };

                cout << "OK, now enter your closed key (x) and p from open key (if you don't know x, I can't help you):";
                cin >> x >> p;
                n = (j - 1)*floor(log(p));
                decoding(a, b, p, x, M, n);
                decode(m, M, j);

                cout << "This is your text:";
                for(int w = 0; w < n; w++)
                    cout << m[i];
            };
        cout << "If you don't want continue, click esc";
        int d_ = getch();
        if(d_ == 27)
            {system("cls"); break;}
        else
            system("cls");
    };
}*/

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");

	/*LL p, g, x, y;
	string str;
	int n, size_;
	char* m;
	int* M;
	LL *a, *b;
	ElGamal(p, g, x, y, str, n, m, size_, M, a, b);*/

	LL p, g, x, y;
	gen_keys(p, g, x, y);
	cout << p << endl << g << endl << x << endl << y << endl;

	string str;
	getline(cin, str);
	int n = str.length();
	char *m = new char[n];
	for (int i = 0; i < n; i++)
	{
		m[i] = str[i];
	};
	int size_ = floor(log(p))/3;
	size_ = (n/size_) + 1;

	int *M = new int[size_];
	LL *a = new LL[size_];
	LL *b = new LL[size_];

	code(m, M, n);
	coding(p, g, y, M, a, b, n);

	for(int i = 0; i < size_; i++)
    {
        cout << a[i] << "  " << b[i] << endl;
    };

	decoding(a, b, p, x, M, n);
	decode(m, M, size_);

	for(int i = 0; i < n; i++)
        cout << m[i] ;

    return 0;
}
