#pragma once
#include <string>

class BingoBook
{
public:

	BingoBook(size_t numOfPages = 0, size_t bingoSheetsOnPage = 0, const std::string& location = "");

	void		SetNumOfPages( size_t numOfPages = 0 );
	void		SetSheetsOnPage( size_t bingoSheetsOnPage = 0 );
	void		SetPDFLocation( const std::string& location = "" );
	bool		IsAbleToCreate();
	bool		CreatePDF();

private:
#ifdef _WIN64
	wchar_t*	GetSaveFileNameFromDialog();
#endif
	std::string	GetFileLocation();

private:

	size_t		numOfPages			= 0;
	size_t		bingoSheetsOnPage	= 0;
	std::string	location;
};