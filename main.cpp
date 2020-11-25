#include <iostream>
#include <random>
#include <chrono>
#include <sstream>

int read_int(const char * question){
		int result;
	re:	std::string inp;
		std::cout << question;
		std::getline (std::cin, inp);
		{
			std::stringstream inps {inp};
			inps >> result;
			if (inps.fail()){
				if (std::cin.fail()){
					std::cerr << "Оишбка ввода-вывода!" << std::endl;
					exit(1);
				}
				std::cout << "Некорректный ввод!" << std::endl;
				goto re;
			} else if(not inps.eof()) {
				std::cout << "Может быть число должно быть целым? Умник..." << std::endl;
				goto re;
			}
		}
		return result;
}

int read_int_in_range(const char * question, int min, int max){ // @suppress("No return")
	int result;
	for(;;) {
		result = read_int(question);
		if (result >= min and result <= max)
			return result;
		std::cout << "Число должно быть в интервале от " << min << " до " << max << std::endl;
	}
}

int main(){
	using clk = std::chrono::system_clock;
	std::minstd_rand rnd{
		unsigned(clk::now().time_since_epoch().count())
	};
	std::uniform_int_distribution <> d{15,25};
	std::uniform_int_distribution <> db{1,3};
	std::uniform_int_distribution <> randbu{1,2};
	int level = 0;

menu:
	unsigned stone_count = d(rnd);
	int menu = read_int_in_range("--------| Меню |--------\n(1) Начать игру\n(2) Уровень сложности\n(3) Выход\n--> ", 1, 3);
	switch(menu){
		case 1: if (level == 0) level = 1; break;
		case 2: level = read_int_in_range("Выберите уровень сложности:\n1) Легкий\n2) Средний\n3) Сложный\n--> ", 1, 3); goto menu; break;
		case 3: std::cout << "Игра завершена!" << std::endl; return 0;
	}

	std::cout << "\nКол-во камешков - " << stone_count << std::endl;

	unsigned rbu =  randbu(rnd);
	if(rbu == 1){
		std::cout << "Первый ходит пользователь!\n" << std::endl;
	} else std::cout << "Первый ходит бот!\n" << std::endl;

	do{
		unsigned count;
		if (rbu == 1){
			count = read_int_in_range("Введите кол-во камешков: ", 1, std::min(3u, stone_count));
			stone_count -= count;

			if (stone_count == 0){
				std::cout << "\nБОТ ВЫИГРАЛ\n" << std::endl;
				goto menu;
				break;
			}
			std::cout << "Кол-во камешков - " << stone_count << std::endl;

			rbu = 2;
		}

		if (rbu == 2){
			//Уровни сложности
			unsigned bot_count;
			if(stone_count < 5 && level != 1){
				switch(stone_count){
					case 4: bot_count = 3; break;
					case 3: bot_count = 2; break;
					case 2: bot_count = 1; break;
				}
			} else {
				if (level == 1){
					bot_count = db(rnd);
				} else if (level == 2){
					switch(count){
						case 1: bot_count = 3; break;
						case 2: bot_count = 2; break;
						case 3: bot_count = 1; break;
						default: bot_count = db(rnd); break;
					}
				} else {
					for(unsigned i = 25; i > 0; i -= 4){
						if (i == stone_count){
							bot_count = db(rnd);
							break;
						}
						if (stone_count > i){
							bot_count = stone_count - i;
							break;
						}
					}
				}
			}

			stone_count -= bot_count;
			std::cout << "Бот взял " << bot_count << " камешка" << std::endl;

			if (stone_count == 0 || stone_count > 25){
				std::cout << "\nПОЛЬЗОВАТЕЛЬ ВЫИГРАЛ\n" << std::endl;
				goto menu;
				break;
			}
			std::cout << "Кол-во камешков - " << stone_count << std::endl;

			rbu = 1;
		}
	} while (true);

	return 0;
}

// 1 5 9 13 17 21 25
