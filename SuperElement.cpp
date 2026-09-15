// SuperElem.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream> 
#include <fstream>
#include <sstream>
#include <string> 
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <iomanip> 
#include"Matrix.h"
#include"ExcelAPI.h"
//#include"Proxy.h"
// Вспомогательная функция для удаления лишних пробелов слева и справа 

std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	if (first == std::string::npos) return "";
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}
// Функция замены символа 'D' на 'E' для корректного парсинга double 
void normalize_exponent(std::string& str)
{
	std::replace(str.begin(), str.end(), 'D', 'E');
	std::replace(str.begin(), str.end(), 'd', 'e');
}
// Функция извлечения данных из строки вида "* 60 1-4.0 ..."
std::vector<std::pair<int, double>> parse_data_line(const std::string& line) {
	std::vector<std::pair<int, double>> result; // Удаляем первую звездочку 
	std::string temp = line.substr(1);
	temp = trim(temp);
	if (temp.empty()) return result;
	std::istringstream iss(temp);
	int skip_col; // В формате DMIG* после * часто идет повтор номера столбца (например, 60). Пропускаем его.
	if (!(iss >> skip_col))
		return result; int row_index; std::string value_str; // Читаем парами: индекс строки -> значение 
	while (iss >> row_index >> value_str) {
		normalize_exponent(value_str);
		try {
			double value = std::stod(value_str);
			result.emplace_back(row_index, value);
		}
		catch (...) { // Если формат числа нарушен, прекращаем чтение этой строки
			break;
		}
	} return result;
}
/**
 * SuperElements
 * \brief Приложение предназначено для парсинга файла matrix.pch и создания матрицы жесткости в эксель файле.
 * \author Udovchenko Egor
*	\version C++20
*	\date Cентябрь 2026 года
* 
 */
int main()
{
	int i = 0;
	int n = -1;
	const char* filename = "matrix.pch"; // Укажите имя вашего файла здесь s
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Ошибка: Не удалось открыть файл '" << filename << "'" << std::endl;
		return 1;
	}
	std::string line;
	int current_col = -1;
	int max_index = 0; // Используем map для хранения элементов. // Ключ: {столбец, строка}. Map автоматически отсортирует элементы и сложит дубликаты. 
	/*
	Создание двумерного массива неограниченного размера
	\code
	Matrix<double, 0>  matrix_elements;
	\endcode
	*/
	Matrix<double, 0>  matrix_elements;
	std::string begin_name;
	int currentLineCount = 0;
	/*
Цикл достает строки из файла для дальнейшего парсинга
\code
while (std::getline(file, line)){
}
\endcode
*/
	while (std::getline(file, line))
	{ // Пропускаем пустые строки 
		//std::cout << line << std::endl;
		if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue; // 1. Обнаружение заголовка нового столбца 
		/*
	Поиск слов в строке. Если слова в строке найдены, то выпоняются соответствующие блоки кода.
	\code
	if (line.rfind("BEGIN", 0) == 0){...}
	if (line.rfind("DMIG*", 0) == 0){...}
	if (line.rfind("ASET", 0) == 0){...}
	\endcode
	*/
		if (line.rfind("BEGIN", 0) == 0)
		{
			std::istringstream header(line);
			std::string tag, test;
			header >> test >> tag >> begin_name;
			//			std::cout << begin_name << std::endl;
		}
		if (line.rfind("ASET", 0) == 0)
		{
			long long number;
			std::istringstream iss(line.substr(4));

			while (iss >> number)
			{
				++currentLineCount; ///< переменная для подсчета размера заполняемой матрицы
			}

		}

		if (line.rfind("DMIG*", 0) == 0)
		{
				
			//	i++;
			n++;
			i = 0;
			if (n >= currentLineCount * 3)break;
			std::istringstream header(line);
			std::string tag, name;
			int col_id, row_start;
			header >> tag >> name >> col_id >> row_start; ///< раскладываем строку данных по соответствующим значениям переменных
			if (header.fail())
			{
				current_col = -1;
				continue;
			}
			current_col = col_id;
			max_index = (std::max)(max_index, current_col);

			continue;
		} // 2. Обработка строки со значениями 
		if (current_col != -1 && !line.empty() && line[0] == '*')
		{
			//std::cout << line << std::endl;
			auto elements = parse_data_line(line);
			/*
Заполняем редуцированную матрицу жесткости
\code
			for (auto& elem : elements) {
				int row = elem.first;
				double val = elem.second;

				matrix_elements[i][n] = val;
				matrix_elements[n][i] = val;
				i++;
				if (i >= currentLineCount * 3)i = 0;
			}
\endcode
*/
			for (auto& elem : elements) {
				int row = elem.first;
				double val = elem.second;

				matrix_elements[i][n] = val;
				matrix_elements[n][i] = val;
				i++;
				if (i >= currentLineCount * 3)i = 0;
			}
			


		}

	}
	file.close(); // --- Вывод информации о распарсеной матрице --- 
	std::cout << "=== Результаты парсинга PCH ===" << std::endl;
				for (auto& elem : elements) {
				int row = elem.first;
				double val = elem.second;

				matrix_elements[i][n] = val;
				matrix_elements[n][i] = val;
				i++;
				if (i >= currentLineCount * 3)i = 0;
			}
	///	std::cout << "Количество уникальных ненулевых координат: " << matrix_elements.size() << std::endl; // Проверка нашего примера K[243, 60] 
	ExcelAPI test;
	test.CreateExcel();
	test.Visible(true);
	test.NameSheetSet(L"Matrix");
	/*
Выводим  редуцированную матрицу жесткости в Excel
\code
for (int i = 0; i < currentLineCount * 3; i++)
	{
		for (int k = 0; k < currentLineCount * 3; k++) //currentLineCount * 6 /2
		{
			//std::cout << matrix_elements[i][k] << "	";
			test.SetRow(i+1);
			test.SetColumn(k+1);
			test.Range(matrix_elements[i][k]);

		}
		
	}
\endcode
*/
	for (int i = 0; i < currentLineCount * 3; i++)
	{
		for (int k = 0; k < currentLineCount * 3; k++) //currentLineCount * 6 /2
		{
			//std::cout << matrix_elements[i][k] << "	";
			test.SetRow(i+1);
			test.SetColumn(k+1);
			test.Range(matrix_elements[i][k]);

		}
		//std::cout << std::endl;
	}

	return 0;
}