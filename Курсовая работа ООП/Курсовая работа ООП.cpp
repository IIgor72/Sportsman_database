/*------------------------------Подключение библиотек------------------------------*/
#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>
#include <fstream>
#include <conio.h>
using namespace std;
/*------------------------------Описание структур------------------------------*/
struct energyReport {
	int orderNumber;					//порядковый номер
	int factoryNumber;					//номер завода
	string directorFullName;			//ФИО директора
	string chiefEngineerFullName;		//ФИО главного энергетика
	float plannedEnergyConsumption;		//расход энергии по плану (в тыс. КВт∙ч)
	float actualEnergyConsumption;		//израсходовано фактически (в тыс. КВт∙ч)												
};
struct list {
	struct energyReport inf;
	struct list* next, * previous;
};

float deviationSize;				//размер отклонения факта от плана(со знаком)
float totalValuesPerPosition;		//суммарные значения по каждой позиции (для отрасли в целом)

/*------------------------------Шаблоны для меню------------------------------*/
const string menuOptions1[] = {
		"Создать список",
		"Вывести на экран",
		"Добавить новый элемент",
		"Удалить элемент",
		"Сохранить список в файл",
		"Загрузить список из файла",
		"Очистить память",
		"Корректировка записи",
		"Сортировка списка",
		"Поиск в списке",

		"Завершить работу"
};
const int N1 = 11; //количество элементов первого варианта меню
//const string menuOptions2[] = { "Вариант 1", "Вариант 2", "Вариант 3", "Выход из программы" };
//const int N2 = ; //количество элементов второго варианта меню
//const string menuOptions3[] = { "Вариант 1", "Вариант 2", "Вариант 3", "Выход из программы" };
//const int N3 = ; //количество элементов третьего варианта меню


/*------------------------------Определение функций------------------------------*/
energyReport InputEnergyReport(int& index);
list* AddFirstField(list* begin, list* _temp);
list* AddLastField(list* begin, list* _temp);
list* AddField(list* begin, int& index);
list* DeleteField(list* begin);
list* CreateList(list* begin, int& index);
void PrintList(list* begin, int _search);
void SaveListToFile(list* begin);
list* DownloadListFromFile(list* begin, int& index);
void DeleteList(list*& begin);
list* CorrectField(list* begin, int modifiedElementIndex);
list* SortingList(list* begin);
void SearchInList(list* begin);

int main() {
	/*------------------------------Настройка окна------------------------------*/
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	SetConsoleTitle(L"Курсовой проект");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	system("CLS");
	system("color E0");
	/*------------------------------Настройка размера шрифта------------------------------*/
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 24;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	//Изъятие дескриптора потока вывода
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	int index = 1;					//Нумерация записей
	int choiceOperation;			//Выбор пользователем операции
	bool programIsOpen = true;		//Условие работы программы
	struct energyReport X;
	struct list* begin;
	begin = NULL;
	//begin->previous = NULL;

	while (programIsOpen) {
		cout << "==============================" << endl;
		cout << "1 - Создать список;" << endl;
		cout << "2 - Вывести на экран;" << endl;
		cout << "3 - Добавить новый элемент;" << endl;
		cout << "4 - Удалить элемент;" << endl;
		cout << "5 - Сохранить список в файл;" << endl;
		cout << "6 - Загрузить список из файла;" << endl;
		cout << "7 - Очистить память;" << endl;
		cout << "8 - Корректировка записи;" << endl;
		cout << "9 - Сортировка списка;" << endl;
		cout << "10 - Поиск в списке;" << endl;

		cout << "0 - Завершить работу." << endl;
		cout << "==============================" << endl;
		cout << "Выберите операцию: ";
		cin >> choiceOperation;
		switch (choiceOperation) {
		case 0:
			programIsOpen = false;
			break;
		case 1: begin = CreateList(begin, index); break;
		case 2: PrintList(begin, 0); break;
		case 3: begin = AddField(begin, index); break;
		case 4: begin = DeleteField(begin); break;
		case 5: SaveListToFile(begin); break;
		case 6: begin = DownloadListFromFile(begin, index); break;
		case 7:
			DeleteList(begin);
			cout << "Память очищена" << endl;
			//cout << "==============================" << endl;
			break;
		case 8:
			int modifiedElementIndex;
			cout << "Введите номер записи, которую необходимо отредактировать: ";
			cin >> modifiedElementIndex;
			begin = CorrectField(begin, modifiedElementIndex);
			break;
		case 9: begin = SortingList(begin); break;
		case 10: SearchInList(begin); break;

		default:
			cout << "Выбрана несуществующая операция" << endl;
			cout << "Выберите заново нужное действие" << endl;
			cout << "==============================" << endl;
			break;
		}
	}

	DeleteList(begin);
	cout << "Нажмите любую кнопку для выхода из программы" << endl;
	system("pause");
	return 0;
}

