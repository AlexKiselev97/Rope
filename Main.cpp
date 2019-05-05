#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Rope.h"

using namespace std;

int main()
{
	/* example program
	read string
	then N reordering operations 
	*/
	Rope rope;
	char c;
	while ((c = cin.get()) != '\n')
	{
		rope.push_back(c);
	}
	int n, i, j, k;
	cin >> n;
	for (int it = 0; it < n; ++it)
	{
		cin >> i >> j >> k;
		rope.reorder(i, j, k);
	}
	cout << rope.getString() << endl;
	return 0;
}