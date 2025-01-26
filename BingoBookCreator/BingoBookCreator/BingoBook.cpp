#include "BingoBook.h"
#include <iostream>
#include <vector>
#ifdef _WIN64
#include <windows.h>
#include <codecvt>
#pragma warning (disable:4996)
#endif

BingoBook::BingoBook( size_t numOfPages, size_t bingoSheetsOnPage, const std::string& location )
{
	this->SetNumOfPages( numOfPages );
	this->SetSheetsOnPage( bingoSheetsOnPage );
	this->SetPDFLocation( location );
}

void BingoBook::SetNumOfPages( size_t pagesCount )
{
	while ( pagesCount <= 0 || pagesCount > 1000 )
	{
		std::cout << "Enter the number of pages that will be from 1 to 1000: ";
		std::cin >> pagesCount;
	}
	this->numOfPages = pagesCount;
}

void BingoBook::SetSheetsOnPage( size_t sheetsCount )
{
	while ( sheetsCount <= 0 || sheetsCount > 4 )
	{
		std::cout << "Enter the number of sheets on page that will be from 1 to 4: ";
		std::cin >> sheetsCount;
	}
	this->bingoSheetsOnPage = sheetsCount;
}

void BingoBook::SetPDFLocation( const std::string& location )
{
	if ( location.empty() )
	{
		this->location = this->GetFileLocation();
	}
	else
	{
		this->location = location;
	}
}

bool BingoBook::IsAbleToCreate()
{
	return numOfPages > 0 && bingoSheetsOnPage > 0 
		&& location.length() > 4 && location.substr( location.length() - 4, 4 ) == ".pdf";
}

bool BingoBook::CreatePDF()
{
	if ( !this->IsAbleToCreate() )
	{
		return false;
	}

	HPDF_Doc pdf = HPDF_New(NULL, NULL);
	if (!pdf) {
		std::cerr << "Error creating PDF document" << std::endl;
		return 1;
	}

	// Create a new page
	HPDF_Page page = HPDF_AddPage(pdf);

	// Set page size (A4)
	HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

	// Add table to the PDF
	this->AddTable(pdf, page, 500, 0, 50);

	// Save the PDF to a file
	const char* output_file = "D:/Development/output_table.pdf";
	HPDF_SaveToFile(pdf, output_file);

	// Clean up and release resources
	HPDF_Free(pdf);

	std::cout << "PDF with table created successfully: " << output_file << std::endl;


	return false;
}

#ifdef _WIN64
// Calls a dialog (the file explorer) in which we choose where to save out pdf file.
wchar_t* BingoBook::GetSaveFileNameFromDialog()
{
	// Initialize OPENFILENAME structure
	OPENFILENAME	ofn; // Common dialog box structure
	wchar_t* szFile
		= new wchar_t [ MAX_PATH ] {'B', 'i', 'n', 'g', 'o', ' ', 'B', 'o', 'o', 'k'};  // Buffer for file name (use wchar_t for wide characters)

	// Initialize OPENFILENAME to zero
	ZeroMemory( &ofn, sizeof( ofn ) );
	ofn.lStructSize		= sizeof( ofn ); // Must be set to the size of the OPENFILENAME structure
	ofn.hwndOwner		= NULL; // Handle to the owner window (NULL for no parent)
	ofn.lpstrFile		= szFile; // Buffer for the file name
	ofn.nMaxFile		= ( sizeof( *szFile ) * MAX_PATH ) / sizeof( szFile[ 0 ] ); // Size of the buffer (in wchar_t units)
	ofn.lpstrFilter		= L"PDF Files\0*.PDF\0All Files\0*.*\0"; // File filter (wide string)
	ofn.nFilterIndex	= 1; // Filter index (1 means the first filter, i.e., Text Files)
	ofn.lpstrFileTitle	= NULL; // File title (not used)
	ofn.nMaxFileTitle	= 0; // Size of the file title buffer (not used)
	ofn.lpstrInitialDir	= NULL; // Initial directory (NULL means use the current directory)
	ofn.lpstrTitle		= L"Save As"; // Dialog title
	ofn.Flags			= OFN_OVERWRITEPROMPT | OFN_LONGNAMES; // Simplified flags

	// Display the Save As dialog box
	if ( GetSaveFileName( &ofn ) == TRUE )
	{
		return szFile; // Return the selected file path
	}
	else
	{
		DWORD dwError = CommDlgExtendedError(); // Get the error code
		if ( dwError != 0 )
		{
			std::wcerr << L"Error code: " << dwError << std::endl;
		}
		return szFile; // No file selected or error occurred
	}
}
#endif

std::string BingoBook::GetFileLocation()
{
	std::string fileLocation;

	const auto& addSuffixIfNeeded = [ &fileLocation ] ()
		{
			if ( fileLocation.substr( fileLocation.length() - 4, 4 ) != ".pdf" )
			{
				fileLocation += ".pdf";
			}
		};

#ifdef _WIN64

	std::cout << "Choose the file location" << std::endl;

	wchar_t*		fp = this->GetSaveFileNameFromDialog();
	std::wstring	filePath = fp;

	// Manually convert wstring to string
	// Convert wstring to string (UTF-8 encoding)
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	fileLocation = converter.to_bytes(filePath);  // Convert to std::string

	if ( !fileLocation.empty() && fileLocation != "Bingo Book" /*The default name. It will come this way if we cancled the dialog.*/ )
	{
		std::cout << "Selected file path: " << fileLocation << std::endl;
	}
	else
	{
		std::cout << "No path selected or error occurred." << std::endl;
	}

	size_t pos = 0;
	while ( ( pos = fileLocation.find( "\\", pos ) ) != std::string::npos )
	{
		fileLocation.replace( pos, 1, "/" );
		pos += 1;
	}

	addSuffixIfNeeded();
	delete[] fp;

#elif

	while ( fileLocation.length() == 0 )
	{
		std::cout << "Enter a valid path for the PDF to be created: ";
		std::cin >> fileLocation;
	}
	addSuffixIfNeeded();

#endif


	return fileLocation;
}

void BingoBook::AddTable( HPDF_Doc pdf, HPDF_Page page, float top, float left, float cell_size )
{
	// Setting font and size
	HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
	HPDF_Page_SetFontAndSize(page, font, 12);

	// Data rows
	std::vector<const char*> rows = 
	{"1", "Viki", "28", "2", "Jane Smith", "34", "3", "Emily Johnson", "22"};

	size_t index = 0;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3 && index < rows.size(); ++j) {
			HPDF_Page_Rectangle(page, left + j * cell_size, top - (i + 1) * cell_size, cell_size, cell_size);
			HPDF_Page_Stroke(page);
			HPDF_Page_BeginText(page);
			HPDF_Page_TextOut(page, left + j * cell_size + 5, top - (i + 1) * cell_size + 5, rows[index]);
			HPDF_Page_EndText(page);
			index++;
		}
	}
}
