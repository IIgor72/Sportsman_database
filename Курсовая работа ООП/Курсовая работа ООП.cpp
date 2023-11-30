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
/*------------------------------Работа со стрелками------------------------------*/
enum {
	UP = 72,
	DOWN = 80,
	ESC = 27,
	ENTER = 13
};
/*------------------------------Шаблоны для меню------------------------------*/
const string menuOptions1[] = {			//Главный экран
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
const int N1 = 11; //количество элементов menuOptions1

const string menuOptions2[] = {			//Корректировка
		"Номер завода",
		"ФИО директора",
		"ФИО главного энергетика",
		"Расход энергии по плану (в тыс. КВт∙ч)",
		"Израсходовано фактически (в тыс. КВт∙ч)",
		"Завершить редактирование"
};
const int N2 = 6; //количество элементов menuOptions2

const string menuOptions3[] = {			//Поиск по полям
	"ФИО директора",
	"ФИО главного инженера",
	"Номер завода"
};
const int N3 = 3; //количество элементов menuOptions3

const string menuOptions4[] = {			//Выбор формата файла
	"Бинарный файл",
	"Текстовый файл"
};
const int N4 = 2; //количество элементов menuOptions4

const string menuOptions5[] = {			//Создание списка
	"Продолжить заполнение",
	"Завершить заполнение"
};
const int N5 = 2; //количество элементов menuOptions5

//Изъятие дескриптора потока вывода
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
/*------------------------------Определение функций------------------------------*/
energyReport InputEnergyReport(int& index);
list* AddFirstField(list* begin, list* _temp);
list* AddLastField(list* begin, list* _temp);
list* AddField(list* begin, int& index);
list* DeleteField(list* begin);
list* CreateList(list* begin, int& index);
void PrintList(list* begin);
void SaveListToFileTxt(list* begin, string _fileName);
void SaveListToFileBin(list* begin, string _fileName);
list* DownloadListFromFileTxt(list* begin, string _fileName, int& index);
list* DownloadListFromFileBin(list* begin, string _fileName, int& index);
void DeleteList(list*& begin, int& index);
list* CorrectField(list* begin, int modifiedElementIndex);
list* SortingList(list* begin);
void SearchInList(list* begin);
void SaveProcessing(float _planned, float _actual);
//void goToXY(short _x, short _y);
void cls();
void cursorVisible(bool _show, int _size);
int menu(const string* _menu, int _size, int& _selectedOption);

int main() {
	/*------------------------------Настройка окна------------------------------*/
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	SetConsoleTitle(L"Курсовой проект");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	system("cls");
	system("color E0");
	/*------------------------------Настройка размера шрифта------------------------------*/
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 24;
	SetCurrentConsoleFontEx(hStdOut, FALSE, &cfi);

	int index = 1;					//Нумерация записей
	int selectedOption = 0;			//Выбор пользователем операции
	string fileName;				//Название файла
	int fileExtension = 0;			//Выбор расширения файла
	bool programIsOpen = true;		//Условие работы программы
	list* begin = NULL;

	while (programIsOpen) {
		switch (menu(menuOptions1, N1, selectedOption)) {
		case -1:
			return 0;
		case 0:
			begin = CreateList(begin, index);
			cls();
			break;
		case 1:
			PrintList(begin);
			system("cls");
			break;
		case 2:
			begin = AddField(begin, index);
			cls();
			break;
		case 3:
			begin = DeleteField(begin);
			cls();
			break;
		case 4:
			cout << "Введите название файла для сохранения: ";
			cin >> fileName;
			cout << "Выберите расширение, в котором хотите сохранить:" << endl;
			if (menu(menuOptions4, N4, fileExtension)) {
				SaveListToFileTxt(begin, fileName);
			}
			else {
				SaveListToFileBin(begin, fileName);
			}
			cls();
			break;
		case 5:
			cout << "Введите название файла для загрузки: ";
			cin >> fileName;
			cout << "Выберите расширение, из которого хотите открыть:" << endl;
			if (menu(menuOptions4, N4, fileExtension)) {
				begin = DownloadListFromFileTxt(begin, fileName, index);
			}
			else {
				begin = DownloadListFromFileBin(begin, fileName, index);
			}
			cls();
			break;
		case 6:
			DeleteList(begin, index);
			cout << "Память очищена" << endl;
			cout << "==============================" << endl;
			cls();
			break;
		case 7:
			int modifiedElementIndex;
			cout << "Введите номер записи, которую необходимо отредактировать: ";
			cin >> modifiedElementIndex;
			begin = CorrectField(begin, modifiedElementIndex);
			cls();
			break;
		case 8:
			begin = SortingList(begin);
			cls();
			break;
		case 9:
			SearchInList(begin);
			cls();
			break;
		case 10:
			programIsOpen = false;
			break;
		default:
			cout << "Выбрана несуществующая операция" << endl;
			cout << "Выберите заново нужное действие" << endl;
			cout << "==============================" << endl;
			cls();
			break;
		}
	}

	DeleteList(begin, index);
	SetConsoleTextAttribute(hStdOut, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
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
	_temp->next = begin;
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
	cout << "Введите номер, на который вы хотите добавить элемент в список: ";
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
	cout << "Введите номер объекта, который вы хотите удалить из списка: ";
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
	int isCreate = 0;
	do {
		list* temp = new list;
		temp->inf = InputEnergyReport(index);
		if (begin == NULL) {
			begin = AddFirstField(begin, temp);
		}
		else {
			begin = AddLastField(begin, temp);
		}
	} while (!menu(menuOptions5, N5, isCreate));
	cout << "Список создан" << endl;
	cout << "==============================" << endl;
	return begin;
}

void PrintList(list* begin) {
	const int maxCount = 10;				//Максимум записей на 1 странице
	int currentCount;						//Текущее количество записей
	list* temp = begin;
	list* tempBeg = begin;

	float deviationSize;					//размер отклонения факта от плана
	float totalValuesPlanned = 0;			//суммарные значения по плану 
	float totalValuesActual = 0;			//суммарные значения по факту
	
	if (begin == NULL) {					
		cout << "Список пустой" << endl;
		_getch();
		return;
	}
	/*Подсчет сумарных показателей*/
	for (; temp != NULL; temp = temp->next) {
		totalValuesPlanned += temp->inf.plannedEnergyConsumption;
		totalValuesActual += temp->inf.actualEnergyConsumption;
	}

	while (true) {
		/*Вывод таблицы по maxCount элементов*/
		cout << "_____________________________________________________________________________________________________________________________________________" << endl;
		cout << "| Порядковый номер | Номер завода | ФИО директора | ФИО главного | Расход энергии по плану | Израсходовано фактически | Отклонение от плана |" << endl;
		cout << "|                  |              |               |   инженера   |      (в тыс. КВт∙ч)     |      (в тыс. КВт∙ч)      |    (в тыс. КВт∙ч)   |" << endl;
		cout << "---------------------------------------------------------------------------------------------------------------------------------------------" << endl;
		for (currentCount = 0, temp = tempBeg; temp != NULL && currentCount < maxCount; temp = temp->next, currentCount++) {
			deviationSize = temp->inf.plannedEnergyConsumption - temp->inf.actualEnergyConsumption;
			cout << "|" << setw(18) << temp->inf.orderNumber;
			cout << "|" << setw(14) << temp->inf.factoryNumber;
			cout << "|" << setw(15) << temp->inf.directorFullName;
			cout << "|" << setw(14) << temp->inf.chiefEngineerFullName;
			cout << "|" << setw(25) << temp->inf.plannedEnergyConsumption;
			cout << "|" << setw(26) << temp->inf.actualEnergyConsumption;
			cout << "|" << setw(21) << deviationSize << "|" << endl;
		}
		cout << "_____________________________________________________________________________________________________________________________________________" << endl << endl;
		cout << "Cуммарное значение расхода по плану: " << totalValuesPlanned << endl;
		cout << "Cуммарное значение фактического расхода: " << totalValuesActual << endl;
		cout << "Cуммарное значение отклонения: " << totalValuesPlanned - totalValuesActual << endl;
		
		/*Реализация скроллинга таблицы по 1 элементу*/
		currentCount = 0;
		int key = _getch();
		if (key == 224) key = _getch();

		switch (key) {
		case UP:
			// Кнопка "вверх"
			system("cls");
			if (tempBeg->previous != NULL) tempBeg = tempBeg->previous;
			break;
		case DOWN:
			// Кнопка "вниз"
			system("cls");
			if (temp != NULL) tempBeg = tempBeg->next;
			break;
		case ESC:
			//Выход назад
			SaveProcessing(totalValuesPlanned, totalValuesActual);
			return;
		default:
			cout << "Ошибка при выборе действия. Попробуйте еще раз" << endl;
			cout << "Для продолжения нажмите любую кнопку..." << endl;
			cls();
			break;
		}
	}
}

void SaveListToFileTxt(list* begin, string _fileName) {
	ofstream fout;
	_fileName += ".txt";
	fout.open(_fileName, ios::app);
	if (fout.is_open()) {
		for (list* temp = begin; temp != NULL; temp = temp->next) {
			fout << temp->inf.orderNumber << "	";
			fout << temp->inf.factoryNumber << "	";
			fout << temp->inf.chiefEngineerFullName << "	";
			fout << temp->inf.directorFullName << "	";
			fout << temp->inf.plannedEnergyConsumption << "	";
			fout << temp->inf.actualEnergyConsumption << endl;
		}
		cout << "Список сохранен в файл " << _fileName << endl;
		cout << "==============================" << endl;
	}
	else cerr << "Не удалось открыть файл для сохранения списка." << endl;
	fout.close();
	return;
}

void SaveListToFileBin(list* begin, string _fileName) {
	ofstream fout;
	_fileName += ".dat";
	fout.open(_fileName, ios::out | ios::binary);
	if (fout.is_open()) {
		uint64_t strLen;
		for (list* temp = begin; temp != NULL; temp = temp->next) {
			fout.write((char*)&temp->inf.orderNumber, sizeof(int32_t));
			fout.write((char*)&temp->inf.factoryNumber, sizeof(int32_t));

			strLen = temp->inf.directorFullName.size();
			fout.write((char*)&strLen, sizeof(uint64_t));
			fout.write(&temp->inf.directorFullName[0], strLen);

			strLen = temp->inf.chiefEngineerFullName.size();
			fout.write((char*)&strLen, sizeof(uint64_t));
			fout.write(&temp->inf.chiefEngineerFullName[0], strLen);

			fout.write((char*)&temp->inf.plannedEnergyConsumption, sizeof(float));
			fout.write((char*)&temp->inf.actualEnergyConsumption, sizeof(float));
		}
		cout << "Список сохранен в файл " << _fileName << endl;
		cout << "==============================" << endl;
	}
	else cerr << "Не удалось открыть файл для сохранения списка." << endl;
	fout.close();
	return;
}

list* DownloadListFromFileTxt(list* begin, string _fileName, int& index) {
	list* end = NULL;
	energyReport tempObject;
	ifstream fin;
	_fileName += ".txt";
	fin.open(_fileName, ios::in);
	fin.seekg(0, ios::beg);

	if (fin.is_open()) {
		DeleteList(begin, index);
		while (fin >> tempObject.orderNumber) {
			fin >> tempObject.factoryNumber;
			fin >> tempObject.directorFullName;
			fin >> tempObject.chiefEngineerFullName;
			fin >> tempObject.plannedEnergyConsumption;
			fin >> tempObject.actualEnergyConsumption;

			//Создание списка
			list* temp = new list{ tempObject, NULL, end };
			if (begin == NULL) begin = temp;
			else {
				temp->previous = end;
				end->next = temp;
			}
			end = temp;
			index++;
		}
		cout << "Список считан из файла" << endl;
		cout << "==============================" << endl;
	}
	else cerr << "Не удалось открыть файл для загрузки списка." << endl;
	fin.close();
	return begin;
}

list* DownloadListFromFileBin(list* begin, string _fileName, int& index) {
	list* end = NULL;
	energyReport tempObject;
	ifstream fin;
	_fileName += ".dat";
	fin.open(_fileName, ios::in | ios::binary);
	fin.seekg(0, ios::beg);

	if (fin.is_open()) {
		DeleteList(begin, index);
		uint64_t strLen = 0;

		while (fin.read((char*)&tempObject.orderNumber, sizeof(int32_t))) {
			fin.read((char*)&tempObject.factoryNumber, sizeof(int32_t));

			fin.read((char*)&strLen, sizeof(uint64_t));
			tempObject.directorFullName.resize(strLen);
			fin.read(&tempObject.directorFullName[0], strLen);

			fin.read((char*)&strLen, sizeof(uint64_t));
			tempObject.chiefEngineerFullName.resize(strLen);
			fin.read(&tempObject.chiefEngineerFullName[0], strLen);

			fin.read((char*)&tempObject.plannedEnergyConsumption, sizeof(float));
			fin.read((char*)&tempObject.actualEnergyConsumption, sizeof(float));

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
	fin.close();
	return begin;
}

void DeleteList(list*& begin, int& index) {
	while (begin != NULL) {
		list* next = begin->next;
		delete begin;
		begin = next;
		index--;
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

	int userChoice = 0;
	bool isTrue = true;
	while (isTrue) {
		switch (menu(menuOptions2, N2, userChoice)) {
		case -1:
			return begin;
		case 0:
			cout << "Введите новый номер завода: ";
			cin >> temp->inf.factoryNumber;
			break;
		case 1:
			cout << "Введите новое ФИО директора: ";
			cin >> temp->inf.directorFullName;
			break;
		case 2:
			cout << "Введите новое ФИО главного энергетика: ";
			cin >> temp->inf.chiefEngineerFullName;
			break;
		case 3:
			cout << "Введите новый расход энергии по плану (в тыс. КВт∙ч): ";
			cin >> temp->inf.plannedEnergyConsumption;
			break;
		case 4:
			cout << "Введите новый фактический расход энергии (в тыс. КВт∙ч): ";
			cin >> temp->inf.actualEnergyConsumption;
			break;
		case 5:
			isTrue = false;
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
	cout << "Сортировка завершена" << endl;
	cout << "==============================" << endl;
	return sorted;
}

void SearchInList(list* begin) {
	int userChoice = 0;
	string searchString;
	int searchInt;
	list* current = begin;
	list* tempBeg = NULL;
	list* end = NULL;
	switch (menu(menuOptions3, N3, userChoice)) {
	case -1:
		return;
	case 0:
		cout << "Введите ФИО директора для поиска: ";
		cin >> searchString;
		for (; current != NULL; current = current->next) {
			if (current->inf.directorFullName == searchString) {
				list* temp = new list{ current->inf, NULL, end };
				if (tempBeg == NULL) tempBeg = temp;
				else {
					temp->previous = end;
					end->next = temp;
				}
				end = temp;
			}
		}
		break;
	case 1:
		cout << "Введите ФИО главного инженера для поиска: ";
		cin >> searchString;
		for (; current != NULL; current = current->next) {
			if (current->inf.chiefEngineerFullName == searchString) {
				list* temp = new list{ current->inf, NULL, end };
				if (tempBeg == NULL) tempBeg = temp;
				else {
					temp->previous = end;
					end->next = temp;
				}
				end = temp;
			}
		}
		break;
	case 2:
		cout << "Введите номер завода для поиска: ";
		cin >> searchInt;
		for (; current != NULL; current = current->next) {
			if (current->inf.factoryNumber == searchInt) {
				list* temp = new list{ current->inf, NULL, end };
				if (tempBeg == NULL) tempBeg = temp;
				else {
					temp->previous = end;
					end->next = temp;
				}
				end = temp;
			}
		}
		break;
	default:
		cout << "Некорректный выбор" << endl;
		break;
	}

	if (tempBeg != NULL) {
		cout << "Найдено совпадение" << endl;
		PrintList(tempBeg);
	}
	else cout << "Совпадений не найдено" << endl;
	cls();
}

void SaveProcessing(float _planned, float _actual) {
	system("cls");
	ofstream fout;
	string fileName;
	cout << "Введите название файла, в который будет сохранена обработка: ";
	cin >> fileName;
	fileName += ".txt";
	fout.open(fileName, ios::app);
	if (fout.is_open()) {
		fout << "Cуммарное значение расхода по плану: " << _planned << endl;
		fout << "Cуммарное значение фактического расхода: " << _actual << endl;
		fout << "Cуммарное значение отклонения: " << _planned - _actual << endl;
	}
	else cerr << "Не удалось открыть файл для сохранения списка." << endl;
	fout.close();
	return;
}

//void goToXY(short _x, short _y) {
//	SetConsoleCursorPosition(hStdOut, { _x, _y });
//}

void cls() {
	_getch();
	system("cls");
}

void cursorVisible(bool _show, int _size) {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hStdOut, &cursorInfo);
	cursorInfo.bVisible = _show;
	cursorInfo.dwSize = _size;
	SetConsoleCursorInfo(hStdOut, &cursorInfo);
}

int menu(const string* _menu, int _size, int& _selectedOption) {
	//int selectedOption = 0;
	//size_t size = sizeof(_menu) / sizeof(_menu[0]);
	system("cls");
	cursorVisible(false, 100);

	while (true) {
		for (int i = 0; i < _size; i++) {
			if (i == _selectedOption) {
				SetConsoleTextAttribute(hStdOut, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << "-> " << _menu[i] << endl;
			}
			else {
				SetConsoleTextAttribute(hStdOut, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
				cout << "   " << _menu[i] << endl;
			}
		}
		int key = _getch();
		if (key == 224) key = _getch();

		switch (key) {
		case UP:
			// Кнопка "вверх"
			system("cls");
			_selectedOption = (_selectedOption - 1 + _size) % _size; //чтобы не выходить за пределы меню
			break;
		case DOWN:
			// Кнопка "вниз"
			system("cls");
			_selectedOption = (_selectedOption + 1) % _size;
			break;
		case ESC:
			system("cls");
			return -1;
		case ENTER:
			system("cls");
			return _selectedOption;
		default:
			cout << "Ошибка при выборе действия. Попробуйте еще раз" << endl;
			break;
		}
	}
	return 0;
}

