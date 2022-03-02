#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <thread>
#include <mutex>
#include <time.h>

std::mutex m;
std::mutex m2;

//////// exercise 1

class pcout {
private:
	std::lock_guard<std::mutex> lg1;
public:
	pcout() : lg1(std::lock_guard<std::mutex>(m2))
	{
	}
	template<typename T>
	pcout& operator<<(const T& x)
	{
		std::cout << x;
		return *this;
	}

	pcout& operator<<(std::ostream& (*fp)(std::ostream&))
	{
		std::cout << fp;
		return *this;
	}
};

void ex_func(int x) {
	pcout() << "start thread" << x << std::endl;
	pcout() << "stop thread" << x << std::endl;
}

/////////

/////////// exercise 2

bool prime_check(int num) {
	if (num > 2)
	{
		for (size_t i = 2; i < num; i++)
		{
			if (num%i == 0)
			{
				return false;
			}
		}
	}
	return true;
}

void prime_det(int number) {
	int number_count = number;
	long int x = 1;
	while (number != 0)
	{
		if (prime_check(x))
		{
			number--;
			if (number != 0)
			{
				std::cout << 100-(100.0 * number / number_count) << "%" << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(3));
				system("cls");
			}
		}
		x++;
	}                         
	x--;
	std::cout << x << std::endl;
}

///////////

///////// exercise 3

void add_number_vect(std::vector<int>& prod_add) {
	std::lock_guard lg(m);
	srand(time(NULL));
	std::this_thread::sleep_for(std::chrono::seconds(1));
	int a = rand() % 100;
	prod_add.push_back(a);
	std::cout << "added " << a << std::endl;
}

void del_number_vect(std::vector<int>& prod_del) {
	std::lock_guard lg(m);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	auto it = max_element(prod_del.begin(), prod_del.end());
	std::cout << "deleted " << *it << std::endl;
	prod_del.erase(it);
}

////////


int main()
{
	///////// exercise 1

	std::thread th1(ex_func, 1);
	std::thread th2(ex_func, 2);
	std::thread th3(ex_func, 3);
	th1.join();
	th2.join();
	th3.join();

	/////////

	///////// exercise 2

	int number = 300;
	std::thread prime_det_thr(prime_det, number);
	prime_det_thr.join();

	//////////

	///////// exercise 3

	std::vector<int> prod = { 15,20 };
	for (size_t i = 0; i < 100; i++)
	{
		std::thread owner([&]() {add_number_vect(prod); });
		std::thread thief([&]() {del_number_vect(prod); });
		owner.join();
		thief.join();
	}

	//////////

    system("pause");
    return 0;
}

