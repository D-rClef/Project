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

void coding(LL p, LL g, LL y, LL *M, LL *&a, LL *&b, int n)
{
	for (int i = 0; i < n; i++)
	{
		LL k = LL((p - 4) * (1.0 * rand() / RAND_MAX) + 2);
		a[i] = powmod(g, k, p);
		b[i] = powmod(y, k, p);
		b[i] *= (M[i] % p);
		b[i] %= p;
	};
}

void decoding(LL *a, LL *b, LL p, LL x, LL *&M, int size_)
{
	for (int i = 0; i < size_; i++)
	{
		LL temp = powmod(a[i], (p-1-x) , p);
		temp = (temp * b[i]) % p;
	};
}

void code(char *m, LL *&M, int n, int size_, int iter)
{
    for(int i = 0; i < n; i++)
    {
        M[i] = (int)m[i];
    };
    LL* cop = new LL [size_];
    cop[0] = cop [1] = 32;
    int j = 2;
    for(int i = 0; i < n; i += iter)
    {
        int w = iter - 1;
        while(w)
        {
            if ((i + iter - w) < n)
            {
                M[i] *= 1000;
                M[i] += M[i + iter - w];
            };
            w--;
        };
        cop[j] = M[i];
        j++;
    };
    for(int t = j; t < size_; t++)
    {
        cop[t] = 32;
    };
    M = cop;

}

void decode(char *&m, LL *M, int size_, int iter, int n)
{
    LL* cop = new LL [n];
    int j = 0;
    for(int i = 0; i < size_; i++)
    {
        if (M[i] == 32) continue;
        int w = iter - 1;
        do
        {
            cop[j + w] = (M[i] % 1000);
            M[i] /= 1000;
            w--;
        } while((w+1));
        j += iter;
    };

    for(int i = 0; i < n; i++)
    {
        m[i] = (char)cop[i];
    };
    delete[]cop;
}

void ElGamal (LL &p, LL &g, LL &x, LL &y, string &str, int &n, int &size_, int& iter, char* &m, LL* &M, LL* &a, LL* &b, char* &a_, char* &b_)
{
    while(1)
    {
        cout << "What do you want?" << endl;
        cout << "1) To generate keys" << endl << "2) To cipher some text" << endl << "3) To decipher some text" << endl;
        int temp, kostyl = 0;
        cin >> temp;
        system("cls");
        if (temp == 1)
            {
                gen_keys(p, g, x, y);
                cout << "Your open keys:" << endl << "p = " << p << endl << "g = " << g << endl << "y = " << y << endl;
                cout << "Your closed key:" << endl << "x = " << x << endl;
                cout << "Do you want use it now? Enter '2' if you want use it to cipher or '3' if you want decipher some text" << endl;
                cin >> temp;
                if (temp == 2)
                    kostyl = 1;
                if (temp == 3)
                    kostyl = 2;
            };
        if (temp == 2)
            {
                if (kostyl != 1)
                {
                    cout << "Please, enter your open keys (p, g, y):" << endl;
                    cin >> p >> g >> y;
                };
                cout << "Enter your text:" << endl;

                cin.ignore();
                getline(cin, str);
                n = str.length();
                m = new char[n];
                for (int i = 0; i < n; i++)
                {
                    m[i] = str[i];
                };
                iter = floor(log_b(p, 10))/3;
                size_ = ceil(1.0*n/iter);
                size_ += 4;
                if (size_ & 1) size_++;

                M = new LL[n];
                a = new LL[size_];
                b = new LL[size_];

             	code(m, M, n, size_, iter);
                coding(p, g, y, M, a, b, size_);

                /*a_ = new char[n];
                b_ = new char[n];

                decode(a_, a, size_, iter, n);
                decode(b_, b, size_, iter, n);
*/
                cout << "This is your cipher: " << endl;
                cout << "a:  ";
                for(int i = 0; i < size_; i++)
                {
                    cout << a_[i] << " ";
                };

                cout << endl;
                cout << "b:  ";
                for(int i = 0; i < size_; i++)
                {
                    cout << b[i] << " ";
                };
                cout << endl;
                cout << "If you want to check is deciphering correctly, enter '3'" << endl;
                cin >> temp;
                if(temp == 3) kostyl = 3;
            };
        if (temp == 3)
            {
                if ((kostyl != 2) && (kostyl != 3))
                {
                    cout << "OK, now enter your closed key (x) and p from open key (if you don't know x, I can't help you):" << endl;
                    cin >> x >> p;
                };
                iter = floor(log_b(p, 10))/3;

                if (kostyl != 3)
                {
                    cout << "Enter number of elements of cipher a:" << endl;
                    cin >> size_;
                    n = size_*iter;

                    a = new LL [size_];
                    b = new LL [size_];
                    M = new LL [n];
                    m = new char [n];
                    cout << "Enter a: " << endl;
                    for(int i = 0; i < size_; i++)
                    {
                        cin >> a[i];
                    };
                    cout << "Enter b:" << endl;
                    for(int i = 0; i < size_; i++)
                    {
                        cin >> b[i];
                    };
                };
                decoding(a, b, p, x, M, n);
                decode(m, M, size_, iter, n);

                cout << "This is your text:" << endl;
                for(int i = 0; i < n; i++)
                    cout << m[i];
                cout << endl;
            };
        cout << "If you don't want continue, click esc";
        int d_ = getch();
        if(d_ == 27)
            {system("cls"); break;}
        else
            system("cls");
    };
}

int main()
{
	srand(time(NULL));

	LL p, g, x, y;
	string str;
	int n, size_, iter;
	char *m, *a_, *b_;
	LL *a, *b, *M;
	ElGamal(p, g, x, y, str, n, size_, iter, m, M, a, b, a_, b_);

	return 0;
}
