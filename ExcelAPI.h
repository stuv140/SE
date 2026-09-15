#pragma once
#import "MSO.DLL" \
    rename( "RGB", "MSORGB" )

using namespace Office;

#import "VBE6EXT.OLB"

using namespace VBIDE;

#import "C:\\Program Files\\Microsoft Office\root\\Office16\\EXCEL.EXE" \
    rename( "DialogBox", "ExcelDialogBox" ) \
    rename( "RGB", "ExcelRGB" ) \
    rename( "CopyFile", "ExcelCopyFile" ) \
    rename( "ReplaceText", "ExcelReplaceText" ) \
    exclude( "IFont", "IPicture" ) no_dual_interfaces
#include<Windows.h>
#include <iostream>
#include <string>
//#include <atlbase.h> приводит к падению программы
//template <class Type> 
/**
 * @file ExcelAPI.h
 * @author Удовченко Егор (Udovchenko.egor@yandex.ru)
 * @brief Файл содержит реализацию класса ExcelAPI
 * @version 0.1.0
 * @date 2026-02-22
 *
 * @copyright Удовченко Егор 2026
 *
 */

class ExcelAPI {
public:
    /// @brief Конструктор инициадлизирует COM интерфейс и устанавливает по умолчанию путь к новому файлу с которым будем работать.
    ExcelAPI();
    /// @brief Создание Excel файла
    int CreateExcel();
    void Visible(bool);
   // template<typename T>
    /// @brief Запись в Excel числа double
    void Range(double d);
    /// @brief Запись в Excel числа int
    void Range(int d);
    /// @brief Запись в Excel строки
    void Range(std::wstring d);
    /// @brief Определение номера ряда в Excel для записи данных
    void SetRow(unsigned long t) { i = t; }
    /// @brief Определение номера строки в Excel для записи данных
    void SetColumn(unsigned long t) { k = t; }
   // void NameSheetSet(std::string s);
    /// @brief Переименования "Листа" в Excel  
    void NameSheetSet(std::wstring s);
    /// @brief Добавление нового "Листа" в Excel 
    void AddSheet(std::wstring s);
    /// @brief Закрытие и сохранение данных в Excel 
    void QuitExcel();
    void SetString(_bstr_t test) { str = test; }
    /// @brief Деструтор освобождает COM интерфейс 
    ~ExcelAPI(){
       // if (comInitialized) 
        {
            CoUninitialize();
        }
    }
private:
    int countSheet;
    Excel::_ApplicationPtr pXL;
    Excel::_WorkbookPtr WorkBook;
    unsigned long i;
    unsigned long k;
    _bstr_t str;
 //   Excel::_WorksheetPtr pWksheet;
};


ExcelAPI::ExcelAPI()
{
    countSheet = 1;
    i = 1;
    k = 1;
    str = "e:\\job\\book.xlsx";
    HRESULT hr= CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        std::cout << "Failed to initialize COM library. Error code = 0x"
            << std::hex << hr << std::endl;
     //   return;
     }
    if (FAILED(pXL.CreateInstance("Excel.Application")))
    {
        std::cout << "Failed to initialize Excel::_Application!" << std::endl;
    
    }

}

int ExcelAPI::CreateExcel() {
    WorkBook = pXL->Workbooks->Add(Excel::xlWorksheet);

    return 0;
}

void ExcelAPI::Visible(bool temp)
{
    pXL->Visible = temp;
}


void ExcelAPI::NameSheetSet(std::wstring s) {
    _bstr_t t(s.c_str());
    Excel::_WorksheetPtr pWksheet = pXL->ActiveSheet;

    pWksheet->Name = t;
    
}

void ExcelAPI::Range(double d)
{
    Excel::_WorksheetPtr pWksheet = pXL->ActiveSheet;
    Excel::RangePtr pRange = pWksheet->Cells;
    pRange->Item[i][k] = d;
}

void ExcelAPI::Range(std::wstring d)
{
    _bstr_t t(d.c_str());
    Excel::_WorksheetPtr pWksheet = pXL->ActiveSheet;
    Excel::RangePtr pRange = pWksheet->Cells;
    pRange->Item[i][k] = t;
}

void ExcelAPI::Range(int d)
{
    Excel::_WorksheetPtr pWksheet = pXL->ActiveSheet;
    Excel::RangePtr pRange = pWksheet->Cells;
    pRange->Item[i][k] = d;
}

void ExcelAPI::AddSheet(std::wstring s)
{
    _bstr_t t(s.c_str());
    Excel::_WorksheetPtr pWksheet = //pXL->Sheets->Add();
        pXL->GetWorksheets()->Add();
    Excel::SheetsPtr pSheets = pXL->Worksheets;
    pWksheet->Name = t;
    pWksheet = pSheets->Item[1];
    pWksheet->Activate();
    countSheet++;


}



void ExcelAPI::QuitExcel()
{   
    Excel::_WorksheetPtr pWksheet = pXL->ActiveSheet;   
    pWksheet->SaveAs(str);
   pXL->Quit();
}