energyReport InputEnergyReport(int& index) {
	energyReport object;
	object.orderNumber = index++;
	cout << "Введите номер завода: ";
	cin >> object.factoryNumber;
	cout << "Введите ФИО директора: ";
	cin >> object.directorFullName;
	cout << "Введите ФИО главного энергетика: ";
	cin >> object.chiefEngineerFullName;
	cout << "Введите расход энергии по плану (в тыс. КВт∙ч): ";
	cin >> object.plannedEnergyConsumption;
	cout << "Введите сколько израсходовано фактически (в тыс. КВт∙ч): ";
	cin >> object.actualEnergyConsumption;
	return object;
}

list* AddFirstField(list* begin, list* _temp) {
	_temp->next = begin;						//связали с бывшей первой и вернули указатель на новое
	_temp->previous = NULL;
	return _temp;
}

list* AddLastField(list* begin, list* _temp) {
	if (begin == NULL) AddFirstField(begin, _temp);
	else {
		list* end = begin;
		for (; end->next != NULL; end = end->next) {}
		end->next = _temp;
		_temp->next = NULL;
		_temp->previous = end;
	}
	return begin;
}

list* AddField(list* begin, int& index) {
	int numberInList;
	list* temp = new list;
	list* end = begin;
	temp->inf = InputEnergyReport(index);
	cout << "Введите номер, на который вы хотите добавить элемент в список" << endl;
	cin >> numberInList;

	if (numberInList == 1) {
		begin = AddFirstField(begin, temp);
		cout << "Добавлено новое поле" << endl;
		cout << "==============================" << endl;
		return begin;
	}

	for (int i = 1; i < numberInList - 1; i++) {
		end = end->next;
		if (end->next == NULL && numberInList - i > 2) {
			cout << "Список содержит меньше полей, чем введеный номер" << endl;
			cout << "==============================" << endl;
			return begin;
		}
	}

	if (end->next == NULL) {
		begin = AddLastField(begin, temp);
		return begin;
	}

	temp->previous = end;
	temp->next = end->next;
	end->next->previous = temp;
	end->next = temp;
	cout << "Добавлено новое поле" << endl;
	cout << "==============================" << endl;
	return begin;
}

list* DeleteField(list* begin) {
	int numberInList;
	list* temp = new list;
	list* end = begin;
	cout << "Введите номер объекта, который вы хотите удалить из списка" << endl;
	cin >> numberInList;

	for (int i = 1; i < numberInList; i++) {
		end = end->next;
		if (end == NULL) {
			cout << "Элемента с таким номером не существует" << endl;
			cout << "==============================" << endl;
			return begin;
		}
	}
	temp = end;

	if (end->previous != NULL) {
		end->previous->next = end->next;
	}
	else {
		begin = end->next;
	}

	if (end->next != NULL) {
		end->next->previous = end->previous;
	}
	delete temp;
	cout << "Поле удалено" << endl;
	cout << "==============================" << endl;
	return begin;
}

list* CreateList(list* begin, int& index) {
	bool isCreate = false;
	do {
		list* temp = new list;
		temp->inf = InputEnergyReport(index);
		if (begin == NULL) {
			begin = AddFirstField(begin, temp);
		}
		else {
			begin = AddLastField(begin, temp);
		}
		cout << "Если хотите продолжить заполнение, введите 1" << endl;
		cout << "Для завершения заполнения введите 0" << endl;
		cin >> isCreate;
	} while (isCreate);
	cout << "Список создан" << endl;
	cout << "==============================" << endl;
	return begin;
}

void PrintList(list* begin, int _search) {
	if (begin == NULL) {
		cout << "Список пустой" << endl;
		return;
	}
	list* temp = new list;
	cout << "_______________________________________________________________________________________________________________________" << endl;
	cout << "| Порядковый номер | Номер завода | ФИО директора | ФИО главного | Расход энергии по плану | Израсходовано фактически |" << endl;
	cout << "|                  |              |               |   инженера   |      (в тыс. КВт∙ч)     |      (в тыс. КВт∙ч)      |" << endl;
	for (temp = begin; temp != NULL; temp = temp->next) {
		cout << "|" << setw(18) << temp->inf.orderNumber;
		cout << "|" << setw(14) << temp->inf.factoryNumber;
		cout << "|" << setw(15) << temp->inf.directorFullName;
		cout << "|" << setw(14) << temp->inf.chiefEngineerFullName;
		cout << "|" << setw(25) << temp->inf.plannedEnergyConsumption;
		cout << "|" << setw(26) << temp->inf.actualEnergyConsumption << "|" << endl;
		if (_search) {
			cout << "_______________________________________________________________________________________________________________________" << endl;
			return;
		}
	}
	cout << "_______________________________________________________________________________________________________________________" << endl;
	return;
} //TO DO скролинг

