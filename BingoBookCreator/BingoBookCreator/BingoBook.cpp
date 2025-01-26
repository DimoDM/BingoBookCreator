#include "BingoBook.h"
#include <iostream>
#include <vector>
#include <random>
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

	HPDF_Doc pdf = HPDF_New( NULL, NULL );
	if ( !pdf )
	{
		std::cerr << "Error creating PDF document" << std::endl;
		return false;
	}

	// Create a new page
	HPDF_Page page = HPDF_AddPage( pdf );

	// Set page size (A4)
	HPDF_Page_SetSize( page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT );

	// Add table to the PDF
	//{(HPDF_REAL)595.276, (HPDF_REAL)841.89},     /* HPDF_PAGE_SIZE_A4 */
	float cell_size = 52;
	this->AddTable( pdf, page, 841.89 - ((841.89 - (cell_size * 10))  / 2), (595.276 - (cell_size * 10))  / 2, cell_size );

	// Save the PDF to a file
	const char* output_file = "D:/Development/output_table.pdf";
	HPDF_SaveToFile( pdf, output_file );

	// Clean up and release resources
	HPDF_Free( pdf );

	std::cout << "PDF with table created successfully: " << output_file << std::endl;


	return true;
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

#include<algorithm>
void BingoBook::AddTable( HPDF_Doc pdf, HPDF_Page page, float top, float left, float cell_size )
{
	// Setting font and size
	HPDF_Font font = HPDF_GetFont( pdf, "Helvetica", NULL );
	HPDF_Page_SetFontAndSize( page, font, 40 );

	// Data rows
	std::vector<std::string> rows =	// I will make chatGPT my bitch (this array is not written by me)
	{	"29",	"86",	"23",	" 2",	"65",	"97",	"92", "40", "37", "61"	,
		"60",	"57",	"78",	" 9",	"15",	"53",	"98", "70", "55", "80"	,
		"36",	"48",	"77",	"28",	"95",	"59",	"13", "44", "35", "17"	,
		"50",	"64",	"18",	"41",	"62",	" 1",	"71", "72", "33", "81"	,
		"75",	"67",	" 8",	"89",	" 3",	"69",	"74", "22", "42", "47"	,
		"14",	"82",	"38",	"24",	"16",	"30",	"31", "34", "51", "46"	,
		"25",	"73",	"43",	"49",	"93",	"56",	"11", "79", "58", " 4"	,
		"39",	"63",	"21",	"90",	"27",	"76",	"32", "20", "83", "85"	,
		"12",	"26",	" 5",	"94",	"84",	" 7",	"68", "66", "91", " 6"	,
		"19",	"52",	"45",	"100",	"87",	"88",	"10", "54", "99", "96" };

	std::random_device	rd; // Obtain a random seed
	std::mt19937		gen(rd()); // Initialize Mersenne Twister with the seed

	const auto Swap = [] ( std::string& lhs, std::string& rhs )
		{
			std::string swap = lhs;
			lhs = rhs;
			rhs = swap;
		};

	const auto GetElementsAtRandomAndExclude = [ &rows, &gen, &Swap ] () -> std::string
		{
			// Define the range [2, 87]
			std::uniform_int_distribution<> dis( 0, rows.size() - 1 );
			int random_number = dis( gen );
			if ( random_number != rows.size() - 1 )
			{
				Swap( rows[ random_number ], rows.back() );
			}
			std::string result = rows.back();
			rows.pop_back();
			return result;
		};

	for ( int i = 0; i < 10; ++i )
	{
		for ( int j = 0; j < 10 && !rows.empty(); ++j )
		{
			HPDF_Page_Rectangle( page, left + j * cell_size, top - ( i + 1 ) * cell_size, cell_size, cell_size );
			HPDF_Page_Stroke( page );
			HPDF_Page_BeginText( page );
			HPDF_Page_TextOut( page, left + j * cell_size + 5, top - ( i + 1 ) * cell_size + 10, GetElementsAtRandomAndExclude().c_str() );
			HPDF_Page_EndText( page );
		}
	}
}
