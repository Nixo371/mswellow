#include <iostream>
#include <vector>
#include <string>
#include <random>


class Egg {
	public:
		Egg(double rarity, std::mt19937 gen) {
			this->rarity = rarity;
			this->gen = gen;
			this->dis = std::uniform_real_distribution<>(0.0, 1.0);
		}

		bool /*rick*/rolled() {
			double roll = this->dis(gen);
			return (roll < rarity);
		}

		// Never actually used :/
		int get_value() {
			// Here we multiply by 1m to get the value in the real game
			return ((int)(1 / rarity) * 1000000);
		}

	private:
		double rarity;
		std::mt19937 gen;
		std::uniform_real_distribution<> dis;
};

class EggList {
	public:
		EggList(std::mt19937 gen) {
			eggs = std::vector<Egg>();
			// For simplicity, we will only use 1b or higher, and assume that everything is divided by 1m
			// to improve performance (because we know that the real game would do something similar)
			eggs.push_back(Egg((double)1 / 1000, gen));		// 1 in 1 billion
			eggs.push_back(Egg((double)1 / 10000, gen));	// 1 in 10 billion
			eggs.push_back(Egg((double)1 / 100000, gen));	// 1 in 100 billion
			eggs.push_back(Egg((double)1 / 1000000, gen));	// 1 in 1 trillion
			eggs.push_back(Egg((double)1 / 5000000, gen));	// 1 in 5 trillion
			eggs.push_back(Egg((double)1 / 10000000, gen));	// 1 in 10 trillion
		}

		int roll() {
			for (int i = eggs.size() - 1; i >= 0; i--) {
				if (eggs[i].rolled()) {
					return (i);
				}
			}
			return (0);
		}

	private:
		std::vector<Egg> eggs;
};

double round(double num, int places) {
	for (int i = 0; i < places; i++) {
		num *= 10;
	}
	double rounded = (int)num;
	for (int i = 0; i < places; i++) {
		rounded /= 10;
	}
	return (rounded);
}

int main() {
	// This is how we randomize values in the range [0, 1) to simulate the random rolls
	// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
	std::random_device rd;
    std::mt19937 gen(rd());

	EggList egg_list = EggList(gen);

	int rolls_per_power = 7000;		// Again, we assume we divide everything by 1m, so -> 7b / 1m = 7000
	int total_sauce = 168000000;	// 168t / 1m = 168000000 
	int sauce_used = 0;

	std::vector<int> egg_counts = std::vector<int>(7, 0);

	while (sauce_used < total_sauce) {
		int best_egg = 0;
		for (int i = 0; i < rolls_per_power; i++) {
			int roll = egg_list.roll();
			if (roll > best_egg) {
				if (best_egg == 3) {
					std::cout << "1t overwritten!" << std::endl;
					egg_counts[6]++;
				}
				best_egg = roll;
			}

			if (best_egg == 5) { // This is the rarest egg in our simulation so no need to continue
				break;
			}
		}

		egg_counts[best_egg]++;

		sauce_used += rolls_per_power;
		std::cout << "Sauce used: " << sauce_used << "/" << total_sauce << "(" << round(((double)sauce_used / total_sauce) * 100, 2) << "%)" << std::endl;
	}

	std::cout << "Egg counts: " << std::endl;
	
	std::cout << "1b: " << egg_counts[0] << std::endl;
	std::cout << "10b: " << egg_counts[1] << std::endl;
	std::cout << "100b: " << egg_counts[2] << std::endl;
	std::cout << "1t: " << egg_counts[3] << std::endl;
	std::cout << "5t: " << egg_counts[4] << std::endl;
	std::cout << "10t: " << egg_counts[5] << std::endl;

	std::cout << "1t (overwritten): " << egg_counts[6] << std::endl;
}