void SaveListToFile(list* begin) {
	bool fileExtension;
	string fileName;
	cout << "Введите название файла для сохранения: ";
	cin >> fileName;
	cout << "Выберите расширение, в котором хотите сохранить:\n0 - bin, 1 - txt" << endl;
	cin >> fileExtension;
	ofstream fout;
	if (fileExtension) {
		fileName += ".txt";
		fout.open(fileName, ios::app);
	}
	else {
		fileName += ".bin";
		fout.open(fileName, ios::binary);
	}
	if (fout.is_open()) {
		for (list* temp = begin; temp != NULL; temp = temp->next) {
			fout << temp->inf.orderNumber << "	";
			fout << temp->inf.factoryNumber << "	";
			fout << temp->inf.chiefEngineerFullName << "	";
			fout << temp->inf.directorFullName << "	";
			fout << temp->inf.plannedEnergyConsumption << "	";
			fout << temp->inf.actualEnergyConsumption << endl;
		}
		cout << "Список сохранен в файл " << fileName << endl;
		cout << "==============================" << endl;
	}
	else cerr << "Не удалось открыть файл для сохранения списка." << endl;
	system("pause");
	fout.close();
	return;
}

list* DownloadListFromFile(list* begin, int& index) {
	list* end = NULL;
	energyReport tempObject;
	bool fileExtension;
	string fileName;

	cout << "Введите название файла для загрузки: ";
	cin >> fileName;
	cout << "Выберите расширение, из которого хотите открыть:\n0 - bin, 1 - txt" << endl;
	cin >> fileExtension;
	ifstream fin;

	if (fileExtension) {
		fileName += ".txt";
		fin.open(fileName, ios::in);
	}
	else {
		fileName += ".bin";
		fin.open(fileName, ios::binary);
	}
	fin.seekg(0, ios::beg);

	if (fin.is_open()) {
		DeleteList(begin);
		while (fin >> tempObject.orderNumber) {
			fin >> tempObject.factoryNumber;
			fin >> tempObject.chiefEngineerFullName;
			fin >> tempObject.directorFullName;
			fin >> tempObject.plannedEnergyConsumption;
			fin >> tempObject.actualEnergyConsumption;
			//Создание списка
			list* temp = new list{ tempObject, nullptr, end };
			if (begin == NULL) begin = temp;
			else end->next = temp;
			end = temp;
			index++;
		}
		cout << "Список считан из файла" << endl;
		cout << "==============================" << endl;
	}
	else cerr << "Не удалось открыть файл для загрузки списка." << endl;
	system("pause");
	fin.close();
	return begin;
}

void DeleteList(list*& begin) {
	while (begin != NULL) {
		list* next = begin->next;
		delete begin;
		begin = next;
	}
	return;
}

list* CorrectField(list* begin, int modifiedElementIndex) {
	bool isElementExist = false;
	list* temp = begin;
	for (; temp->next != NULL; temp = temp->next)
		if (temp->inf.orderNumber == modifiedElementIndex) {
			isElementExist = true;
			break;
		}
	if (!isElementExist) {
		cout << "Элемента с таким номером нет в списке" << endl;
		return begin;
	}

	int userChoice;
	bool isTrue = true;
	while (isTrue) {
		cout << "Выберите, что вы хотите изменить:" << endl;
		cout << "1. Номер завода" << endl;
		cout << "2. ФИО директора" << endl;
		cout << "3. ФИО главного энергетика" << endl;
		cout << "4. Расход энергии по плану (в тыс. КВт∙ч)" << endl;
		cout << "5. Израсходовано фактически (в тыс. КВт∙ч)" << endl;
		cout << "Нажмите 0, если закончили редактирование" << endl;
		cin >> userChoice;
		switch (userChoice) {
		case 0:
			isTrue = false;
			break;
		case 1:
			cout << "Введите новый номер завода: ";
			cin >> temp->inf.factoryNumber;
			break;
		case 2:
			cout << "Введите новое ФИО директора: ";
			cin >> temp->inf.directorFullName;
			break;
		case 3:
			cout << "Введите новое ФИО главного энергетика: ";
			cin >> temp->inf.chiefEngineerFullName;
			break;
		case 4:
			cout << "Введите новый расход энергии по плану (в тыс. КВт∙ч): ";
			cin >> temp->inf.plannedEnergyConsumption;
			break;
		case 5:
			cout << "Введите новый фактический расход энергии (в тыс. КВт∙ч): ";
			cin >> temp->inf.actualEnergyConsumption;
			break;
		default:
			cout << "Некорректный выбор" << endl;
			break;
		}
	}
	return begin;
}

