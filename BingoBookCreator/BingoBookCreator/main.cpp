#include <windows.h>
#include <commdlg.h>
#include <iostream>

#include <locale>
#include <codecvt>


#pragma warning (disable:4996)
wchar_t* GetSaveFileNameFromDialog() {
	// Initialize OPENFILENAME structure
	OPENFILENAME ofn;        // Common dialog box structure
	wchar_t* szFile = new wchar_t [ MAX_PATH ] {'B', 'i', 'n', 'g', 'o', ' ', 'B', 'o', 'o', 'k'};  // Buffer for file name (use wchar_t for wide characters)

	// Initialize OPENFILENAME to zero
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);  // Must be set to the size of the OPENFILENAME structure
	ofn.hwndOwner = NULL;  // Handle to the owner window (NULL for no parent)
	ofn.lpstrFile = szFile;  // Buffer for the file name
	ofn.nMaxFile = (sizeof(*szFile) * MAX_PATH)/ sizeof(szFile[0]); // Size of the buffer (in wchar_t units)
	ofn.lpstrFilter = L"PDF Files\0*.PDF\0All Files\0*.*\0"; // File filter (wide string)
	ofn.nFilterIndex = 1;  // Filter index (1 means the first filter, i.e., Text Files)
	ofn.lpstrFileTitle = NULL; // File title (not used)
	ofn.nMaxFileTitle = 0; // Size of the file title buffer (not used)
	ofn.lpstrInitialDir = NULL; // Initial directory (NULL means use the current directory)
	ofn.lpstrTitle = L"Save As"; // Dialog title
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_LONGNAMES;  // Simplified flags

	// Display the Save As dialog box
	if (GetSaveFileName(&ofn) == TRUE) {
		
		return szFile; // Return the selected file path
	} else {
		DWORD dwError = CommDlgExtendedError(); // Get the error code
		if (dwError != 0) {
			std::wcerr << L"Error code: " << dwError << std::endl;
		}
		return szFile; // No file selected or error occurred
	}
}

int main() {
	wchar_t* fp = GetSaveFileNameFromDialog();
	std::wstring filePath = fp;
	// Manually convert wstring to string
	// Convert wstring to string (UTF-8 encoding)
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string str = converter.to_bytes(filePath);  // Convert to std::string

	std::cout << "Converted string: " << str << std::endl;

	if (!str.empty() && str != "Bingo Book" /*The default name. It will come this way if we cancled the dialog.*/ ) {
		std::cout << "Selected file path: " << str << std::endl;
	} else {
		std::cout << "No file selected or error occurred." << std::endl;
	}

	delete[] fp;

	return 0;
}



/*
#include <iostream>
#include <hpdf.h>

// Function to add a table to the PDF
void AddTable(HPDF_Doc pdf, HPDF_Page page) {
	// Table header
	const char* columns[] = {"ID", "Name", "Age"};
	float table_x = 50.0f;  // Starting position for the table
	float table_y = 750.0f; // Starting position for the table (from top)
	float cell_width = 100.0f; // Width of each column
	float cell_height = 20.0f; // Height of each row

	// Setting font and size
	HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
	HPDF_Page_SetFontAndSize(page, font, 12);

	// Draw table header
	for (int i = 0; i < 3; ++i) {
		HPDF_Page_Rectangle(page, table_x + i * cell_width, table_y, cell_width, cell_height);
		HPDF_Page_Stroke(page);
		HPDF_Page_BeginText(page);
		HPDF_Page_TextOut(page, table_x + i * cell_width + 5, table_y + 5, columns[i]);
		HPDF_Page_EndText(page);
	}

	// Data rows
	const char* rows[][3] = {
		{"1", "Viki", "28"},
		{"2", "Jane Smith", "34"},
		{"3", "Emily Johnson", "22"}
	};

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			HPDF_Page_Rectangle(page, table_x + j * cell_width, table_y - (i + 1) * cell_height, cell_width, cell_height);
			HPDF_Page_Stroke(page);
			HPDF_Page_BeginText(page);
			HPDF_Page_TextOut(page, table_x + j * cell_width + 5, table_y - (i + 1) * cell_height + 5, rows[i][j]);
			HPDF_Page_EndText(page);
		}
	}
}

// Main function to create the PDF file
int main() {
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
	AddTable(pdf, page);

	// Save the PDF to a file
	const char* output_file = "D:/Development/output_table.pdf";
	HPDF_SaveToFile(pdf, output_file);

	// Clean up and release resources
	HPDF_Free(pdf);

	std::cout << "PDF with table created successfully: " << output_file << std::endl;
	return 0;
}
*/