list* SortingList(list* begin) {
	if (begin == NULL || begin->next == NULL)
		return begin;
	list* sorted = NULL;
	list* current = begin;

	while (current != NULL) {
		list* next = current->next;

		if (sorted == NULL || sorted->inf.orderNumber >= current->inf.orderNumber) {
			current->next = sorted;
			current->previous = NULL;
			if (sorted != NULL) {
				sorted->previous = current;
			}
			sorted = current;
		}
		else {
			list* temp = sorted;
			while (temp->next != NULL && temp->next->inf.orderNumber < current->inf.orderNumber) {
				temp = temp->next;
			}
			current->next = temp->next;
			if (temp->next != NULL) {
				temp->next->previous = current;
			}
			temp->next = current;
			current->previous = temp;
		}
		current = next;
	}
	return sorted;
}

void SearchInList(list* begin) {
	int userChoice;
	cout << "Выберите по какому полю будет производиться поиск: " << endl;
	cout << "1. ФИО директора" << endl;
	cout << "2. ФИО главного инженера" << endl;
	cout << "3. Номер завода" << endl;
	cin >> userChoice;
	string searchString;
	int searchInt;
	list* temp = begin;
	switch (userChoice) {
	case 1:
		cout << "Введите ФИО директора для поиска: ";
		cin >> searchString;
		for (; temp != NULL; temp = temp->next) {
			if (temp->inf.directorFullName == searchString) {
				cout << "Найдено совпадение" << endl;
				PrintList(temp, 1);
			}
		}
		break;
	case 2:
		cout << "Введите ФИО главного инженера для поиска: ";
		cin >> searchString;
		for (; temp != NULL; temp = temp->next) {
			if (temp->inf.chiefEngineerFullName == searchString) {
				cout << "Найдено совпадение" << endl;
				PrintList(temp, 1);
			}
		}
		break;
	case 3:
		cout << "Введите номер завода для поиска: ";
		cin >> searchInt;
		for (; temp != NULL; temp = temp->next) {
			if (temp->inf.factoryNumber == searchInt) {
				cout << "Найдено совпадение" << endl;
				PrintList(temp, 1);
			}
		}
		break;
	default:
		cout << "Некорректный выбор" << endl;
		break;
	}
}



//void menu(string* _menu) {
//	enum {
//		UP = 38,
//		DOWN = 40,
//		ESC = 27,
//		ENTER = 13
//	};
//	SetConsoleTitle(L"Курсовой проект");
//
//	int selectedOption = 0;
//	int totalOptions = sizeof(_menu) / sizeof(_menu[0]);
//
//
//	int x = 40, y = 12;
//	goToXY(x, y);
//	for (int i = 0; i < (sizeof(_menu) / sizeof(*_menu)); i++) {
//		if (i == selectedOption) {
//			SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
//		}
//		SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
//		goToXY(x, y++);
//		cout << _menu[i] << endl;
//	}
//
//	HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
//	DWORD mode;
//	GetConsoleMode(hstdin, &mode);
//	SetConsoleMode(hstdin, mode& (~ENABLE_ECHO_INPUT)& (~ENABLE_LINE_INPUT));
//
//	INPUT_RECORD eventBuffer[128];
//	DWORD numEvents;
//	ReadConsoleInput(hstdin, eventBuffer, 128, &numEvents);
//
//	for (DWORD i = 0; i < numEvents; ++i) {
//		if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown) {
//			// Обработка нажатия клавиши
//			switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {
//			case UP:
//				// Кнопка "вверх"
//				selectedOption = (selectedOption - 1 + totalOptions) % totalOptions; //чтобы не выходить за пределы меню
//				break;
//			case DOWN:
//				// Кнопка "вниз"
//				selectedOption = (selectedOption + 1) % totalOptions;
//				break;
//			case ENTER:
//				// Кнопка "Enter"
//				if (selectedOption == totalOptions - 1) {
//					std::cout << "Выход из программы." << std::endl;
//					return;
//				}
//				break;
//			default:
//				// Некорректный ввод
//				break;
//			}
//		}
//	}
//}
//
//HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
//void goToXY(short _x, short _y) {
//	SetConsoleCursorPosition(hStdOut, { _x, _y });
//}
//
//void cursorVisible(bool _show, int _size = 100) {
//	CONSOLE_CURSOR_INFO cursorInfo;
//	GetConsoleCursorInfo(hStdOut, &cursorInfo);
//	cursorInfo.bVisible = _show;
//	cursorInfo.dwSize = _size;
//	SetConsoleCursorInfo(hStdOut, &cursorInfo);
//}




//1 1 1 1 1 1 1 0 0 0 0 0 0
//cout << "==============================" << endl;
